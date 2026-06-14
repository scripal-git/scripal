#!/usr/bin/env node
'use strict';

const os   = require('os');
const path = require('path');
const fs   = require('fs');
const { execSync, spawnSync } = require('child_process');
const https = require('https');

const INSTALL_DIR = __dirname;
const GITHUB_BASE = 'https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/';

const PLATFORM_CONFIG = {
  linux: {
    url:     `${GITHUB_BASE}/scripal-so-linux-x64.tar.xz`,
    archive: 'scripal-linux.tar.xz',
    lib:     'libscripal.so',
  },
  win32: {
    url:     `${GITHUB_BASE}/scripal-dll-win32-x64.tar.xz`,
    archive: 'scripal-win32.tar.xz',
    lib:     'scripal.dll',
  },
};

// ─── Helper: download via https, following redirects ─────────────────────────
function download(url, dest) {
  return new Promise((resolve, reject) => {
    function get(currentUrl, redirectCount) {
      if (redirectCount > 10) return reject(new Error('Too many redirects'));
      https.get(currentUrl, (res) => {
        if (res.statusCode >= 300 && res.statusCode < 400 && res.headers.location) {
          res.resume();
          return get(res.headers.location, redirectCount + 1);
        }
        if (res.statusCode !== 200) {
          res.resume();
          return reject(new Error(`Download failed: HTTP ${res.statusCode} for ${currentUrl}`));
        }
        const file = fs.createWriteStream(dest);
        res.pipe(file);
        file.on('finish', () => file.close(resolve));
        file.on('error', (err) => { try { fs.unlinkSync(dest); } catch {} reject(err); });
      }).on('error', reject);
    }
    get(url, 0);
  });
}

// ─── Helper: find a file by extension recursively ────────────────────────────
function findByExt(dir, ext) {
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    const full = path.join(dir, entry.name);
    if (entry.isDirectory()) {
      const found = findByExt(full, ext);
      if (found) return found;
    } else if (entry.name.endsWith(ext)) {
      return full;
    }
  }
  return null;
}

// ─── Extraction strategies for .tar.xz ───────────────────────────────────────

// Strategy A: system tar (works on Linux always; on Windows 10+ build 17063+)
function extractViaTar(archivePath, tmpDir) {
  const result = spawnSync('tar', ['-xJf', archivePath, '-C', tmpDir], { encoding: 'utf8' });
  if (result.status !== 0) {
    throw new Error(`tar failed (exit ${result.status}): ${result.stderr || result.stdout}`);
  }
}

// Strategy B: 7-Zip (common Windows tool, tries both default install paths)
function extractVia7zip(archivePath, tmpDir) {
  const candidates = [
    'C:\\Program Files\\7-Zip\\7z.exe',
    'C:\\Program Files (x86)\\7-Zip\\7z.exe',
    '7z',   // if on PATH
    '7za',
  ];
  let sevenZip = null;
  for (const c of candidates) {
    try {
      execSync(`"${c}" i`, { stdio: 'ignore' });
      sevenZip = c;
      break;
    } catch {}
  }
  if (!sevenZip) throw new Error('7-Zip not found');

  // Step 1: decompress .xz → .tar
  const tarPath = archivePath.replace(/\.xz$/, '');
  const r1 = spawnSync(sevenZip, ['e', archivePath, `-o${tmpDir}`, '-y'], { encoding: 'utf8' });
  if (r1.status !== 0) throw new Error(`7z decompress failed: ${r1.stderr}`);

  // Step 2: extract .tar
  const tarFile = path.join(tmpDir, path.basename(tarPath));
  const r2 = spawnSync(sevenZip, ['x', tarFile, `-o${tmpDir}`, '-y'], { encoding: 'utf8' });
  if (r2.status !== 0) throw new Error(`7z extract failed: ${r2.stderr}`);

  // Cleanup intermediate .tar
  try { fs.unlinkSync(tarFile); } catch {}
}

// Strategy C: lzma-native npm package (pure JS, install on demand)
async function extractViaLzmaNative(archivePath, tmpDir) {
  let lzma;
  try {
    lzma = require('lzma-native');
  } catch {
    console.log('  ℹ  Installing lzma-native for .xz decompression …');
    execSync('npm install lzma-native --no-save', { stdio: 'inherit', cwd: INSTALL_DIR });
    lzma = require('lzma-native');
  }
  const tar = require('tar');

  await new Promise((resolve, reject) => {
    const input    = fs.createReadStream(archivePath);
    const decomp   = lzma.createDecompressor();
    const extract  = tar.extract({ cwd: tmpDir });
    input.pipe(decomp).pipe(extract);
    extract.on('finish', resolve);
    extract.on('error', reject);
    decomp.on('error', reject);
  });
}

// Master extract function – tries strategies in order until one works
async function extractLib(archivePath, targetLibName) {
  const tmpDir = path.join(INSTALL_DIR, '_scripal_tmp');
  if (fs.existsSync(tmpDir)) fs.rmSync(tmpDir, { recursive: true });
  fs.mkdirSync(tmpDir, { recursive: true });

  const strategies = [
    { name: 'system tar',   fn: () => extractViaTar(archivePath, tmpDir) },
    { name: '7-Zip',        fn: () => extractVia7zip(archivePath, tmpDir) },
    { name: 'lzma-native',  fn: () => extractViaLzmaNative(archivePath, tmpDir) },
  ];

  let lastErr;
  for (const { name, fn } of strategies) {
    try {
      console.log(`  … trying extraction via ${name}`);
      await fn();
      console.log(`  ✔  Extracted using ${name}`);
      lastErr = null;
      break;
    } catch (err) {
      console.log(`  ✗  ${name} failed: ${err.message}`);
      lastErr = err;
      // Clean tmpDir for next attempt
      try { fs.rmSync(tmpDir, { recursive: true }); fs.mkdirSync(tmpDir); } catch {}
    }
  }

  if (lastErr) {
    fs.rmSync(tmpDir, { recursive: true });
    throw new Error(
      'All extraction strategies failed. Please install 7-Zip (https://www.7-zip.org) and re-run:\n' +
      '  node install.js\n' +
      `Last error: ${lastErr.message}`
    );
  }

  const ext   = path.extname(targetLibName); // .dll or .so
  const found = findByExt(tmpDir, ext);
  if (!found) {
    fs.rmSync(tmpDir, { recursive: true });
    throw new Error(`No *${ext} file found inside the archive after extraction.`);
  }

  const dest = path.join(INSTALL_DIR, targetLibName);
  fs.copyFileSync(found, dest);
  console.log(`  ✔  Library installed → ${dest}`);

  // Cleanup
  fs.rmSync(tmpDir, { recursive: true });
  try { fs.unlinkSync(archivePath); } catch {}
}

// ─── Helper: ensure @makeomatic/ffi-napi is available ────────────────────────
function ensureFfiNapi() {
  try {
    require.resolve('@makeomatic/ffi-napi');
    console.log('  ✔  @makeomatic/ffi-napi is already installed.');
  } catch {
    console.log('  ℹ  @makeomatic/ffi-napi not found – installing now …');
    execSync('npm install @makeomatic/ffi-napi ref-napi --save', { stdio: 'inherit', cwd: INSTALL_DIR });
    console.log('  ✔  @makeomatic/ffi-napi installed.');
  }
}

// ─── Main ─────────────────────────────────────────────────────────────────────
async function main() {
  console.log('\n╔══════════════════════════════════════╗');
  console.log('║        scripal postinstall setup     ║');
  console.log('╚══════════════════════════════════════╝\n');

  // 1. Check ffi-napi
  console.log('① Checking @makeomatic/ffi-napi …');
  ensureFfiNapi();

  // 2. Detect OS
  const platform = os.platform();
  console.log(`\n② Detected platform: ${platform} (${os.arch()})`);

  const config = PLATFORM_CONFIG[platform];
  if (!config) {
    console.warn(`\n⚠  Platform "${platform}" is not supported by scripal.`);
    console.warn('   Supported: linux x64, win32 x64. Skipping download.\n');
    return;
  }

  // 3. Skip if already installed
  const libDest = path.join(INSTALL_DIR, config.lib);
  if (fs.existsSync(libDest)) {
    console.log(`\n③ ${config.lib} already present – skipping download.`);
    return;
  }

  // 4. Download
  const archivePath = path.join(INSTALL_DIR, config.archive);
  // Remove stale partial download if present
  if (fs.existsSync(archivePath)) fs.unlinkSync(archivePath);

  console.log(`\n③ Downloading ${config.lib} from GitHub …`);
  console.log(`   ${config.url}`);
  await download(config.url, archivePath);
  const sizeMB = (fs.statSync(archivePath).size / 1024 / 1024).toFixed(2);
  console.log(`  ✔  Download complete (${sizeMB} MB)`);

  // 5. Extract
  console.log(`\n④ Extracting ${config.archive} …`);
  await extractLib(archivePath, config.lib);

  console.log('\n✅ scripal setup complete!\n');
}

main().catch((err) => {
  console.error('\n❌ scripal postinstall error:', err.message);
  process.exit(0); // Non-fatal – don't block npm install
});

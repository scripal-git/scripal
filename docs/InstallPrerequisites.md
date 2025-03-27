# Scripal: prerequisites for common usage

## Text tools 
If you want to search in docx or PDF files using Scripal, external utilities are needed:

### pandoc

install from <https://pandoc.org/installing.html>

on **Linux** type systems, the package is often found in the standard repository 

**Ubuntu**: <https://launchpad.net/ubuntu/plucky/+package/pandoc>

**Debian**: <https://packages.debian.org/bookworm/pandoc>

**Fedora**: <https://packages.fedoraproject.org/pkgs/pandoc/pandoc/>

**Windows**: <https://github.com/jgm/pandoc/releases/tag/3.5>

under **Windows**: specify path to binary pandoc in **config.frmReader** or put binary in same folder as scripal 

### pdftotext (poppler-utils)

install from <https://www.xpdfreader.com/pdftotext-man.html>

on **Linux** type systems, the package is often found in the standard repository 

**Ubuntu**: <https://launchpad.net/ubuntu/plucky/+package/poppler-utils>

**Debian**: <https://packages.debian.org/bookworm/poppler-utils>

**Fedora**: <https://packages.fedoraproject.org/pkgs/poppler/poppler-utils/>

**Windows**: <http://www.xpdfreader.com/download.html>

under **Windows**: specify path to binary pdftotext in **config.pdfReader** or put binary in same folder as scripal 

 
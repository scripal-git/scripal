// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

// Scripal in Java

package scripal;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import com.sun.jna.ptr.*;
import java.util.ArrayList;
import java.util.Locale;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;

/* Scripal wrapper for Java */
// specfiy JNA jar in CLASSPATH, Java Native Access is needed to 
// create package Linux , specfiy JNA jar in CLASSPATH
// javac -d . -cp "/usr/share/maven-repo/net/java/dev/jna/jna/debian/jna-debian.jar" base.java  
// create package Windows
// javac -d . -cp ".\jna-jpms-5.16.0.jar" base.java

// build jar in /java directory Linux
// jar cf scripalwrapper.jar ./scripal/*
// build jar in /java directory Windows
// jar cf scripalwrapper.jar ./scripal/*
// set PATH or specify jar.exe ->"C:\Program Files\Java\jdk-23\bin\jar.exe" cf scripalwrapper.jar ./scripal/*

public class base {

    // Global library pointer and flags
    public static ScripalLib scripalLib = null;
    public static boolean initialized = false;
    
    // encodings
    public static final int ENC_DEFAULT  = 1;
    public static final int ENC_UTF8     = 2;
    public static final int ENC_UTF16L   = 3;
    public static final int ENC_UTF16B   = 4;
    public static final int ENC_UTF32L   = 5;
    public static final int ENC_UTF32B   = 6;
    public static final int ENC_ASCII    = 10;
    public static final int ENC_CP932    = 11;
    public static final int ENC_CP936    = 12;
    public static final int ENC_CP949    = 13;
    public static final int ENC_CP950    = 14;
    public static final int ENC_LATIN1   = 30;
    public static final int ENC_LATIN2   = 31;
    public static final int ENC_LATIN9   = 32;
    public static final int ENC_WIN874   = 50;
    public static final int ENC_WIN1250  = 51;
    public static final int ENC_WIN1251  = 52;
    public static final int ENC_WIN1252  = 53;
    public static final int ENC_WIN1253  = 54;
    public static final int ENC_WIN1254  = 55;
    public static final int ENC_WIN1255  = 56;
    public static final int ENC_WIN1256  = 57;
    public static final int ENC_WIN1257  = 58;
    public static final int ENC_WIN1258  = 59;
    
    // matching algorithms (block and nearest)
    public static final int PATTERN_LEVEN_WORD = 1;
    public static final int PATTERN_LEVENPLUS_WORD = 2;
    public static final int PATTERN_LEVEN = 3;
    public static final int PATTERN_JARO = 100;
    public static final int PATTERN_JAROWINKLER = 101;
    public static final int PATTERN_JAROWINKLER_WORD = 102;
    
    // match models
    public static final int MATCH_SOURCE      = 1;
    public static final int MATCH_NEAREST     = 2;
    public static final int MATCH_BLOCK       = 3;
    
    // result position types
    public static final int POS_UTF8          = 1;
    public static final int POS_OFFSET        = 2;
    public static final int POS_COUNT         = 3;
    
    // constants
    public static final int NO_POS = -1;
    public static final int NO_VALUE = -2;
    
    // globals
    public static boolean scripalInit = false;
    public static TFConfig config = null;
    public static TFTemplate templates = null;
    public static String javaEncoding = "";

    // Definition of the native library interface using JNA
    public interface ScripalLib extends Library {
        // printText
        void printText(byte[] text, int len, int encoding);
        
        // getVersion
        void getVersion(PointerByReference verC, IntByReference lenC);
        
        // base initialization and finalization
        void baseInit();
        void baseFinalize();
        
        // library initialization and finalization
        void libInit(byte[] configPath, int len, int encoding);
        void libFinalize();
        
        // log initialization and management
        void logInit(byte[] logChannel, int len);
        void getLogBuf(PointerByReference bufC, IntByReference lenC);       
        void clearLogBuf();

        // pointer deletion
        void deletePtr(Pointer ptr);
        
        // error retrieval functions
        void getErrMsg(PointerByReference errC, IntByReference lenC);
        void getErrExp(PointerByReference errC, IntByReference lenC);
        int getErrPos();
        void clearErr();
        
        // encoding setting
        void setEncoding(int encoding);
        
        // new object functions
        int newCodeObj(byte[] source, int len);
        int newNearestObj(byte[] source, int len, double dist);
        int newBlockObj(byte[] source, int len, double dist);
        
        // delete object
        void deleteObj(int id);
        
        // matching functions
        boolean match(int id, byte[] text, int len);
        boolean matchFile(int id, byte[] file, int len, int fileEncoding);
        boolean replace(int id, byte[] text, int len, byte[] replace, int lenReplace);
        boolean replaceFile(int id, byte[] fileIn, int lenIn, byte[] fileOut, int lenOut, byte[] replace, int lenReplace, int fileEncoding);
        boolean split(int id, byte[] text, int len);
        boolean splitFile(int id, byte[] fileIn, int lenIn, byte[] fileOut, int lenOut, int fileEncoding);
        boolean searchFiles(int id, byte[] path, int len, byte[] dirExtension, int lenDirExtension, boolean recursive);
        
        // result functions
        int getResultSize(int id);
        boolean getResultSpecs(int id, int index, IntByReference encoding, IntByReference matchType, IntByReference positionType);
        boolean getResultStr(int id, int index, PointerByReference resultC, IntByReference lenC);
        boolean getResultFileName(int id, int index, PointerByReference resultC, IntByReference lenC);
        boolean getResultTag(int id, int index, PointerByReference resultC, IntByReference lenC);
        boolean getResultPos(int id, int index, IntByReference posStart, IntByReference posEnd);
        boolean getResultRating(int id, int index, DoubleByReference rating);
        boolean getResultJSON(int id, PointerByReference resultC, IntByReference lenC);
        boolean getResultCSV(int id, PointerByReference resultC, IntByReference lenC);
        boolean getResultHRF(int id, PointerByReference resultC, IntByReference lenC);
        
        // configuration functions
        boolean configSetLocale(byte[] locale, int len);
        boolean configToJSON(PointerByReference resultC, IntByReference lenC);
        boolean configToJSONVal(PointerByReference resultC, IntByReference lenC, byte[] names, int lenNames);
        boolean configFromJSON(byte[] textJSON, int len);
        boolean configStore(byte[] file, int len);
        boolean configRestore(byte[] file, int len);
        
        // template functions
        boolean templateSet(byte[] name, int lenName, byte[] source, int lenSource);
        boolean templateClear(byte[] name, int lenName);
        void templateReset();
        boolean templateGet(byte[] name, int lenName, PointerByReference resultC, IntByReference lenC);
        boolean templateStore(byte[] file, int len);
        boolean templateRestore(byte[] file, int len);
        boolean templatesToJSON(PointerByReference resultC, IntByReference lenC);
        boolean templatesToJSONVal(PointerByReference resultC, IntByReference lenC, byte[] names, int lenNames);
        boolean templatesFromJSON(byte[] textJSON, int len);
    }
    
    // baseInit function
    public static void baseInit() {
        baseInit("");
    }
    
    public static void baseInit(String libPath) {
        // global scripalLib and initialized
        if (System.getProperty("os.name").toLowerCase().contains("linux")) {
            if (libPath.isEmpty()) {
                libPath = "/usr/lib/scripal/libscripal.so";
                Path path = FileSystems.getDefault().getPath(libPath);
                if (!Files.exists(path)) 
                    libPath = "/usr/lib64/scripal/libscripal.so"; 
            }
            scripalLib = com.sun.jna.Native.load(libPath, ScripalLib.class);
        }
        if (System.getProperty("os.name").toLowerCase().contains("win")) {
            if (libPath.isEmpty()) {
                libPath = System.getProperty("user.dir");
            }
            // Add the lib path to the search path for Windows DLLs
            com.sun.jna.NativeLibrary.addSearchPath("scripal", libPath);
            scripalLib = com.sun.jna.Native.load("scripal", ScripalLib.class);
        }
        
        // Call baseInit
        scripalLib.baseInit();
        initialized = true;
        return;
    }
    
    // getVersion function
    public static String getVersion() {
        PointerByReference verC = new PointerByReference();
        IntByReference lenC = new IntByReference(0);
        scripalLib.getVersion(verC, lenC);
        String version = verC.getValue().getString(0);
        scripalLib.deletePtr(verC.getValue());
        return version;
    }
    
    // baseFinalize function
    public static void baseFinalize() {
        scripalLib.baseFinalize();
        return;
    }
    
    // libInit function
    public static void libInit(String configPath, int encoding) {
        if (!initialized) {
            baseInit();
        }

        switch(encoding) {
          case ENC_DEFAULT  : javaEncoding = "UTF-8"; break;  
          case ENC_UTF8     : javaEncoding = "UTF-8"; break;  
          case ENC_UTF16B   : javaEncoding = "UTF-16BE"; break;  
          case ENC_UTF16L   : javaEncoding = "UTF-16LE"; break;  
          case ENC_UTF32B   : javaEncoding = "UTF-32BE"; break;  
          case ENC_UTF32L   : javaEncoding = "UTF-32LE"; break;  
        }
        clearErr();
        byte[] configPathC = null;
        try {
            configPathC = configPath.getBytes("UTF-8");
        } catch (Exception e) {
            e.printStackTrace();
        }
        scripalLib.libInit(configPathC, configPathC.length, encoding);
        config = new TFConfig();
        templates = new TFTemplate();
        if (!getErrMsg().isEmpty()) {
            throw new IllegalArgumentException(getErrExp());
        }
        if (!scripalInit) {
            scripalInit = true;
            templates = new TFTemplate();
        }
        return;
    }
    
    // Overloaded libInit with default encoding = ENC_DEFAULT (if needed)
    public static void libInit(String configPath) {
        libInit(configPath, ENC_DEFAULT);
    }
    
    // libFinalize function
    public static void libFinalize() {
        scripalLib.libFinalize();
        return;
    }
    
    // logInit function
    public static void logInit(String logChannel) {
        clearErr();
        byte[] logChannelC = null;
        try {
            logChannelC = logChannel.getBytes("UTF-8");
        } catch (Exception e) {
            e.printStackTrace();
        }
        scripalLib.logInit(logChannelC, logChannelC.length);
        if (!getErrMsg().isEmpty()) {
            throw new IllegalArgumentException(getErrExp());
        }
        return;
    }
    
    // getLogBuf function
    public static String getLogBuf() {
        PointerByReference bufC = new PointerByReference();
        IntByReference lenC = new IntByReference(0);
        scripalLib.getLogBuf(bufC, lenC);
        String logBuf = bufC.getValue().getString(0);
        scripalLib.deletePtr(bufC.getValue());
        return logBuf;
    }

    // clearLogBuf function
    public static void clearLogBuf() {
        scripalLib.clearLogBuf();
        return;
    }

    // getErrMsg function
    public static String getErrMsg() {
        PointerByReference errC = new PointerByReference();
        IntByReference lenC = new IntByReference(0);
        scripalLib.getErrMsg(errC, lenC);
        String errMsg = errC.getValue().getString(0);
        scripalLib.deletePtr(errC.getValue());
        return errMsg;
    }
    
    // getErrExp function
    public static String getErrExp() {
        PointerByReference errC = new PointerByReference();
        IntByReference lenC = new IntByReference(0);
        scripalLib.getErrExp(errC, lenC);
        String errMsg = errC.getValue().getString(0);
        scripalLib.deletePtr(errC.getValue());
        return errMsg;
    }
    
    // getErrPos function
    public static int getErrPos() {
        return scripalLib.getErrPos();
    }
    
    // clearErr function
    public static void clearErr() {
        scripalLib.clearErr();
        return;
    }
    
    // setEncoding function
    public static void setEncoding(int encoding) {
        clearErr();
        scripalLib.setEncoding(encoding);
        if (!getErrMsg().isEmpty()) {
            throw new IllegalArgumentException(getErrExp());
        }
        return;
    }
    
    // testError function
    public static void testError() {
        // Here we simulate passing an invalid pointer by constructing a Pointer with value 1.
        scripalLib.deletePtr(Pointer.createConstant(1));
        return;
    }
    
    // TFResults class
    public static class TFResults {
        public int id = -1;
        public int encoding = ENC_UTF8;
        public int matchType = MATCH_SOURCE;
        public int positionType = POS_UTF8;
        public ArrayList<String> fileNames = new ArrayList<String>();
        public ArrayList<String> text = new ArrayList<String>();
        public ArrayList<int[]> positions = new ArrayList<int[]>();
        public ArrayList<Double> ratings = new ArrayList<Double>();
        public ArrayList<String> tags = new ArrayList<String>();
        
        public int size() {
            return scripalLib.getResultSize(this.id);
        }
    }
    
    // TFScripal class
    public static class TFScripal implements AutoCloseable {
        public int id = -1;
        public TFResults results = new TFResults();
        
        public TFScripal(String matchtype, String source) {
            clearErr();
            byte[] sourceC = null;
            try {
                sourceC = source.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }

            if (matchtype.toLowerCase().equals("source")) {
                this.id = scripalLib.newCodeObj(sourceC, sourceC.length);
            }
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return;
        }

        public TFScripal(String matchtype, String source, double dist) {
            clearErr();
            byte[] sourceC = null;
            try {
                sourceC = source.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            if (matchtype.toLowerCase().equals("nearest")) {
                this.id = scripalLib.newNearestObj(sourceC, sourceC.length, dist);
            } else if (matchtype.toLowerCase().equals("block")) {
                this.id = scripalLib.newBlockObj(sourceC, sourceC.length, dist);
            }
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return;
        }
        
        // finalize method to clean up native object
        @Override
        public void close() {
          scripalLib.deleteObj(this.id);
          return;
        }
        
        public void fillResults() {
            clearErr();
            PointerByReference resultC = new PointerByReference();
            IntByReference lenC = new IntByReference(0);
            IntByReference posStart = new IntByReference(0);
            IntByReference posEnd = new IntByReference(0);
            IntByReference encodingRef = new IntByReference(0);
            IntByReference matchTypeRef = new IntByReference(0);
            IntByReference positionTypeRef = new IntByReference(0);
            DoubleByReference rating = new DoubleByReference(0);
            
            this.results.id = this.id;
            this.results.fileNames.clear();
            this.results.text.clear();
            this.results.positions.clear();
            this.results.ratings.clear();
            this.results.tags.clear();
            
            int size = getResultSize();
            for (int index = 0; index < size; index++) {
                boolean resultBool = scripalLib.getResultSpecs(this.id, index, encodingRef, matchTypeRef, positionTypeRef);
                if (!getErrMsg().isEmpty()) {
                    throw new IllegalArgumentException(getErrExp());
                }
                this.results.encoding = encodingRef.getValue();
                this.results.matchType = matchTypeRef.getValue();
                this.results.positionType = positionTypeRef.getValue();
                
                resultBool = scripalLib.getResultFileName(this.id, index, resultC, lenC);
                if (!getErrMsg().isEmpty()) {
                    throw new IllegalArgumentException(getErrExp());
                }
                String fileName = resultC.getValue().getString(0);
                this.results.fileNames.add(fileName);
                
                resultBool = scripalLib.getResultStr(this.id, index, resultC, lenC);
                if (!getErrMsg().isEmpty()) {
                    throw new IllegalArgumentException(getErrExp());
                }
                String textResult = resultC.getValue().getString(0);
                this.results.text.add(textResult);
                
                scripalLib.getResultPos(this.id, index, posStart, posEnd);
                if (!getErrMsg().isEmpty()) {
                    throw new IllegalArgumentException(getErrExp());
                }
                int[] posArray = { posStart.getValue(), posEnd.getValue() };
                this.results.positions.add(posArray);
                
                scripalLib.getResultRating(this.id, index, rating);
                if (!getErrMsg().isEmpty()) {
                    throw new IllegalArgumentException(getErrExp());
                }
                this.results.ratings.add(rating.getValue());
                
                resultBool = scripalLib.getResultTag(this.id, index, resultC, lenC);
                if (!getErrMsg().isEmpty()) {
                    throw new IllegalArgumentException(getErrExp());
                }
                String tag = resultC.getValue().getString(0);
                this.results.tags.add(tag);
            }
            return;
        }
        
        public boolean match(String text) {
            clearErr();
            byte[] textC = null;
            try {
                textC = text.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.match(this.id, textC, textC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            fillResults();
            return result;
        }
        
        public boolean matchFile(String file, int fileEncoding) {
            clearErr();
            byte[] fileC = null;
            try {
                fileC = file.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.matchFile(this.id, fileC, fileC.length, fileEncoding);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            fillResults();
            return result;
        }
        
        // Overloaded matchFile with default encoding ENC_UTF8
        public boolean matchFile(String file) {
            return matchFile(file, ENC_UTF8);
        }
        
        public boolean replace(String text, String replace) {
            clearErr();
            byte[] textC = null;
            byte[] replaceC = null;
            try {
                textC = text.getBytes("UTF-8");
                replaceC = replace.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.replace(this.id, textC, textC.length, replaceC, replaceC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            fillResults();
            return result;
        }
        
        public boolean replaceFile(String fileIn, String fileOut, String replace, int fileEncoding) {
            clearErr();
            byte[] fileInC = null;
            byte[] fileOutC = null;
            byte[] replaceC = null;
            try {
                fileInC = fileIn.getBytes("UTF-8");
                fileOutC = fileOut.getBytes("UTF-8");
                replaceC = replace.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.replaceFile(this.id, fileInC, fileInC.length, fileOutC, fileOutC.length, 
                                                      replaceC, replaceC.length, fileEncoding);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            fillResults();
            return result;
        }
        
        // Overloaded replaceFile with default encoding ENC_UTF8
        public boolean replaceFile(String fileIn, String fileOut, String replace) {
            return replaceFile(fileIn, fileOut, replace, ENC_UTF8);
        }
        
        public boolean split(String text) {
            clearErr();
            byte[] textC = null;
            try {
                textC = text.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.split(this.id, textC, textC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            fillResults();
            return result;
        }
        
        public boolean splitFile(String fileIn, String fileOut, int fileEncoding) {
            clearErr();
            byte[] fileInC = null;
            byte[] fileOutC = null;
            try {
                fileInC = fileIn.getBytes("UTF-8");
                fileOutC = fileOut.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.splitFile(this.id, fileInC, fileInC.length, fileOutC, fileOutC.length, fileEncoding);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            fillResults();
            return result;
        }
        
        // Overloaded splitFile with default encoding ENC_UTF8
        public boolean splitFile(String fileIn, String fileOut) {
            return splitFile(fileIn, fileOut, ENC_UTF8);
        }
        
        public boolean searchFiles(String path, String dirExtension, boolean recursive) {
            clearErr();
            byte[] pathC = null;
            byte[] dirExtensionC = null;
            try {
                pathC = path.getBytes("UTF-8");
                dirExtensionC = dirExtension.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.searchFiles(this.id, pathC, pathC.length, dirExtensionC, dirExtensionC.length, recursive);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            fillResults();
            return result;
        }
        
        public int getResultSize() {
            return scripalLib.getResultSize(this.id);
        }
        
        public String getResultJSON() {
            clearErr();
            PointerByReference resultC = new PointerByReference();
            IntByReference lenC = new IntByReference(0);
            scripalLib.getResultJSON(this.id, resultC, lenC);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            String result = resultC.getValue().getString(0);
            scripalLib.deletePtr(resultC.getValue());
            return result;
        }
        
        public String getResultCSV() {
            clearErr();
            PointerByReference resultC = new PointerByReference();
            IntByReference lenC = new IntByReference(0);
            scripalLib.getResultCSV(this.id, resultC, lenC);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            String result = resultC.getValue().getString(0);
            scripalLib.deletePtr(resultC.getValue());
            return result;
        }
        
        public String getResultHRF() {
            clearErr();
            PointerByReference resultC = new PointerByReference();
            IntByReference lenC = new IntByReference(0);
            scripalLib.getResultHRF(this.id, resultC, lenC);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            String result = resultC.getValue().getString(0);
            scripalLib.deletePtr(resultC.getValue());
            return result;
        }
    }
    
    // TFConfig class
    public static class TFConfig {
        public TFConfig(Object config) {
            return;
        }
        public TFConfig() {
            return;
        }
        
        public boolean setLocale(String locale) {
            clearErr();
            byte[] localeC = null;
            try {
                localeC = locale.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.configSetLocale(localeC, localeC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return result;
        }
        
        public String toJSON() {
            clearErr();
            PointerByReference resultC = new PointerByReference();
            IntByReference lenC = new IntByReference(0);
            scripalLib.configToJSON(resultC, lenC);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            String result = resultC.getValue().getString(0);
            scripalLib.deletePtr(resultC.getValue());
            return result;
        }
        
        public String toJSONVal(String names) {
            clearErr();
            PointerByReference resultC = new PointerByReference();
            IntByReference lenC = new IntByReference(0);
            byte[] namesC = null;
            try {
                namesC = names.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            scripalLib.configToJSONVal(resultC, lenC, namesC, namesC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            String result = resultC.getValue().getString(0);
            scripalLib.deletePtr(resultC.getValue());
            return result;
        }
        
        public boolean fromJSON(String textJSON) {
            clearErr();
            byte[] textJSONC = null;
            try {
                textJSONC = textJSON.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.configFromJSON(textJSONC, textJSONC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return result;
        }
        
        public boolean store(String file) {
            clearErr();
            byte[] fileC = null;
            try {
                fileC = file.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.configStore(fileC, fileC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return result;
        }
        
        public boolean restore(String file) {
            clearErr();
            byte[] fileC = null;
            try {
                fileC = file.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.configRestore(fileC, fileC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return result;
        }
    }
    
    // TFTemplate class
    public static class TFTemplate {
        public boolean set(String name, String source) {
            clearErr();
            byte[] nameC = null;
            byte[] sourceC = null;
            try {
                nameC = name.getBytes("UTF-8");
                sourceC = source.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.templateSet(nameC, nameC.length, sourceC, sourceC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return result;
        }
        
        public boolean clear(String name) {
            clearErr();
            byte[] nameC = null;
            try {
                nameC = name.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.templateClear(nameC, nameC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return result;
        }
        
        public void reset() {
            scripalLib.templateReset();
            return;
        }
        
        public String get(String name) {
            clearErr();
            PointerByReference resultC = new PointerByReference();
            IntByReference lenC = new IntByReference(0);
            byte[] nameC = null;
            try {
                nameC = name.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean resultBool = scripalLib.templateGet(nameC, nameC.length, resultC, lenC);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            String result = resultC.getValue().getString(0);
            scripalLib.deletePtr(resultC.getValue());
            return result;
        }

        public String toJSON() {
            clearErr();
            PointerByReference resultC = new PointerByReference();
            IntByReference lenC = new IntByReference(0);
            boolean resultBool = scripalLib.templatesToJSON(resultC, lenC);
            if (!getErrMsg().equals("")) {
              throw new IllegalArgumentException(getErrExp());
            }
            String result = new String(resultC.getValue().getByteArray(0, lenC.getValue()), StandardCharsets.UTF_8);
            scripalLib.deletePtr(resultC.getValue());
            return result;
        }

        public String toJSONVal(String names) {
            clearErr();
            PointerByReference resultC = new PointerByReference();
            IntByReference lenC = new IntByReference(0);
            byte[] namesC = null;
            try {
              namesC = names.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }; 
            boolean resultBool = scripalLib.templatesToJSONVal(resultC, lenC, namesC, names.getBytes(StandardCharsets.UTF_8).length);
            if (!getErrMsg().equals("")) {
              throw new IllegalArgumentException(getErrExp());
            }
            String result = new String(resultC.getValue().getByteArray(0, lenC.getValue()), StandardCharsets.UTF_8);
            scripalLib.deletePtr(resultC.getValue());
            return result;
        }

        public boolean fromJSON(String textJSON) {
            clearErr();
            byte[] textJSONC = null;
            try {
              textJSONC = textJSON.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            };
            boolean result = scripalLib.templatesFromJSON(textJSONC, textJSON.getBytes(StandardCharsets.UTF_8).length);
            if (!getErrMsg().equals("")) {
               throw new IllegalArgumentException(getErrExp());
            }
            return result;
        }
        
        public boolean store(String file) {
            clearErr();
            byte[] fileC = null;
            try {
                fileC = file.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.templateStore(fileC, fileC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return result;
        }
        
        public boolean restore(String file) {
            clearErr();
            byte[] fileC = null;
            try {
                fileC = file.getBytes("UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            boolean result = scripalLib.templateRestore(fileC, fileC.length);
            if (!getErrMsg().isEmpty()) {
                throw new IllegalArgumentException(getErrExp());
            }
            return result;
        }
    }
}
  
/* End of base */
  
   
  
   
  
   
  
   
  
   
  
   
  
   
  

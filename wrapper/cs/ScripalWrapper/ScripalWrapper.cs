// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

using System;
using System.Text;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace ScripalWrapper
{
    // Scripal wrapper for C#

    public static class Scripal
    {
        public static IntPtr scripalLib = IntPtr.Zero;
        public static bool initialized = false;

        // encodings
        public const int ENC_DEFAULT = 1;
        public const int ENC_UTF8 = 2;
        public const int ENC_UTF16L = 3;
        public const int ENC_UTF16B = 4;
        public const int ENC_UTF32L = 5;
        public const int ENC_UTF32B = 6;
        public const int ENC_ASCII = 10;  //#  ASCII , Extended ASCII (up to 255)
        public const int ENC_CP932 = 11;  //#  CP932 DBCS, Japanese characters
        public const int ENC_CP936 = 12;  //#  CP93 6DBCS, simple Chinese characters
        public const int ENC_CP949 = 13;  //#  CP949 DBCS, Korean characters
        public const int ENC_CP950 = 14;  //#  CP505 DBCS, Chinese Big5 characters
        public const int ENC_LATIN1 = 30;  //#  West Europe Latin-1, ISO 8859-15
        public const int ENC_LATIN2 = 31;  //#  Middle Europe Latin-2, ISO 8859-15
        public const int ENC_LATIN9 = 32;  //#  West Europe Latin-9, ISO 8859-15
        public const int ENC_WIN874 = 50;  //#  Windows Codepage 874, Thai characters
        public const int ENC_WIN1250 = 51;  //#  Windows Codepage 1251, Middle Europe
        public const int ENC_WIN1251 = 52;  //#  Windows Codepage 1251, Cyrillic
        public const int ENC_WIN1252 = 53;  //#  Windows Codepage 1252, West Europe
        public const int ENC_WIN1253 = 54;  //#  Windows Codepage 1253, Greek
        public const int ENC_WIN1254 = 55;  //#  Windows Codepage 1254, Turkish
        public const int ENC_WIN1255 = 56;  //#  Windows Codepage 1255, Hebrew
        public const int ENC_WIN1256 = 57;  //#  Windows Codepage 1256, Arabic
        public const int ENC_WIN1257 = 58;  //#  Windows Codepage 1257, Baltic
        public const int ENC_WIN1258 = 59;  //#  Windows Codepage 1258, Vietnamese

        // matching algorithms (block and nearest)
        public const int PATTERN_LEVEN_WORD = 1;
        public const int PATTERN_LEVENPLUS_WORD = 2;
        public const int PATTERN_LEVEN = 3;
        public const int PATTERN_JARO = 100;
        public const int PATTERN_JAROWINKLER = 101;
        public const int PATTERN_JAROWINKLER_WORD = 102;

        // match models
        public const int MATCH_SOURCE = 1;
        public const int MATCH_NEAREST = 2;
        public const int MATCH_BLOCK = 3;

        // result position types
        public const int POS_UTF8 = 1;
        public const int POS_OFFSET = 2;
        public const int POS_COUNT = 3;

        // constants
        public const int NO_POS = -1;
        public const int NO_VALUE = -2;

        // Delegate definitions for all library functions
        public delegate void printTextDelegate(IntPtr text, int len, int encoding);
        public static printTextDelegate printText;

        public delegate void getVersionDelegate(ref IntPtr verC, ref int lenC);
        public static getVersionDelegate getVersionFunc;

        public delegate void baseInitDelegate();
        public static baseInitDelegate baseInitFunc;

        public delegate void baseFinalizeDelegate();
        public static baseFinalizeDelegate baseFinalizeFunc;

        public delegate void libInitDelegate(IntPtr configPath, int len, int encoding);
        public static libInitDelegate libInitFunc;

        public delegate void libFinalizeDelegate();
        public static libFinalizeDelegate libFinalizeFunc;

        public delegate void logInitDelegate(IntPtr logChannel, int len);
        public static logInitDelegate logInitFunc;

        public delegate void deletePtrDelegate(IntPtr ptr);
        public static deletePtrDelegate deletePtrFunc;

        public delegate void getErrMsgDelegate(ref IntPtr errC, ref int lenC);
        public static getErrMsgDelegate getErrMsgFunc;

        public delegate void getErrExpDelegate(ref IntPtr errC, ref int lenC);
        public static getErrExpDelegate getErrExpFunc;

        public delegate int getErrPosDelegate();
        public static getErrPosDelegate getErrPosFunc;

        public delegate void clearErrDelegate();
        public static clearErrDelegate clearErrFunc;

        public delegate void setEncodingDelegate(int encoding);
        public static setEncodingDelegate setEncodingFunc;

        public delegate int newCodeObjDelegate(IntPtr source, int len);
        public static newCodeObjDelegate newCodeObjFunc;

        public delegate int newNearestObjDelegate(IntPtr source, int len, double dist);
        public static newNearestObjDelegate newNearestObjFunc;

        public delegate int newBlockObjDelegate(IntPtr source, int len, double dist);
        public static newBlockObjDelegate newBlockObjFunc;

        public delegate void deleteObjDelegate(int id);
        public static deleteObjDelegate deleteObjFunc;

        public delegate bool matchDelegate(int id, IntPtr text, int len);
        public static matchDelegate matchFunc;

        public delegate bool matchFileDelegate(int id, IntPtr file, int len, int fileEncoding);
        public static matchFileDelegate matchFileFunc;

        public delegate bool replaceDelegate(int id, IntPtr text, int len, IntPtr replace, int repLen);
        public static replaceDelegate replaceFunc;

        public delegate bool replaceFileDelegate(int id, IntPtr fileIn, int lenIn, IntPtr fileOut, int lenOut, IntPtr replace, int lenRep, int fileEncoding);
        public static replaceFileDelegate replaceFileFunc;

        public delegate bool splitDelegate(int id, IntPtr text, int len);
        public static splitDelegate splitFunc;

        public delegate bool splitFileDelegate(int id, IntPtr fileIn, int lenIn, IntPtr fileOut, int lenOut, int fileEncoding);
        public static splitFileDelegate splitFileFunc;

        public delegate bool searchFilesDelegate(int id, IntPtr path, int lenPath, IntPtr dirExtension, int lenExt, bool recursive);
        public static searchFilesDelegate searchFilesFunc;

        public delegate int getResultSizeDelegate(int id);
        public static getResultSizeDelegate getResultSizeFunc;

        public delegate bool getResultSpecsDelegate(int id, int index, ref int encoding, ref int matchType, ref int positionType);
        public static getResultSpecsDelegate getResultSpecsFunc;

        public delegate bool getResultStrDelegate(int id, int index, ref IntPtr outStr, ref int len);
        public static getResultStrDelegate getResultStrFunc;

        public delegate bool getResultFileNameDelegate(int id, int index, ref IntPtr outStr, ref int len);
        public static getResultFileNameDelegate getResultFileNameFunc;

        public delegate bool getResultTagDelegate(int id, int index, ref IntPtr outStr, ref int len);
        public static getResultTagDelegate getResultTagFunc;

        public delegate bool getResultPosDelegate(int id, int index, ref int posStart, ref int posEnd);
        public static getResultPosDelegate getResultPosFunc;

        public delegate bool getResultRatingDelegate(int id, int index, ref double rating);
        public static getResultRatingDelegate getResultRatingFunc;

        public delegate bool getResultJSONDelegate(int id, ref IntPtr outStr, ref int len);
        public static getResultJSONDelegate getResultJSONFunc;

        public delegate bool getResultCSVDelegate(int id, ref IntPtr outStr, ref int len);
        public static getResultCSVDelegate getResultCSVFunc;

        public delegate bool getResultHRFDelegate(int id, ref IntPtr outStr, ref int len);
        public static getResultHRFDelegate getResultHRFFunc;

        public delegate bool configSetLocaleDelegate(IntPtr locale, int len);
        public static configSetLocaleDelegate configSetLocaleFunc;

        public delegate bool configToJSONDelegate(ref IntPtr outStr, ref int len);
        public static configToJSONDelegate configToJSONFunc;

        public delegate bool configToJSONValDelegate(ref IntPtr outStr, ref int len, IntPtr names, int namesLen);
        public static configToJSONValDelegate configToJSONValFunc;

        public delegate bool configFromJSONDelegate(IntPtr textJSON, int len);
        public static configFromJSONDelegate configFromJSONFunc;

        public delegate bool configStoreDelegate(IntPtr file, int len);
        public static configStoreDelegate configStoreFunc;

        public delegate bool configRestoreDelegate(IntPtr file, int len);
        public static configRestoreDelegate configRestoreFunc;

        public delegate bool templateSetDelegate(IntPtr name, int nameLen, IntPtr source, int sourceLen);
        public static templateSetDelegate templateSetFunc;

        public delegate bool templateClearDelegate(IntPtr name, int nameLen);
        public static templateClearDelegate templateClearFunc;

        public delegate void templateResetDelegate();
        public static templateResetDelegate templateResetFunc;

        public delegate bool templateGetDelegate(IntPtr name, int nameLen, ref IntPtr outStr, ref int len);
        public static templateGetDelegate templateGetFunc;

        public delegate bool templateStoreDelegate(IntPtr file, int len);
        public static templateStoreDelegate templateStoreFunc;

        public delegate bool templateRestoreDelegate(IntPtr file, int len);
        public static templateRestoreDelegate templateRestoreFunc;

        public delegate bool templatesToJSONDelegate(ref IntPtr outStr, ref int len);
        public static templatesToJSONDelegate templatesToJSONFunc;

        public delegate bool templatesToJSONValDelegate(ref IntPtr outStr, ref int len, IntPtr name, int nameLen);
        public static templatesToJSONValDelegate templatesToJSONValFunc;

        public delegate bool templatesFromJSONDelegate(IntPtr json, int len);
        public static templatesFromJSONDelegate templatesFromJSONFunc;

        // baseInit method
        public static void baseInit(string libPath = "")
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            {
                if (libPath == "")
                {
                    libPath = "/usr/local/lib/libscripal.so";
                }
                scripalLib = NativeLibrary.Load(libPath);
            }
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                if (libPath == "")
                {
                    libPath = Environment.CurrentDirectory;
                }
                // Add DLL directory equivalent not needed as we load full path below
                scripalLib = NativeLibrary.Load(System.IO.Path.Combine(libPath, "scripal.dll"));
            }

            // Load all function delegates from the native library
            printText = Marshal.GetDelegateForFunctionPointer<printTextDelegate>(NativeLibrary.GetExport(scripalLib, "printText"));
            getVersionFunc = Marshal.GetDelegateForFunctionPointer<getVersionDelegate>(NativeLibrary.GetExport(scripalLib, "getVersion"));
            baseInitFunc = Marshal.GetDelegateForFunctionPointer<baseInitDelegate>(NativeLibrary.GetExport(scripalLib, "baseInit"));
            baseFinalizeFunc = Marshal.GetDelegateForFunctionPointer<baseFinalizeDelegate>(NativeLibrary.GetExport(scripalLib, "baseFinalize"));
            libInitFunc = Marshal.GetDelegateForFunctionPointer<libInitDelegate>(NativeLibrary.GetExport(scripalLib, "libInit"));
            libFinalizeFunc = Marshal.GetDelegateForFunctionPointer<libFinalizeDelegate>(NativeLibrary.GetExport(scripalLib, "libFinalize"));
            logInitFunc = Marshal.GetDelegateForFunctionPointer<logInitDelegate>(NativeLibrary.GetExport(scripalLib, "logInit"));
            deletePtrFunc = Marshal.GetDelegateForFunctionPointer<deletePtrDelegate>(NativeLibrary.GetExport(scripalLib, "deletePtr"));
            getErrMsgFunc = Marshal.GetDelegateForFunctionPointer<getErrMsgDelegate>(NativeLibrary.GetExport(scripalLib, "getErrMsg"));
            getErrExpFunc = Marshal.GetDelegateForFunctionPointer<getErrExpDelegate>(NativeLibrary.GetExport(scripalLib, "getErrExp"));
            getErrPosFunc = Marshal.GetDelegateForFunctionPointer<getErrPosDelegate>(NativeLibrary.GetExport(scripalLib, "getErrPos"));
            clearErrFunc = Marshal.GetDelegateForFunctionPointer<clearErrDelegate>(NativeLibrary.GetExport(scripalLib, "clearErr"));
            setEncodingFunc = Marshal.GetDelegateForFunctionPointer<setEncodingDelegate>(NativeLibrary.GetExport(scripalLib, "setEncoding"));
            newCodeObjFunc = Marshal.GetDelegateForFunctionPointer<newCodeObjDelegate>(NativeLibrary.GetExport(scripalLib, "newCodeObj"));
            newNearestObjFunc = Marshal.GetDelegateForFunctionPointer<newNearestObjDelegate>(NativeLibrary.GetExport(scripalLib, "newNearestObj"));
            newBlockObjFunc = Marshal.GetDelegateForFunctionPointer<newBlockObjDelegate>(NativeLibrary.GetExport(scripalLib, "newBlockObj"));
            deleteObjFunc = Marshal.GetDelegateForFunctionPointer<deleteObjDelegate>(NativeLibrary.GetExport(scripalLib, "deleteObj"));
            matchFunc = Marshal.GetDelegateForFunctionPointer<matchDelegate>(NativeLibrary.GetExport(scripalLib, "match"));
            matchFileFunc = Marshal.GetDelegateForFunctionPointer<matchFileDelegate>(NativeLibrary.GetExport(scripalLib, "matchFile"));
            replaceFunc = Marshal.GetDelegateForFunctionPointer<replaceDelegate>(NativeLibrary.GetExport(scripalLib, "replace"));
            replaceFileFunc = Marshal.GetDelegateForFunctionPointer<replaceFileDelegate>(NativeLibrary.GetExport(scripalLib, "replaceFile"));
            splitFunc = Marshal.GetDelegateForFunctionPointer<splitDelegate>(NativeLibrary.GetExport(scripalLib, "split"));
            splitFileFunc = Marshal.GetDelegateForFunctionPointer<splitFileDelegate>(NativeLibrary.GetExport(scripalLib, "splitFile"));
            searchFilesFunc = Marshal.GetDelegateForFunctionPointer<searchFilesDelegate>(NativeLibrary.GetExport(scripalLib, "searchFiles"));
            getResultSizeFunc = Marshal.GetDelegateForFunctionPointer<getResultSizeDelegate>(NativeLibrary.GetExport(scripalLib, "getResultSize"));
            getResultSpecsFunc = Marshal.GetDelegateForFunctionPointer<getResultSpecsDelegate>(NativeLibrary.GetExport(scripalLib, "getResultSpecs"));
            getResultStrFunc = Marshal.GetDelegateForFunctionPointer<getResultStrDelegate>(NativeLibrary.GetExport(scripalLib, "getResultStr"));
            getResultFileNameFunc = Marshal.GetDelegateForFunctionPointer<getResultFileNameDelegate>(NativeLibrary.GetExport(scripalLib, "getResultFileName"));
            getResultTagFunc = Marshal.GetDelegateForFunctionPointer<getResultTagDelegate>(NativeLibrary.GetExport(scripalLib, "getResultTag"));
            getResultPosFunc = Marshal.GetDelegateForFunctionPointer<getResultPosDelegate>(NativeLibrary.GetExport(scripalLib, "getResultPos"));
            getResultRatingFunc = Marshal.GetDelegateForFunctionPointer<getResultRatingDelegate>(NativeLibrary.GetExport(scripalLib, "getResultRating"));
            getResultJSONFunc = Marshal.GetDelegateForFunctionPointer<getResultJSONDelegate>(NativeLibrary.GetExport(scripalLib, "getResultJSON"));
            getResultCSVFunc = Marshal.GetDelegateForFunctionPointer<getResultCSVDelegate>(NativeLibrary.GetExport(scripalLib, "getResultCSV"));
            getResultHRFFunc = Marshal.GetDelegateForFunctionPointer<getResultHRFDelegate>(NativeLibrary.GetExport(scripalLib, "getResultHRF"));
            configSetLocaleFunc = Marshal.GetDelegateForFunctionPointer<configSetLocaleDelegate>(NativeLibrary.GetExport(scripalLib, "configSetLocale"));
            configToJSONFunc = Marshal.GetDelegateForFunctionPointer<configToJSONDelegate>(NativeLibrary.GetExport(scripalLib, "configToJSON"));
            configToJSONValFunc = Marshal.GetDelegateForFunctionPointer<configToJSONValDelegate>(NativeLibrary.GetExport(scripalLib, "configToJSONVal"));
            configFromJSONFunc = Marshal.GetDelegateForFunctionPointer<configFromJSONDelegate>(NativeLibrary.GetExport(scripalLib, "configFromJSON"));
            configStoreFunc = Marshal.GetDelegateForFunctionPointer<configStoreDelegate>(NativeLibrary.GetExport(scripalLib, "configStore"));
            configRestoreFunc = Marshal.GetDelegateForFunctionPointer<configRestoreDelegate>(NativeLibrary.GetExport(scripalLib, "configRestore"));
            templateSetFunc = Marshal.GetDelegateForFunctionPointer<templateSetDelegate>(NativeLibrary.GetExport(scripalLib, "templateSet"));
            templateClearFunc = Marshal.GetDelegateForFunctionPointer<templateClearDelegate>(NativeLibrary.GetExport(scripalLib, "templateClear"));
            templateResetFunc = Marshal.GetDelegateForFunctionPointer<templateResetDelegate>(NativeLibrary.GetExport(scripalLib, "templateReset"));
            templateGetFunc = Marshal.GetDelegateForFunctionPointer<templateGetDelegate>(NativeLibrary.GetExport(scripalLib, "templateGet"));
            templateStoreFunc = Marshal.GetDelegateForFunctionPointer<templateStoreDelegate>(NativeLibrary.GetExport(scripalLib, "templateStore"));
            templateRestoreFunc = Marshal.GetDelegateForFunctionPointer<templateRestoreDelegate>(NativeLibrary.GetExport(scripalLib, "templateRestore"));
            templatesToJSONFunc = Marshal.GetDelegateForFunctionPointer<templatesToJSONDelegate>(NativeLibrary.GetExport(scripalLib, "templatesToJSON"));
            templatesToJSONValFunc = Marshal.GetDelegateForFunctionPointer<templatesToJSONValDelegate>(NativeLibrary.GetExport(scripalLib, "templatesToJSONVal"));
            templatesFromJSONFunc = Marshal.GetDelegateForFunctionPointer<templatesFromJSONDelegate>(NativeLibrary.GetExport(scripalLib, "templatesFromJSON"));

            baseInitFunc();
            initialized = true;
            return;
        }

        public static string getVersion()
        {
            IntPtr verC = IntPtr.Zero;
            int lenC = 0;
            getVersionFunc(ref verC, ref lenC);
            string version = Marshal.PtrToStringUTF8(verC);
            deletePtrFunc(verC);
            return version;
        }

        public static void baseFinalize()
        {
            baseFinalizeFunc();
            return;
        }

        public static void libInit(string configPath, int encoding = ENC_UTF8)
        {
            if (!initialized)
            {
                baseInit();
            }
            clearErr();
            IntPtr configPathC = StringToUtf8(configPath, out int configPathLen);
            libInitFunc(configPathC, configPathLen, encoding);
            Marshal.FreeHGlobal(configPathC);
            config = new TFConfig();
            templates = new TFTemplate();
            if (getErrMsg() != "")
                throw new ArgumentException(getErrExp());
            if (!scripalInit)
            {
                scripalInit = true;
                templates = new TFTemplate();
            }
            return;
        }

        public static void libFinalize()
        {
            libFinalizeFunc();
            return;
        }

        public static void logInit(string logChannel)
        {
            clearErr();
            IntPtr logChannelC = StringToUtf8(logChannel, out int logChannelLen);
            logInitFunc(logChannelC, logChannelLen);
            Marshal.FreeHGlobal(logChannelC);
            if (getErrMsg() != "")
                throw new ArgumentException(getErrExp());
            return;
        }

        public static string getErrMsg()
        {
            IntPtr errC = IntPtr.Zero;
            int lenC = 0;
            getErrMsgFunc(ref errC, ref lenC);
            string errMsg = Marshal.PtrToStringUTF8(errC);
            deletePtrFunc(errC);
            return errMsg;
        }

        public static string getErrExp()
        {
            IntPtr errC = IntPtr.Zero;
            int lenC = 0;
            getErrExpFunc(ref errC, ref lenC);
            string errMsg = Marshal.PtrToStringUTF8(errC);
            deletePtrFunc(errC);
            return errMsg;
        }

        public static int getErrPos()
        {
            return getErrPosFunc();
        }

        public static void clearErr()
        {
            clearErrFunc();
            return;
        }

        public static void setEncoding(int encoding)
        {
            clearErr();
            setEncodingFunc(encoding);
            if (getErrMsg() != "")
                throw new ArgumentException(getErrExp());
            return;
        }

        public static void testError()
        {
            // Mimic testError by calling deletePtr with pointer value 1
            deletePtrFunc(new IntPtr(1));
            return;
        }

        // globals for libInit
        public static bool scripalInit = false;
        public static TFConfig config = null;
        public static TFTemplate templates = null;

        // Helper: converts string to UTF8 unmanaged memory and returns IntPtr and length.
        public static IntPtr StringToUtf8(string s, out int len)
        {
            byte[] bytes = Encoding.UTF8.GetBytes(s);
            len = bytes.Length;
            IntPtr ptr = Marshal.AllocHGlobal(len);
            Marshal.Copy(bytes, 0, ptr, len);
            return ptr;
        }
    }

    public class TFResults
    {
        public int id = -1;
        public int size()
        {
            return Scripal.getResultSizeFunc(id);
        }

        public int encoding = Scripal.ENC_UTF8;
        public int matchType = Scripal.MATCH_SOURCE;
        public int positionType = Scripal.POS_UTF8;
        public List<string> fileNames = new List<string>();
        public List<string> text = new List<string>();
        public List<List<int>> positions = new List<List<int>>();
        public List<double> ratings = new List<double>();
        public List<string> tags = new List<string>();
    }

    public class TFScripal
    {
        public int id = -1;
        public TFResults results = new TFResults();

        public TFScripal(string matchtype, string source, double dist = 0, int encoding = Scripal.ENC_UTF8)
        {
            Scripal.clearErr();
            IntPtr sourceC = Scripal.StringToUtf8(source, out int sourceLen);
            if (matchtype.ToLower() == "source")
            {
                id = Scripal.newCodeObjFunc(sourceC, sourceLen);
            }
            else if (matchtype.ToLower() == "nearest")
            {
                id = Scripal.newNearestObjFunc(sourceC, sourceLen, dist);
            }
            else if (matchtype.ToLower() == "block")
            {
                id = Scripal.newBlockObjFunc(sourceC, sourceLen, dist);
            }
            Marshal.FreeHGlobal(sourceC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            return;
        }

        ~TFScripal()
        {
            if (id != -1)
            {
                Scripal.deleteObjFunc(id);
            }
        }

        public void fillResults()
        {
            Scripal.clearErr();
            IntPtr resultC = IntPtr.Zero;
            int lenC = 0;
            int posStart = 0;
            int posEnd = 0;
            int enc = 0;
            int mType = 0;
            int pType = 0;
            double rating = 0;

            results.id = id;
            results.fileNames = new List<string>();
            results.text = new List<string>();
            results.positions = new List<List<int>>();
            results.ratings = new List<double>();
            results.tags = new List<string>();

            for (int index = 0; index < getResultSize(); index++)
            {
                bool resultBool = Scripal.getResultSpecsFunc(id, index, ref enc, ref mType, ref pType);
                if (Scripal.getErrMsg() != "")
                    throw new ArgumentException(Scripal.getErrExp());
                results.encoding = enc;
                results.matchType = mType;
                results.positionType = pType;

                resultBool = Scripal.getResultFileNameFunc(id, index, ref resultC, ref lenC);
                if (Scripal.getErrMsg() != "")
                    throw new ArgumentException(Scripal.getErrExp());
                results.fileNames.Add(Marshal.PtrToStringUTF8(resultC));

                resultBool = Scripal.getResultStrFunc(id, index, ref resultC, ref lenC);
                if (Scripal.getErrMsg() != "")
                    throw new ArgumentException(Scripal.getErrExp());
                results.text.Add(Marshal.PtrToStringUTF8(resultC));

                Scripal.getResultPosFunc(id, index, ref posStart, ref posEnd);
                if (Scripal.getErrMsg() != "")
                    throw new ArgumentException(Scripal.getErrExp());
                results.positions.Add(new List<int> { posStart, posEnd });

                Scripal.getResultRatingFunc(id, index, ref rating);
                if (Scripal.getErrMsg() != "")
                    throw new ArgumentException(Scripal.getErrExp());
                results.ratings.Add(rating);

                resultBool = Scripal.getResultTagFunc(id, index, ref resultC, ref lenC);
                if (Scripal.getErrMsg() != "")
                    throw new ArgumentException(Scripal.getErrExp());
                results.tags.Add(Marshal.PtrToStringUTF8(resultC));
            }
            return;
        }

        public bool match(string text)
        {
            Scripal.clearErr();
            IntPtr textC = Scripal.StringToUtf8(text, out int textLen);
            bool result = Scripal.matchFunc(id, textC, textLen);
            Marshal.FreeHGlobal(textC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            fillResults();
            return result;
        }

        public bool matchFile(string file, int fileEncoding = Scripal.ENC_UTF8)
        {
            Scripal.clearErr();
            IntPtr fileC = Scripal.StringToUtf8(file, out int fileLen);
            bool result = Scripal.matchFileFunc(id, fileC, fileLen, fileEncoding);
            Marshal.FreeHGlobal(fileC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            fillResults();
            return result;
        }

        public bool replace(string text, string replace)
        {
            Scripal.clearErr();
            IntPtr textC = Scripal.StringToUtf8(text, out int textLen);
            IntPtr replaceC = Scripal.StringToUtf8(replace, out int repLen);
            bool result = Scripal.replaceFunc(id, textC, textLen, replaceC, repLen);
            Marshal.FreeHGlobal(textC);
            Marshal.FreeHGlobal(replaceC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            fillResults();
            return result;
        }

        public bool replaceFile(string fileIn, string fileOut, string replace, int fileEncoding = Scripal.ENC_UTF8)
        {
            Scripal.clearErr();
            IntPtr fileInC = Scripal.StringToUtf8(fileIn, out int fileInLen);
            IntPtr fileOutC = Scripal.StringToUtf8(fileOut, out int fileOutLen);
            IntPtr replaceC = Scripal.StringToUtf8(replace, out int repLen);
            bool result = Scripal.replaceFileFunc(id, fileInC, fileInLen, fileOutC, fileOutLen, replaceC, repLen, fileEncoding);
            Marshal.FreeHGlobal(fileInC);
            Marshal.FreeHGlobal(fileOutC);
            Marshal.FreeHGlobal(replaceC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            fillResults();
            return result;
        }

        public bool split(string text)
        {
            Scripal.clearErr();
            IntPtr textC = Scripal.StringToUtf8(text, out int textLen);
            bool result = Scripal.splitFunc(id, textC, textLen);
            Marshal.FreeHGlobal(textC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            fillResults();
            return result;
        }

        public bool splitFile(string fileIn, string fileOut, int fileEncoding = Scripal.ENC_UTF8)
        {
            Scripal.clearErr();
            IntPtr fileInC = Scripal.StringToUtf8(fileIn, out int fileInLen);
            IntPtr fileOutC = Scripal.StringToUtf8(fileOut, out int fileOutLen);
            bool result = Scripal.splitFileFunc(id, fileInC, fileInLen, fileOutC, fileOutLen, fileEncoding);
            Marshal.FreeHGlobal(fileInC);
            Marshal.FreeHGlobal(fileOutC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            fillResults();
            return result;
        }

        public bool searchFiles(string path, string dirExtension, bool recursive = false)
        {
            Scripal.clearErr();
            IntPtr pathC = Scripal.StringToUtf8(path, out int pathLen);
            IntPtr dirExtensionC = Scripal.StringToUtf8(dirExtension, out int extLen);
            bool result = Scripal.searchFilesFunc(id, pathC, pathLen, dirExtensionC, extLen, recursive);
            Marshal.FreeHGlobal(pathC);
            Marshal.FreeHGlobal(dirExtensionC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            fillResults();
            return result;
        }

        public int getResultSize()
        {
            return Scripal.getResultSizeFunc(id);
        }

        public string getResultJSON()
        {
            Scripal.clearErr();
            IntPtr resultC = IntPtr.Zero;
            int lenC = 0;
            Scripal.getResultJSONFunc(id, ref resultC, ref lenC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            string result = Marshal.PtrToStringUTF8(resultC);
            Scripal.deletePtrFunc(resultC);
            return result;
        }

        public string getResultCSV()
        {
            Scripal.clearErr();
            IntPtr resultC = IntPtr.Zero;
            int lenC = 0;
            Scripal.getResultCSVFunc(id, ref resultC, ref lenC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            string result = Marshal.PtrToStringUTF8(resultC);
            Scripal.deletePtrFunc(resultC);
            return result;
        }

        public string getResultHRF()
        {
            Scripal.clearErr();
            IntPtr resultC = IntPtr.Zero;
            int lenC = 0;
            Scripal.getResultHRFFunc(id, ref resultC, ref lenC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            string result = Marshal.PtrToStringUTF8(resultC);
            Scripal.deletePtrFunc(resultC);
            return result;
        }
    }

    public class TFConfig
    {
        public TFConfig()
        {
            return;
        }

        public bool setLocale(string locale)
        {
            Scripal.clearErr();
            IntPtr localeC = Scripal.StringToUtf8(locale, out int localeLen);
            bool result = Scripal.configSetLocaleFunc(localeC, localeLen);
            Marshal.FreeHGlobal(localeC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            return result;
        }

        public string toJSON()
        {
            Scripal.clearErr();
            IntPtr resultC = IntPtr.Zero;
            int lenC = 0;
            Scripal.configToJSONFunc(ref resultC, ref lenC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            string result = Marshal.PtrToStringUTF8(resultC);
            Scripal.deletePtrFunc(resultC);
            return result;
        }

        public string toJSONVal(string names)
        {
            Scripal.clearErr();
            IntPtr resultC = IntPtr.Zero;
            int lenC = 0;
            IntPtr namesC = Scripal.StringToUtf8(names, out int namesLen);
            Scripal.configToJSONValFunc(ref resultC, ref lenC, namesC, namesLen);
            Marshal.FreeHGlobal(namesC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            string result = Marshal.PtrToStringUTF8(resultC);
            Scripal.deletePtrFunc(resultC);
            return result;
        }

        public bool fromJSON(string textJSON)
        {
            Scripal.clearErr();
            IntPtr textJSONC = Scripal.StringToUtf8(textJSON, out int textJSONLen);
            bool result = Scripal.configFromJSONFunc(textJSONC, textJSONLen);
            Marshal.FreeHGlobal(textJSONC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            return result;
        }

        public bool store(string file)
        {
            Scripal.clearErr();
            IntPtr fileC = Scripal.StringToUtf8(file, out int fileLen);
            bool result = Scripal.configStoreFunc(fileC, fileLen);
            Marshal.FreeHGlobal(fileC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            return result;
        }

        public bool restore(string file)
        {
            Scripal.clearErr();
            IntPtr fileC = Scripal.StringToUtf8(file, out int fileLen);
            bool result = Scripal.configRestoreFunc(fileC, fileLen);
            Marshal.FreeHGlobal(fileC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            return result;
        }
    }

    public class TFTemplate
    {
        public bool set(string name, string source)
        {
            Scripal.clearErr();
            IntPtr nameC = Scripal.StringToUtf8(name, out int nameLen);
            IntPtr sourceC = Scripal.StringToUtf8(source, out int sourceLen);
            bool result = Scripal.templateSetFunc(nameC, nameLen, sourceC, sourceLen);
            Marshal.FreeHGlobal(nameC);
            Marshal.FreeHGlobal(sourceC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            return result;
        }

        public bool clear(string name)
        {
            Scripal.clearErr();
            IntPtr nameC = Scripal.StringToUtf8(name, out int nameLen);
            bool result = Scripal.templateClearFunc(nameC, nameLen);
            Marshal.FreeHGlobal(nameC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            return result;
        }

        public void reset()
        {
            Scripal.templateResetFunc();
            return;
        }

        public string get(string name)
        {
            Scripal.clearErr();
            IntPtr resultC = IntPtr.Zero;
            int lenC = 0;
            IntPtr nameC = Scripal.StringToUtf8(name, out int nameLen);
            bool resultBool = Scripal.templateGetFunc(nameC, nameLen, ref resultC, ref lenC);
            Marshal.FreeHGlobal(nameC);
            if (Scripal.getErrMsg() != "")
                throw new ArgumentException(Scripal.getErrExp());
            string result = Marshal.PtrToStringUTF8(resultC);
            Scripal.deletePtrFunc(resultC);
            return result;
        }

        //
        public string toJSON()
        {
            Scripal.clearErr(); 
            IntPtr resultC = IntPtr.Zero;
            int lenC = 0;
            bool resultBool = Scripal.templatesToJSONFunc(ref resultC, ref lenC);
            if (Scripal.getErrMsg() != "")
            {
                throw new ArgumentException(Scripal.getErrExp());
            }
            string result = Marshal.PtrToStringUTF8(resultC);
            Scripal.deletePtrFunc(resultC);
            return result;
        }
        public string ToJSONVal(string names)
        {
            Scripal.clearErr();
            IntPtr resultC = IntPtr.Zero;
            int lenC = 0;
            byte[] namesC = System.Text.Encoding.UTF8.GetBytes(names);
            Scripal.configToJSONValFunc(ref resultC, ref lenC, Marshal.AllocHGlobal(namesC.Length), namesC.Length);
            if (Scripal.getErrMsg() != "") throw new ArgumentException(Scripal.getErrExp());
            string result = Marshal.PtrToStringUTF8(resultC);
            Scripal.deletePtrFunc(resultC);
            return result;
        }
        public bool FromJSON(string textJSON)
        {
            Scripal.clearErr();
            byte[] textJSONC = System.Text.Encoding.UTF8.GetBytes(textJSON);
            IntPtr jsonPtr = Marshal.AllocHGlobal(textJSONC.Length);
            Marshal.Copy(textJSONC, 0, jsonPtr, textJSONC.Length);

            bool result = Scripal.configFromJSONFunc(jsonPtr, textJSONC.Length);
            Marshal.FreeHGlobal(jsonPtr);

            if (Scripal.getErrMsg() != "") throw new ArgumentException(Scripal.getErrExp());
            return result;
        }
        public bool Store(string file)
        {
            Scripal.clearErr();
            byte[] fileC = System.Text.Encoding.UTF8.GetBytes(file);
            bool result = Scripal.templateStoreFunc(Marshal.StringToHGlobalAnsi(file), fileC.Length);
            if (Scripal.getErrMsg() != "") throw new ArgumentException(Scripal.getErrExp());
            return result;
        }
        public bool Restore(string file)
        {
            Scripal.clearErr();
            byte[] fileC = System.Text.Encoding.UTF8.GetBytes(file);
            bool result = Scripal.templateRestoreFunc(Marshal.StringToHGlobalAnsi(file), fileC.Length);
            if (Scripal.getErrMsg() != "") throw new ArgumentException(Scripal.getErrExp());
            return result;
        }

        //
    }
}

// End of Scripal wrapper for C#
# Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
# License:  BSL 1.0
# Scripal wrapper for Python

import ctypes
import sys
import os

from ctypes import *

scripalLib = None
initialized = False

# encodings
ENC_DEFAULT  = 1
ENC_UTF8     = 2
ENC_UTF16L   = 3
ENC_UTF16B   = 4
ENC_UTF32L   = 5
ENC_UTF32B   = 6
ENC_ASCII    = 10  #  ASCII , Extended ASCII (up to 255)
ENC_CP932    = 11  #  CP932 DBCS, Japanese characters
ENC_CP936    = 12  #  CP93 6DBCS, simple Chinese characters
ENC_CP949    = 13  #  CP949 DBCS, Korean characters
ENC_CP950    = 14  #  CP505 DBCS, Chinese Big5 characters
ENC_LATIN1   = 30  #  West Europe Latin-1, ISO 8859-15
ENC_LATIN2   = 31  #  Middle Europe Latin-2, ISO 8859-15
ENC_LATIN9   = 32  #  West Europe Latin-9, ISO 8859-15
ENC_WIN874   = 50  #  Windows Codepage 874, Thai characters
ENC_WIN1250  = 51  #  Windows Codepage 1251, Middle Europe
ENC_WIN1251  = 52  #  Windows Codepage 1251, Cyrillic
ENC_WIN1252  = 53  #  Windows Codepage 1252, West Europe
ENC_WIN1253  = 54  #  Windows Codepage 1253, Greek
ENC_WIN1254  = 55  #  Windows Codepage 1254, Turkish
ENC_WIN1255  = 56  #  Windows Codepage 1255, Hebrew
ENC_WIN1256  = 57  #  Windows Codepage 1256, Arabic
ENC_WIN1257  = 58  #  Windows Codepage 1257, Baltic
ENC_WIN1258  = 59  #  Windows Codepage 1258, Vietnamese

# matching algorithms (block and nearest)
PATTERN_LEVEN_WORD = 1
PATTERN_LEVENPLUS_WORD = 2
PATTERN_LEVEN = 3
PATTERN_JARO = 100
PATTERN_JAROWINKLER = 101
PATTERN_JAROWINKLER_WORD = 102

# match models
MATCH_SOURCE      = 1
MATCH_NEAREST     = 2
MATCH_BLOCK       = 3

# result position types
POS_UTF8          = 1
POS_OFFSET        = 2
POS_COUNT         = 3

# constants
NO_POS = -1
NO_VALUE = -2

def baseInit(libPath = ""):
  global scripalLib
  global initialized

  if sys.platform == "linux":
    loadLib = True
    if libPath == "":
      libPath = "/usr/lib/scripal/libscripal.so"
    if not os.path.exists(libPath): 
      libPath = "/usr/lib64/scripal/libscripal.so"
    scripalLib = cdll.LoadLibrary(libPath)
      
  if sys.platform == "win32":
    if libPath == "":
      libPath = os.getcwd()
    os.add_dll_directory(libPath) 
    scripalLib = cdll.LoadLibrary("scripal.dll")

  scripalLib.printText.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
  scripalLib.printText.restype = None

  scripalLib.getVersion.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getVersion.restype = None

  scripalLib.baseInit.argtypes = []
  scripalLib.baseInit.restype = None

  scripalLib.baseFinalize.argtypes = []
  scripalLib.baseFinalize.restype = None

  scripalLib.libInit.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
  scripalLib.libInit.restype = None

  scripalLib.libFinalize.argtypes = []
  scripalLib.libFinalize.restype = None

  scripalLib.logInit.argtypes = [ctypes.c_char_p, ctypes.c_int]
  scripalLib.logInit.restype = None
 
  scripalLib.getLogBuf.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getLogBuf.restype = None

  scripalLib.clearLogBuf.argtypes = []
  scripalLib.clearLogBuf.restype = None

  scripalLib.deletePtr.argtypes = [ctypes.c_char_p]
  scripalLib.deletePtr.restype = None

  scripalLib.getErrMsg.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getErrMsg.restype = None

  scripalLib.getErrExp.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getErrExp.restype = None

  scripalLib.getErrPos.argtypes = []
  scripalLib.getErrPos.restype = ctypes.c_int

  scripalLib.clearErr.argtypes = []
  scripalLib.clearErr.restype = None

  scripalLib.setEncoding.argtypes = [ctypes.c_int]
  scripalLib.setEncoding.restype = None

  scripalLib.newCodeObj.argtypes = [ctypes.c_char_p, ctypes.c_int]
  scripalLib.newCodeObj.restype = ctypes.c_int

  scripalLib.newNearestObj.argtypes = [ctypes.c_char_p, ctypes.c_int, c_double]
  scripalLib.newNearestObj.restype = ctypes.c_int

  scripalLib.newBlockObj.argtypes = [ctypes.c_char_p, ctypes.c_int, c_double]
  scripalLib.newBlockObj.restype = ctypes.c_int

  scripalLib.deleteObj.argtypes = [ctypes.c_int]
  scripalLib.deleteObj.restype = None

  scripalLib.match.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int]
  scripalLib.match.restype = ctypes.c_bool

  scripalLib.matchFile.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
  scripalLib.matchFile.restype = ctypes.c_bool

  scripalLib.replace.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int]
  scripalLib.replace.restype = ctypes.c_bool

  scripalLib.replaceFile.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
  scripalLib.replaceFile.restype = ctypes.c_bool

  scripalLib.split.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int]
  scripalLib.split.restype = ctypes.c_bool

  scripalLib.splitFile.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
  scripalLib.splitFile.restype = ctypes.c_bool

  scripalLib.searchFiles.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_bool]
  scripalLib.searchFiles.restype = ctypes.c_bool

  scripalLib.getResultSize.argtypes = [ctypes.c_int]
  scripalLib.getResultSize.restype = ctypes.c_int

  scripalLib.getResultSpecs.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]
  scripalLib.getResultSpecs.restype = ctypes.c_bool

  scripalLib.getResultStr.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getResultStr.restype = ctypes.c_bool

  scripalLib.getResultFileName.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getResultFileName.restype = ctypes.c_bool

  scripalLib.getResultTag.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getResultTag.restype = ctypes.c_bool

  scripalLib.getResultPos.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]
  scripalLib.getResultPos.restype = ctypes.c_bool

  scripalLib.getResultRating.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_double)]
  scripalLib.getResultRating.restype = ctypes.c_bool

  scripalLib.getResultJSON.argtypes = [ctypes.c_int, ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getResultJSON.restype = ctypes.c_bool

  scripalLib.getResultCSV.argtypes = [ctypes.c_int, ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getResultCSV.restype = ctypes.c_bool

  scripalLib.getResultHRF.argtypes = [ctypes.c_int, ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.getResultHRF.restype = ctypes.c_bool

  scripalLib.configSetLocale.argtypes = [ctypes.c_char_p, ctypes.c_int]
  scripalLib.configSetLocale.restype = ctypes.c_bool

  scripalLib.configToJSON.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.configToJSON.restype = ctypes.c_bool

  scripalLib.configToJSONVal.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int), ctypes.c_char_p, ctypes.c_int]
  scripalLib.configToJSONVal.restype = ctypes.c_bool

  scripalLib.configFromJSON.argtypes = [ctypes.c_char_p, ctypes.c_int]
  scripalLib.configFromJSON.restype = ctypes.c_bool

  scripalLib.configStore.argtypes = [ctypes.c_char_p, ctypes.c_int]
  scripalLib.configStore.restype = ctypes.c_bool

  scripalLib.configRestore.argtypes = [ctypes.c_char_p, ctypes.c_int]
  scripalLib.configRestore.restype = ctypes.c_bool

  scripalLib.templateSet.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int]
  scripalLib.templateSet.restype = ctypes.c_bool

  scripalLib.templateClear.argtypes = [ctypes.c_char_p, ctypes.c_int]
  scripalLib.templateClear.restype = ctypes.c_bool

  scripalLib.templateReset.argtypes = []
  scripalLib.templateReset.restype = None

  scripalLib.templateGet.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.templateGet.restype = ctypes.c_bool

  scripalLib.templateStore.argtypes = [ctypes.c_char_p, ctypes.c_int]
  scripalLib.templateStore.restype = ctypes.c_bool

  scripalLib.templateRestore.argtypes = [ctypes.c_char_p, ctypes.c_int]
  scripalLib.templateRestore.restype = ctypes.c_bool

  scripalLib.templatesToJSON.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
  scripalLib.templatesToJSON.restype = ctypes.c_bool

  scripalLib.templatesToJSONVal.argtypes= [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int), ctypes.c_char_p, ctypes.c_int]
  scripalLib.templatesToJSONVal.restype = ctypes.c_bool

  scripalLib.templatesFromJSON.argtypes= [ctypes.c_char_p, ctypes.c_int]
  scripalLib.templatesFromJSON.restype = ctypes.c_bool

  scripalLib.baseInit()
  initialized = True;
  return

def getVersion():
  verC= c_void_p()
  lenC = c_int(0)
  scripalLib.getVersion(byref(verC), byref(lenC))
  version = (c_char_p(verC.value).value).decode("utf-8")
  scripalLib.deletePtr(c_char_p(verC.value))
  return version

def baseFinalize():
  scripalLib.baseFinalize()
  return

def libInit(configPath, encoding = ENC_UTF8):
  global scripalInit
  global config
  global templates
  global initialized

  if not initialized:
    baseInit()
  clearErr()
  configPathC = configPath.encode("utf-8")
  scripalLib.libInit(c_char_p(configPathC), c_int(len(configPathC)), c_int(encoding))
  config = TFConfig(config)
  templates = TFTemplate()
  if getErrMsg() != "": raise ValueError(getErrExp())

  if not scripalInit:
    scripalInit = True
    templates = TFTemplate() 
  return

def libFinalize():
  scripalLib.libFinalize()
  return

def logInit(logChannel):
  clearErr()
  logChannelC = logChannel.encode("utf-8")
  scripalLib.logInit(c_char_p(logChannelC), c_int(len(logChannelC)))
  if getErrMsg() != "": raise ValueError(getErrExp())
  return

def getLogBuf():
  bufC= c_void_p()
  lenC = c_int(0)
  scripalLib.getLogBuf(byref(bufC), byref(lenC))
  bufMsg = (c_char_p(bufC.value).value).decode("utf-8")
  scripalLib.deletePtr(c_char_p(bufC.value))
  return bufMsg

def clearLogBuf():
  scripalLib.clearLogBuf()
  return

def getErrMsg():
  errC= c_void_p()
  lenC = c_int(0)
  scripalLib.getErrMsg(byref(errC), byref(lenC))
  errMsg = (c_char_p(errC.value).value).decode("utf-8")
  scripalLib.deletePtr(c_char_p(errC.value))
  return errMsg

def getErrExp():
  errC= c_void_p()
  lenC = c_int(0)
  scripalLib.getErrExp(byref(errC), byref(lenC))
  errMsg = (c_char_p(errC.value).value).decode("utf-8")
  scripalLib.deletePtr(c_char_p(errC.value))
  return errMsg

def getErrPos():
  return int(scripalLib.getErrPos())

def clearErr():
  scripalLib.clearErr()
  return 

def setEncoding(encoding):
  clearErr()
  scripalLib.setEncoding(c_int(encoding))
  if getErrMsg() != "": raise ValueError(getErrExp())  
  return 

def testError():
  scripalLib.deletePtr(c_char_p(1))
  return

class TFResults:
  id = c_int(-1)
  def size(self):
    return(scripalLib.getResultSize(self.id))

  encoding = ENC_UTF8                                
  matchType = MATCH_SOURCE
  positionType = POS_UTF8
  fileNames = []
  text = []
  positions = []
  ratings = []                       
  tags = []                          

class TFScripal:
  id = c_int(-1)
  results = TFResults()

  def __init__(self, matchtype, source, dist = 0): 
    clearErr()
    sourceC = source.encode("utf-8")
    if matchtype.lower() == "source":
      self.id = int(scripalLib.newCodeObj(c_char_p(sourceC), c_int(len(sourceC))))
    elif matchtype.lower() == "nearest":
      self.id = int(scripalLib.newNearestObj(c_char_p(sourceC), c_int(len(sourceC)), c_double(dist)))
    elif matchtype.lower() == "block":
      self.id = int(scripalLib.newBlockObj(c_char_p(sourceC), c_int(len(sourceC)), c_double(dist)))

    if getErrMsg() != "": raise ValueError(getErrExp())
    return
     
  def __del__(self):
    if self.id != -1:
      scripalLib.deleteObj(self.id)
    return

  def fillResults(self):
    clearErr()
    resultC= c_void_p()
    lenC = c_int(0)
    posStart = c_int(0)
    posEnd = c_int(0)
    encoding = c_int(0)
    matchType = c_int(0)
    positionType = c_int(0)
    rating = c_double(0)

    self.results.id = self.id
    self.results.fileNames = []
    self.results.text = []
    self.results.positions = []
    self.results.ratings = []
    self.results.tags = []

    for index in range(0, self.getResultSize()):
      resultBool = scripalLib.getResultSpecs(c_int(self.id), c_int(index), byref(encoding), byref(matchType), byref(positionType))
      if getErrMsg() != "": raise ValueError(getErrExp())
      self.results.encoding = encoding.value
      self.results.matchType = matchType.value
      self.results.positionType = positionType.value

      resultBool = scripalLib.getResultFileName(c_int(self.id), c_int(index), byref(resultC), byref(lenC))
      if getErrMsg() != "": raise ValueError(getErrExp())
      self.results.fileNames.append((c_char_p(resultC.value).value).decode("utf-8"))

      resultBool = scripalLib.getResultStr(c_int(self.id), c_int(index), byref(resultC), byref(lenC))
      if getErrMsg() != "": raise ValueError(getErrExp())
      self.results.text.append((c_char_p(resultC.value).value).decode("utf-8"))

      scripalLib.getResultPos(c_int(self.id), c_int(index), byref(posStart), byref(posEnd) )      
      if getErrMsg() != "": raise ValueError(getErrExp())
      self.results.positions.append([posStart.value, posEnd.value])
    
      scripalLib.getResultRating(c_int(self.id), c_int(index), byref(rating))
      if getErrMsg() != "": raise ValueError(getErrExp())
      self.results.ratings.append(rating.value)

      resultBool = scripalLib.getResultTag(c_int(self.id), c_int(index), byref(resultC), byref(lenC))
      if getErrMsg() != "": raise ValueError(getErrExp())
      self.results.tags.append((c_char_p(resultC.value).value).decode("utf-8"))
    return

  def match(self, text):
    clearErr()
    textC = text.encode("utf-8")
    result = bool(scripalLib.match(c_int(self.id), c_char_p(textC), c_int(len(textC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    self.fillResults()
    return result

  def matchFile(self, file, fileEncoding = ENC_UTF8):
    clearErr()
    fileC = file.encode("utf-8")
    result = bool(scripalLib.matchFile(c_int(self.id), c_char_p(fileC), c_int(len(fileC)), c_int(fileEncoding)))
    if getErrMsg() != "": raise ValueError(getErrExp())
    self.fillResults()
    return result

  def replace(self, text, replace):
    clearErr()
    textC = text.encode("utf-8")
    replaceC = replace.encode("utf-8")
    result = bool(scripalLib.replace(c_int(self.id), c_char_p(textC), c_int(len(textC)), c_char_p(replaceC), c_int(len(replaceC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    self.fillResults()
    return result

  def replaceFile(self, fileIn, fileOut, replace, fileEncoding = ENC_UTF8):
    clearErr()
    fileInC = fileIn.encode("utf-8")
    fileOutC = fileOut.encode("utf-8")
    replaceC = replace.encode("utf-8")
    result = bool(scripalLib.replaceFile(c_int(self.id), c_char_p(fileInC), c_int(len(fileInC)), c_char_p(fileOutC), c_int(len(fileOutC)), 
                                             c_char_p(replaceC), c_int(len(replaceC)), c_int(fileEncoding)))
    if getErrMsg() != "": raise ValueError(getErrExp())
    self.fillResults()
    return result

  def split(self, text):
    clearErr()
    textC = text.encode("utf-8")
    result = bool(scripalLib.split(c_int(self.id), c_char_p(textC), c_int(len(textC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    self.fillResults()
    return result

  def splitFile(self, fileIn, fileOut, fileEncoding = ENC_UTF8):
    clearErr()
    fileInC = fileIn.encode("utf-8")
    fileOutC = fileOut.encode("utf-8")
    result = bool(scripalLib.splitFile(c_int(self.id), c_char_p(fileInC), c_int(len(fileInC)), c_char_p(fileOutC), c_int(len(fileOutC)), c_int(fileEncoding)))
    if getErrMsg() != "": raise ValueError(getErrExp())
    self.fillResults()
    return result

  def searchFiles(self, path, dirExtension, recursive = False):
    clearErr()
    pathC = path.encode("utf-8")
    dirExtensionC = dirExtension.encode("utf-8")
    result = bool(scripalLib.searchFiles(c_int(self.id), c_char_p(pathC), c_int(len(pathC)), c_char_p(dirExtensionC),
                                    c_int(len(dirExtensionC)), c_bool(recursive)))
    if getErrMsg() != "": raise ValueError(getErrExp())
    self.fillResults()
    return result

  def getResultSize(self):
    return int(scripalLib.getResultSize(c_int(self.id)))
     
  def getResultJSON(self):
    clearErr()
    resultC= c_void_p()
    lenC = c_int(0)
    scripalLib.getResultJSON(c_int(self.id), byref(resultC), byref(lenC))
    if getErrMsg() != "": raise ValueError(getErrExp())
    result = (c_char_p(resultC.value).value).decode("utf-8")
    scripalLib.deletePtr(c_char_p(resultC.value))
    return result

  def getResultCSV(self):
    clearErr()
    resultC= c_void_p()
    lenC = c_int(0)
    scripalLib.getResultCSV(c_int(self.id), byref(resultC), byref(lenC))
    if getErrMsg() != "": raise ValueError(getErrExp())
    result = (c_char_p(resultC.value).value).decode("utf-8")
    scripalLib.deletePtr(c_char_p(resultC.value))
    return result

  def getResultHRF(self):
    clearErr()
    resultC= c_void_p()
    lenC = c_int(0)
    scripalLib.getResultHRF(c_int(self.id), byref(resultC), byref(lenC))
    if getErrMsg() != "": raise ValueError(getErrExp())
    result = (c_char_p(resultC.value).value).decode("utf-8")
    scripalLib.deletePtr(c_char_p(resultC.value))
    return result

class TFConfig:
  def __init__(self, config):
    return

  def setLocale(self, locale):
    clearErr()
    localeC = locale.encode("utf-8")
    result = bool(scripalLib.configSetLocale(c_char_p(localeC), c_int(len(localeC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    return result  

  def toJSON(self):
    clearErr()
    resultC= c_void_p()
    lenC = c_int(0)
    scripalLib.configToJSON(byref(resultC), byref(lenC))
    if getErrMsg() != "": raise ValueError(getErrExp())
    result = (c_char_p(resultC.value).value).decode("utf-8")
    scripalLib.deletePtr(c_char_p(resultC.value))
    return result  

  def toJSONVal(self, names):
    clearErr()
    resultC= c_void_p()
    lenC = c_int(0)
    namesC = names.encode("utf-8")
    scripalLib.configToJSONVal(byref(resultC), byref(lenC), c_char_p(namesC), c_int(len(namesC)))
    if getErrMsg() != "": raise ValueError(getErrExp())
    result = (c_char_p(resultC.value).value).decode("utf-8")
    scripalLib.deletePtr(c_char_p(resultC.value))
    return result  

  def fromJSON(self, textJSON):
    clearErr()
    textJSONC = textJSON.encode("utf-8")
    result = bool(scripalLib.configFromJSON(c_char_p(textJSONC), c_int(len(textJSONC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    return result  

  def store(self, file):
    clearErr()
    fileC = file.encode("utf-8")
    result = bool(scripalLib.configStore(c_char_p(fileC), c_int(len(fileC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    return result  

  def restore(self, file):
    clearErr()
    fileC = file.encode("utf-8")
    result = bool(scripalLib.configRestore(c_char_p(fileC), c_int(len(fileC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    return result  

class TFTemplate:
  def set(self, name, source):
    clearErr()
    nameC = name.encode("utf-8")
    sourceC = source.encode("utf-8")
    result = bool(scripalLib.templateSet(c_char_p(nameC), c_int(len(nameC)), c_char_p(sourceC), c_int(len(sourceC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    return result 
 
  def clear(self, name):
    clearErr()
    nameC = name.encode("utf-8")
    result = bool(scripalLib.templateClear(c_char_p(nameC), c_int(len(nameC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    return result 

  def reset(self):
    scripalLib.templateReset()
    return  

  def get(self, name):
    clearErr()
    resultC= c_void_p()
    lenC = c_int(0)
    nameC = name.encode("utf-8")
    resultBool = scripalLib.templateGet(c_char_p(nameC), c_int(len(nameC)), byref(resultC), byref(lenC))
    if getErrMsg() != "": raise ValueError(getErrExp())
    result = (c_char_p(resultC.value).value).decode("utf-8")
    scripalLib.deletePtr(c_char_p(resultC.value))
    return result 

  def toJSON(self):
    clearErr()
    resultC= c_void_p()
    lenC = c_int(0)
    resultBool = bool(scripalLib.templatesToJSON(byref(resultC), byref(lenC)))
    if getErrMsg() != "": raise ValueError(getErrExp())
    result = (c_char_p(resultC.value).value).decode("utf-8")
    scripalLib.deletePtr(c_char_p(resultC.value))
    return result  

  def toJSONVal(self, names):
    clearErr()
    resultC= c_void_p()
    lenC = c_int(0)
    namesC = names.encode("utf-8")
    resultBool = bool(scripalLib.templatesToJSONVal(byref(resultC), byref(lenC), c_char_p(namesC), c_int(len(namesC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    result = (c_char_p(resultC.value).value).decode("utf-8")
    scripalLib.deletePtr(c_char_p(resultC.value))
    return result  

  def fromJSON(self, textJSON):
    clearErr()
    textJSONC = textJSON.encode("utf-8")
    result = bool(scripalLib.templatesFromSON(c_char_p(textJSONC), c_int(len(textJSONC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    return result  

  def store(self, file):
    clearErr()
    fileC = file.encode("utf-8")
    result = bool(scripalLib.templateStore(c_char_p(fileC), c_int(len(fileC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    return result  

  def restore(self, file):
    clearErr()
    fileC = file.encode("utf-8")
    result = bool(scripalLib.templateRestore(c_char_p(fileC), c_int(len(fileC))))
    if getErrMsg() != "": raise ValueError(getErrExp())
    return result  

# globals
scripalInit = False
config = None
templates = None


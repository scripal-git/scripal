// ***************************************************************************
// general functions for UNICODE text handling  
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef TEXT_HPP_INCLUDED
#define TEXT_HPP_INCLUDED

#include "definitions.hpp"
#include "encoding.hpp"

// export under namespace Scripal
namespace Scripal {

// **********************************************
// constants

#define fmtStr(aNum) to_string(aNum).data() 

enum TFConvertTypes : uint8_t { CONVERT_LOWER, CONVERT_UPPER, CONVERT__MAX };

enum TFComparison : uint8_t { CMP_LSS, CMP_LSS_EQU, CMP_EQU, CMP_UQU, CMP_GTR, CMP_GTR_EQU, \
                               CMP_SMALL, CMP_SMALL_SAME , CMP_SAME,  CMP_NOT_SAME, CMP_BIG,  CMP_BIG_SAME, CMP_COPY, CMP_NULL, CMP__MAX };

static std::map<char32_t, char32_t> digits = {
{U'𞥐',U'0'}, {U'𞥑',U'1'}, {U'𞥒', U'2'},{U'𞥓', U'3'},{U'𞥔', U'4'},{U'𞥕',U'5'},{U'𞥖',U'6'},{U'𞥗',U'7'},{U'𞥘',U'8'},{U'𞥙',U'9'},
{U'𑜰',U'0'}, {U'𑜱',U'1'}, {U'𑜲',U'2'}, {U'𑜳',U'3'}, {U'𑜴',U'4'}, {U'𑜵',U'5'}, {U'𑜶',U'6'}, {U'𑜷',U'7'}, {U'𑜸',U'8'}, {U'𑜹',U'9'},
{U'٠',U'0'}, {U'١',U'1'}, {U'٢',U'2'}, {U'٣',U'3'}, {U'٤',U'4'}, {U'٥',U'5'}, {U'٦',U'6'}, {U'٧',U'7'}, {U'٨',U'8'}, {U'٩',U'9'},
{U'᭐',U'0'}, {U'᭑',U'1'}, {U'᭒',U'2'}, {U'᭓',U'3'}, {U'᭔',U'4'}, {U'᭕',U'5'}, {U'᭖',U'6'}, {U'᭗',U'7'}, {U'᭘',U'8'}, {U'᭙',U'9'},
{U'০',U'0'}, {U'১',U'1'}, {U'২',U'2'}, {U'৩',U'3'}, {U'৪',U'4'}, {U'৫',U'5'}, {U'৬',U'6'}, {U'৭',U'7'}, {U'৮',U'8'}, {U'৯',U'9'},
{U'𑱐',U'0'}, {U'𑱑',U'1'}, {U'𑱒',U'2'}, {U'𑱓',U'3'}, {U'𑱔',U'4'}, {U'𑱕',U'5'}, {U'𑱖',U'6'}, {U'𑱗',U'7'}, {U'𑱘',U'8'}, {U'𑱙',U'9'},
{U'𑄶',U'0'}, {U'𑄷',U'1'}, {U'𑄸',U'2'}, {U'𑄹',U'3'}, {U'𑄺',U'4'}, {U'𑄻',U'5'}, {U'𑄼',U'6'}, {U'𑄽',U'7'}, {U'𑄾',U'8'}, {U'𑄿',U'9'},
{U'꩐',U'0'}, {U'꩑',U'1'}, {U'꩒',U'2'}, {U'꩓',U'3'}, {U'꩔',U'4'}, {U'꩕',U'5'}, {U'꩖',U'6'}, {U'꩗',U'7'}, {U'꩘',U'8'}, {U'꩙',U'9'},
{U'〇',U'0'}, {U'一',U'1'}, {U'二',U'2'}, {U'三',U'3'}, {U'四',U'4'}, {U'五',U'5'}, {U'六',U'6'}, {U'七',U'7'}, {U'八',U'8'}, {U'九',U'9'},
{U'零',U'0'}, {U'壹',U'1'}, {U'貳',U'2'}, {U'參',U'3'}, {U'肆',U'4'}, {U'伍',U'5'}, {U'陸',U'6'}, {U'柒',U'7'}, {U'捌',U'8'}, {U'玖',U'9'},
{U'〇',U'0'}, {U'〡',U'1'}, {U'〢',U'2'}, {U'〣',U'3'}, {U'〤',U'4'}, {U'〥',U'5'}, {U'〦',U'6'}, {U'〧',U'7'}, {U'〨',U'8'}, {U'〩',U'9'},
{U'०',U'0'}, {U'१',U'1'}, {U'२',U'2'}, {U'३',U'3'}, {U'४',U'4'}, {U'५',U'5'}, {U'६',U'6'}, {U'७',U'7'}, {U'८',U'8'}, {U'९',U'9'},
{U'૦',U'0'}, {U'૧',U'1'}, {U'૨',U'2'}, {U'૩',U'3'}, {U'૪',U'4'}, {U'૫',U'5'}, {U'૬',U'6'}, {U'૭',U'7'}, {U'૮',U'8'}, {U'૯',U'9'},
{U'੦',U'0'}, {U'੧',U'1'}, {U'੨',U'2'}, {U'੩',U'3'}, {U'੪',U'4'}, {U'੫',U'5'}, {U'੬',U'6'}, {U'੭',U'7'}, {U'੮',U'8'}, {U'੯',U'9'},
{U'೦',U'0'}, {U'೧',U'1'}, {U'೨',U'2'}, {U'೩',U'3'}, {U'೪',U'4'}, {U'೫',U'5'}, {U'೬',U'6'}, {U'೭',U'7'}, {U'೮',U'8'}, {U'೯',U'9'},
{U'꧐',U'0'}, {U'꧑',U'1'}, {U'꧒',U'2'}, {U'꧓',U'3'}, {U'꧔',U'4'}, {U'꧕',U'5'}, {U'꧖',U'6'}, {U'꧗',U'7'}, {U'꧘',U'8'}, {U'꧙',U'9'},
{U'០',U'0'}, {U'១',U'1'}, {U'២',U'2'}, {U'៣',U'3'}, {U'៤',U'4'}, {U'៥',U'5'}, {U'៦',U'6'}, {U'៧',U'7'}, {U'៨',U'8'}, {U'៩',U'9'},
{U'𑋰',U'0'}, {U'𑋱',U'1'}, {U'𑋲',U'2'}, {U'𑋳',U'3'}, {U'𑋴',U'4'}, {U'𑋵',U'5'}, {U'𑋶',U'6'}, {U'𑋷',U'7'}, {U'𑋸',U'8'}, {U'𑋹',U'9'},
{U'໐',U'0'}, {U'໑',U'1'}, {U'໒',U'2'}, {U'໓',U'3'}, {U'໔',U'4'}, {U'໕',U'5'}, {U'໖',U'6'}, {U'໗',U'7'}, {U'໘',U'8'}, {U'໙',U'9'},
{U'0',U'0'}, {U'1',U'1'}, {U'2',U'2'}, {U'3',U'3'}, {U'4',U'4'}, {U'5',U'5'}, {U'6',U'6'}, {U'7',U'7'}, {U'8',U'8'}, {U'9',U'9'},
{U'᱀',U'0'}, {U'᱁',U'1'}, {U'᱂',U'2'}, {U'᱃',U'3'}, {U'᱄',U'4'}, {U'᱅',U'5'}, {U'᱆',U'6'}, {U'᱇',U'7'}, {U'᱈',U'8'}, {U'᱉',U'9'},
{U'᥆',U'0'}, {U'᥇',U'1'}, {U'᥈',U'2'}, {U'᥉',U'3'}, {U'᥊',U'4'}, {U'᥋',U'5'}, {U'᥌',U'6'}, {U'᥍',U'7'}, {U'᥎',U'8'}, {U'᥏',U'9'},
{U'൦',U'0'}, {U'൧',U'1'}, {U'൨',U'2'}, {U'൩',U'3'}, {U'൪',U'4'}, {U'൫',U'5'}, {U'൬',U'6'}, {U'൭',U'7'}, {U'൮',U'8'}, {U'൯',U'9'},
{U'𑵐',U'0'}, {U'𑵑',U'1'}, {U'𑵒',U'2'}, {U'𑵓',U'3'}, {U'𑵔',U'4'}, {U'𑵕',U'5'}, {U'𑵖',U'6'}, {U'𑵗',U'7'}, {U'𑵘',U'8'}, {U'𑵙',U'9'},
{U'꯰',U'0'}, {U'꯱',U'1'}, {U'꯲',U'2'}, {U'꯳',U'3'}, {U'꯴',U'4'}, {U'꯵',U'5'}, {U'꯶',U'6'}, {U'꯷',U'7'}, {U'꯸',U'8'}, {U'꯹',U'9'},
{U'𑙐',U'0'}, {U'𑙑',U'1'}, {U'𑙒',U'2'}, {U'𑙓',U'3'}, {U'𑙔',U'4'}, {U'𑙕',U'5'}, {U'𑙖',U'6'}, {U'𑙗',U'7'}, {U'𑙘',U'8'}, {U'𑙙',U'9'},
{U'᠐',U'0'}, {U'᠑',U'1'}, {U'᠒',U'2'}, {U'᠓',U'3'}, {U'᠔',U'4'}, {U'᠕',U'5'}, {U'᠖',U'6'}, {U'᠗',U'7'}, {U'᠘',U'8'}, {U'᠙',U'9'},
{U'𖩠',U'0'}, {U'𖩡',U'1'}, {U'𖩢',U'2'}, {U'𖩣',U'3'}, {U'𖩤',U'4'}, {U'𖩥',U'5'}, {U'𖩦',U'6'}, {U'𖩧',U'7'}, {U'𖩨',U'8'}, {U'𖩩',U'9'},
{U'၀',U'0'}, {U'၁',U'1'}, {U'၂',U'2'}, {U'၃',U'3'}, {U'၄',U'4'}, {U'၅',U'5'}, {U'၆',U'6'}, {U'၇',U'7'}, {U'၈',U'8'}, {U'၉',U'9'},
{U'𑑐',U'0'}, {U'𑑑',U'1'}, {U'𑑒',U'2'}, {U'𑑓',U'3'}, {U'𑑔',U'4'}, {U'𑑕',U'5'}, {U'𑑖',U'6'}, {U'𑑗',U'7'}, {U'𑑘',U'8'}, {U'𑑙',U'9'},
{U'᧐',U'0'}, {U'᧑',U'1'}, {U'᧒',U'2'}, {U'᧓',U'3'}, {U'᧔',U'4'}, {U'᧕',U'5'}, {U'᧖',U'6'}, {U'᧗',U'7'}, {U'᧘',U'8'}, {U'᧙',U'9'},
{U'୦',U'0'}, {U'୧',U'1'}, {U'୨',U'2'}, {U'୩',U'3'}, {U'୪',U'4'}, {U'୫',U'5'}, {U'୬',U'6'}, {U'୭',U'7'}, {U'୮',U'8'}, {U'୯',U'9'},
{U'𐒡',U'0'}, {U'𐒢',U'1'}, {U'𐒣',U'2'}, {U'𐒤',U'3'}, {U'𐒥',U'4'}, {U'𐒦',U'5'}, {U'𐒧',U'6'}, {U'𐒨',U'7'}, {U'𐒩',U'8'}, {U'𐒠',U'9'},
{U'꣐',U'0'}, {U'꣑',U'1'}, {U'꣒',U'2'}, {U'꣓',U'3'}, {U'꣔',U'4'}, {U'꣕',U'5'}, {U'꣖',U'6'}, {U'꣗',U'7'}, {U'꣘',U'8'}, {U'꣙',U'9'},
{U'𑇐',U'0'}, {U'𑇑',U'1'}, {U'𑇒',U'2'}, {U'𑇓',U'3'}, {U'𑇔',U'4'}, {U'𑇕',U'5'}, {U'𑇖',U'6'}, {U'𑇗',U'7'}, {U'𑇘',U'8'}, {U'𑇙',U'9'},
{U'𑇐',U'0'}, {U'𑇑',U'1'}, {U'𑇒',U'2'}, {U'𑇓',U'3'}, {U'𑇔',U'4'}, {U'𑇕',U'5'}, {U'𑇖',U'6'}, {U'𑇗',U'7'}, {U'𑇘',U'8'}, {U'𑇙',U'9'},
{U'෦',U'0'}, {U'෧',U'1'}, {U'෨',U'2'}, {U'෩',U'3'}, {U'෪',U'4'}, {U'෫',U'5'}, {U'෬',U'6'}, {U'෭',U'7'}, {U'෮',U'8'}, {U'෯',U'9'},
{U'𑃰',U'0'}, {U'𑃱',U'1'}, {U'𑃲',U'2'}, {U'𑃳',U'3'}, {U'𑃴',U'4'}, {U'𑃵',U'5'}, {U'𑃶',U'6'}, {U'𑃷',U'7'}, {U'𑃸',U'8'}, {U'𑃹',U'9'},
{U'᪐',U'0'}, {U'᪑',U'1'}, {U'᪒',U'2'}, {U'᪓',U'3'}, {U'᪔',U'4'}, {U'᪕',U'5'}, {U'᪖',U'6'}, {U'᪗',U'7'}, {U'᪘',U'8'}, {U'᪙',U'9'},
{U'𑛀',U'0'}, {U'𑛁',U'1'}, {U'𑛂',U'2'}, {U'𑛃',U'3'}, {U'𑛄',U'4'}, {U'𑛅',U'5'}, {U'𑛆',U'6'}, {U'𑛇',U'7'}, {U'𑛈',U'8'}, {U'𑛉',U'9'},
{U'௦',U'0'}, {U'௧',U'1'}, {U'௨',U'2'}, {U'௩',U'3'}, {U'௪',U'4'}, {U'௫',U'5'}, {U'௬',U'6'}, {U'௭',U'7'}, {U'௮',U'8'}, {U'௯',U'9'},
{U'౦',U'0'}, {U'౧',U'1'}, {U'౨',U'2'}, {U'౩',U'3'}, {U'౪',U'4'}, {U'౫',U'5'}, {U'౬',U'6'}, {U'౭',U'7'}, {U'౮',U'8'}, {U'౯',U'9'},
{U'๐',U'0'}, {U'๑',U'1'}, {U'๒',U'2'}, {U'๓',U'3'}, {U'๔',U'4'}, {U'๕',U'5'}, {U'๖',U'6'}, {U'๗',U'7'}, {U'๘',U'8'}, {U'๙',U'9'},
{U'༠',U'0'}, {U'༡',U'1'}, {U'༢',U'2'}, {U'༣',U'3'}, {U'༤',U'4'}, {U'༥',U'5'}, {U'༦',U'6'}, {U'༧',U'7'}, {U'༨',U'8'}, {U'༩',U'9'},
{U'𑓐',U'0'}, {U'𑓑',U'1'}, {U'𑓒',U'2'}, {U'𑓓',U'3'}, {U'𑓔',U'4'}, {U'𑓕',U'5'}, {U'𑓖',U'6'}, {U'𑓗',U'7'}, {U'𑓘',U'8'}, {U'𑓙',U'9'},
{U'۰',U'0'}, {U'۱',U'1'}, {U'۲',U'2'}, {U'۳',U'3'}, {U'۴',U'4'}, {U'۵',U'5'}, {U'۶',U'6'}, {U'۷',U'7'}, {U'۸',U'8'}, {U'۹',U'9'},
{U'𞋰',U'0'}, {U'𞋱',U'1'}, {U'𞋲',U'2'}, {U'𞋳',U'3'}, {U'𞋴',U'4'}, {U'𞋵',U'5'}, {U'𞋶',U'6'}, {U'𞋷',U'7'}, {U'𞋸',U'8'}, {U'𞋹',U'9'},
{U'𑣠',U'0'}, {U'𑣡',U'1'}, {U'𑣢',U'2'}, {U'𑣣',U'3'}, {U'𑣤',U'4'}, {U'𑣥',U'5'}, {U'𑣦',U'6'}, {U'𑣧',U'7'}, {U'𑣨',U'8'}, {U'𑣩',U'9'}
}; 

bool isLetter(char32_t aCodePoint);
bool isAlphaNum(char32_t aCodePoint);
bool isDigit(char32_t aCodePoint, bool aAttrs[_SP_ATTR_END]);
bool textTFNumToNum(const std::string &aString, TFNum &aNum, char32_t aDecimalPoint, char32_t aThousandsSep);
bool textUIntToNum(const std::string &aString, TFNum &aNum);
bool textHexToNum(const std::string &aString, TFNum &aNum);
bool textOctToNum(const std::string &aString, TFNum &aNum);
bool textBinToNum(const std::string &aString, TFNum &aNum);
bool numBinToText(TFNum &aNum, std::string &aString);
bool numOctToText(TFNum &aNum, std::string &aString);
bool numHexToText(TFNum &aNum, std::string &aString);
bool numFloatToText(TFNum &aNum, std::string &aString);
bool numFloatToText(TFNum &aNum, std::string &aString, std::size_t aPrecision);
bool numFloatToText(TFNum &aNum, std::string &aString, int aPrecision);
bool numToText(TFNum &aNum, std::string &aString, bool aAttrs[_SP_ATTR_END]);
bool textToNum(const std::string &aString, TFNum &aNum, bool aAttrs[_SP_ATTR_END], char32_t aDecimalPoint, char32_t aThousandsSep);
void replacePart(std::string &aText, const std::string &aSearch, const std::string &aReplace);
void unescape(std::string &aText);
void printCtrl(std::string &aText);
std::string intToFixText(std::size_t aNum, std::size_t aLen);
std::string trimText(const std::string &aText);
std::string trimArg(const std::string &aText);
std::string lowerText(const std::string &aText);
std::string upperText(const std::string &aText);
bool isLowerText(const std::string &aText);
bool isUpperText(const std::string &aText);
uint8_t compareText(const std::string &aText1, const std::string &aText2);
uint8_t compareTextICase(const std::string &aText1, const std::string &aText2);
uint8_t  compareCollation(const std::string& aCollation1, const std::string& aCollation2);
int getCPSize(const std::string &aText, const std::size_t aIndex);
int getCPSize(TFAccess &aText, const std::size_t aIndex);
char32_t getCP(const std::string &aText, const std::size_t aIndex);
char32_t getCP(TFAccess &aText, const std::size_t aIndex);
int getCP(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCP(TFAccess &aText, const std::size_t aIndex, char32_t &aCodePoint);
std::size_t skipCP(const std::string &aText, std::size_t aIndex, char32_t aCodePoint);
std::size_t getCPLen(const std::string &aString);
std::string toString(char32_t aCodePoint);
char32_t fromString(const std::string &aString);
std::size_t getNextCP(const std::string &aText, std::size_t aIndex);
std::size_t getNextCP(TFAccess &aText, std::size_t aIndex);
std::size_t getNextCP(TFAccess &aText, std::size_t aIndex, std::size_t aCount);
std::size_t getPrevCP(const std::string &aText, std::size_t aIndex);
std::size_t getPrevCP(TFAccess &aText, std::size_t aIndex);
std::size_t testBOL(TFAccess &aText, std::size_t aIndex);
std::size_t findEOL(TFAccess &aText, std::size_t aIndex);
std::size_t testBOW(TFAccess &aText, std::size_t aIndex);
std::size_t findEOW(TFAccess &aText, std::size_t aIndex);
std::size_t testBOS(TFAccess &aText, std::size_t aIndex);
std::size_t findEOS(TFAccess &aText, std::size_t aIndex);
std::size_t findBOMark(TFAccess &aText, std::size_t aIndex);
std::size_t getBOMEncoding(TFAccess &aText, std::size_t aIndex);
std::size_t findBlock(TFAccess &aText, std::size_t aIndex, uint8_t aDefine, uint8_t aExclude);
std::size_t findMarkedBlock(TFAccess &aText, std::size_t aIndex, const std::string &aStartMarker, const std::string &aEndMarker, const std::vector<std::string> aInvalidChars = {}, const std::vector<std::string> aRuleChars = {});
std::size_t findFirstOf(TFAccess &aText, std::size_t aIndex, const std::vector<std::string> aStrings);
std::size_t findBlank(TFAccess &aText, std::size_t aIndex);
std::size_t findWord(TFAccess &aText, std::size_t aIndex);
std::size_t findNumber(TFAccess &aText, std::size_t aIndex, bool aStrict, bool aAttrs[_SP_ATTR_END]);
std::size_t findNumberBegin(TFAccess &aText, std::size_t aIndex, bool aAttrs[_SP_ATTR_END]);
std::size_t findText(std::vector<std::string> &aVec, const std::string &aString);
std::size_t findStr(TFAccess &aText, const std::string &aSearch, std::size_t aStart);
std::size_t findStrCase(TFAccess &aString, const std::string &aSearch, std::size_t aStart);
bool readFromFile(const std::string &aFile, std::string &aStr);
bool writeToFile(const std::string &aFile, const std::string &aStr);
void tokenize(const std::string &aList, const std::string &aSeperator, std::vector<std::string> &aTokens);

};// end of namespace

#endif // TEXT_HPP_INCLUDED
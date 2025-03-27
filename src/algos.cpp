// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski 
// License:  BSL 1.0

#include "definitions.hpp"
#include "algos.hpp"
#include "text.hpp"
#include "extern.hpp"

namespace Scripal {

// ***************************************************************************
// calculate t levenshtein distance between two strings
// aStr1    - string 1 
// aStr2    - string 2 
// return   - distance (0..1 is perfect match)
// **************************************************************************
TFNum levenDistance(const std::string &aStr1, const std::string aStr2) {
  std::size_t m = getCPLen(aStr1);
  std::size_t n = getCPLen(aStr2);
  char32_t c1 = 0, c2 = 0;
  std::size_t index1 = 0, index2 = 0, len1, len2;

  if (m == 0)
    return(n);
  if (n == 0)
    return(m);

  std::vector<std::size_t> costs(n + 1);
  std::iota(costs.begin(), costs.end(), 0);
  std::size_t i = 0, j = 0;

  
  while (index1 < aStr1.size()) {
    costs[0] = i + 1;
    std::size_t corner = i;
    j = 0;
    len1 = getCP(aStr1, index1, c1);

    index2 = 0;
    while (index2 < aStr2.size()) {
      len2 = getCP(aStr2, index2, c2);
      size_t upper = costs[j + 1];
      costs[j + 1] = (Poco::Unicode::toUpper(c1) == Poco::Unicode::toUpper(c2)) ? corner : 1 + std::min(std::min(upper, corner), costs[j]);
      corner = upper;
      index2 += len2;
      ++j;
    }
    index1 += len1;
    ++i;
  }

  TFNum result = 1;
  if (costs[n] != 0)
    result = (TFNum)1 - ((TFNum)costs[n] / (TFNum)std::max(i, j));
    //std::cout << "cost, max, res: " << costs[n] << " , " << std::max(i , j) << " , " << result << "\n";
  return (result);
}

// ***************************************************************************
// find Jaro similarity of two strings (UTF32 and UTF8 versions)
// aStr1,aStr2 : strings to compare
// return - Jaro distance 0 (not at all equal).. 1 (equal)
// ***************************************************************************
TFNum jaroDistance(const std::u32string &aStr1, const std::u32string &aStr2) {
  if (aStr1 == aStr2)
    return(1.0);

  // length of two strings
  int len1 = aStr1.size();
  int len2 = aStr2.size();

  if ((len1 == 0) || (len2 == 0))
    return(0.0);
    
  // maximum distance upto which matching is allowed
  int max_dist = floor(std::max(len1, len2) / 2) - 1;

  // Count of matches
  int match = 0;

  // Hash for matches
  std::vector<int> hash_s1(len1, 0);
  std::vector<int> hash_s2(len2, 0);

  // traverse through the first string
  for (int i = 0; i < len1; i++) {
    // Check if there is any matches
    for (int j = std::max(0, i - max_dist); j < std::min(len2, i + max_dist + 1); j++) {
      // If there is a match
      if ((Poco::Unicode::toUpper(aStr1[i]) == Poco::Unicode::toUpper(aStr2[j])) && (hash_s2[j] == 0)) {
        hash_s1[(std::size_t)i] = 1;
        hash_s2[(std::size_t)j] = 1;
        match++;
        break;
      }  
    }
  }

  // If there is no match
  if (match == 0)
    return 0.0;

  // Number of transpositions
  TFNum t = 0;

  int point = 0;

  // count number of occurrences where two characters match but
  // there is a third matched character in between the indices
  for (int i = 0; i < len1; i++) {
    if (hash_s1[i]) {
      // find the next matched character in second string
      while (hash_s2[point] == 0)
        point++;

      if (Poco::Unicode::toUpper(aStr1[i]) != Poco::Unicode::toUpper(aStr2[point++]))
        t++;
    }
  }

  t /= 2;

  return (((TFNum)match) / ((TFNum)len1) + ((TFNum)match) / ((TFNum)len2) + ((TFNum)match - t) / ((TFNum)match)) / 3.0;
}

TFNum jaroDistance(const std::string &aStr1, const std::string &aStr2) {
   return(jaroDistance(utf8ToUtf32(aStr1),  utf8ToUtf32(aStr2)));
}  

// ***************************************************************************
// find Jaro Winkler distance similarity of two strings (UTF32 and UTF8 versions)
// aStr1,aStr2 : strings to compare
// return - Jaro-Winkler distance 0 (not at all equal).. 1 (equal)
// ***************************************************************************
TFNum jaroWinklerDistance(const std::u32string &aStr1, const std::u32string &aStr2) {
  TFNum jaro_dist = jaroDistance(aStr1, aStr2);

  // if the jaro similarity is above a threshold
  if (jaro_dist > 0.7) {

    // Find the length of common prefix
    int prefix = 0;

    for (int i = 0; i < (int)std::min(aStr1.length(), aStr2.length()); i++) {
      // if the characters match
      if (Poco::Unicode::toUpper(aStr1[i]) == Poco::Unicode::toUpper(aStr2[i]))
        prefix++;
      else
        break;
    }

    // Maximum of 4 characters are allowed in prefix
    prefix = std::min(4, prefix);

    // Calculate jaro winkler Similarity
    jaro_dist += 0.1 * prefix * (1 - jaro_dist);
  }

  return jaro_dist;
}

TFNum jaroWinklerDistance(const std::string &aStr1, const std::string &aStr2) {
   return(jaroWinklerDistance( utf8ToUtf32(aStr1),  utf8ToUtf32(aStr2)));
} 

// ***************************************************************************
// calculate t levenshtein distance and favor start of text between two strings
// aStr1    - string 1 
// aStr2    - string 2 
// return   - distance (0..1 means equality)
// **************************************************************************
TFNum levenPlusDistance(const std::string &aStr1, const std::string aStr2) {
  TFNum dist = levenDistance(aStr1, aStr2);
  std::size_t len = (std::size_t)(0.75 * (TFNum)aStr1.size()); 
  if (upperText(aStr1).substr(0,len) != upperText(aStr2).substr(0,len)) {
    dist = dist * 0.8;
  }   
  return(dist);
}

// ***************************************************************************
// find nearest matches in text
// aSearch      - search string  
// aText        - text to search through
// aResults     - results
// aMin         - minimum similarity to match (0...1 means equality)
// return - true if at least one result was found 
// **************************************************************************
bool findNearest(const std::string &aSearch, TFAccess &aText, TFResults &aResults, TFNum aMin) {
  std::size_t index = 0, endIndex,  len, maxStart, maxEnd;
  TFNum dist, maxDist;
  
  bool insert;
  aResults.init(MATCH_NEAREST, config.positionType);

  aMin = std::ceil(aMin * 100.0) / 100.0; 
  if ((config.patternNearest == PATTERN_LEVEN_WORD) || (config.patternNearest == PATTERN_LEVENPLUS_WORD) || \
      (config.patternNearest == PATTERN_JAROWINKLER_WORD)) {
    // ***********************
    // word search

    while (index < aText.size()) {
      endIndex = findBlock(aText, index, CL_LETTER, CL_NO_SEPARATOR);

      if (endIndex == nPos) {
        index += getCPSize(aText, index);
        continue;
      }
      switch (config.patternNearest) {
        case PATTERN_LEVEN_WORD:
          dist = levenDistance(aSearch, aText.substr(index, endIndex - index + 1));
          break;
        case PATTERN_LEVENPLUS_WORD:
          dist = levenPlusDistance(aSearch, aText.substr(index, endIndex - index + 1));
          break;
        case PATTERN_JAROWINKLER_WORD:
          dist = jaroWinklerDistance(aSearch, aText.substr(index, endIndex - index + 1));
          break;
        default:
          dist = 0;
          break;    
      }

      if (dist >= aMin) {
        aResults.push_back("", aText.substr(index, endIndex - index + 1), {index, endIndex}, dist, "");
      }
      index = endIndex + 1;
    }  
  } else if ((config.patternNearest == PATTERN_LEVEN) || (config.patternNearest == PATTERN_JARO) ||\
    (config.patternNearest == PATTERN_JAROWINKLER)) {
    // ***********************
    // phrase search

    len = getCPLen(aSearch);
    endIndex = getNextCP(aText, index, len)  - 1;
    if (endIndex == nPos)
      return(false);
    maxStart = maxEnd = 0;
    maxDist = 0;

    while (index < aText.size()) {
      switch (config.patternNearest) {
        case PATTERN_LEVEN:
          dist = levenDistance(aSearch, aText.substr(index, endIndex - index + 1));
          break;
        case PATTERN_JARO:
          dist = jaroDistance(aSearch, aText.substr(index, endIndex - index + 1));
          break;
        case PATTERN_JAROWINKLER:
          dist = jaroWinklerDistance(aSearch, aText.substr(index, endIndex - index + 1));
          break;
        default:
          dist = 0;
          break;    
      }

      if (dist >= maxDist) {
        // new local maximum? 
        maxStart = index;
        maxEnd = endIndex;
        maxDist = dist;
        index = getNextCP(aText, index);
        if (index == nPos)
          break;
        endIndex = getNextCP(aText, endIndex);
      } else {
        // local maximum reached
        if (maxDist >= aMin) {
          // store new maximum
          aResults.push_back("", aText.substr(maxStart, maxEnd - maxStart  + 1), {maxStart, maxEnd}, maxDist, "");
          // reset maximum
          maxDist = 0;
          index = endIndex + 1;
          if (index >= aText.size())
            break;
          endIndex = getNextCP(aText, index, len)  - 1;
        } else {
          // reset maximum
          maxDist = 0;
          index = getNextCP(aText, index);
          if (index == nPos)
            break;
          endIndex = getNextCP(aText, endIndex);
        }
      } 
      // past end?
      if (endIndex == nPos) 
        endIndex = aText.size()- 1;
    }  
    // local maximum reached
    if (maxDist >= aMin) {
      // store new maximum
       aResults.push_back("", aText.substr(maxStart, maxEnd - maxStart  + 1), {maxStart, maxEnd}, maxDist, "");
    };

  };

  if (aResults.size() > 0)
    return(true);
  else   
    return(false);
} 
  
// ***************************************************************************
// find similarity between text blocks
// aSearch      - source string to compare with  
// aText        - text to compare
// aResults     - results
// aMin         - minimum similarity to match (0: no similarity, 1: equal)
// return - true if result occured 
// **************************************************************************
bool compareBlock(const std::string &aSource, TFAccess &aText, TFResults &aResults, TFNum aMin) {
  TFNum rank = 0;
  
  aResults.init(MATCH_BLOCK, config.positionType);

  switch (config.patternBlock) {
    case PATTERN_JARO:
      rank = jaroDistance(aSource, aText.substr(0, aText.size())); 
      break;
    case PATTERN_JAROWINKLER:
    case PATTERN_JAROWINKLER_WORD:
      rank = jaroWinklerDistance(aSource, aText.substr(0, aText.size())); 
      break;
    default:
      rank = 0;  
  };     
  if (rank >= aMin) 
    aResults.push_back("", "match", {0, 0}, rank, "");

  if (aResults.size() > 0)
    return(true);
  else   
    return(false);
} 
  
// ***************************************************************************
// helper template for generateString()
// **************************************************************************
template <typename TFRandom = std::mt19937>

auto randomGenerator() -> TFRandom {
    auto constexpr seed_bytes = sizeof(typename TFRandom::result_type) * TFRandom::state_size;
    auto constexpr seed_len = seed_bytes / sizeof(std::seed_seq::result_type);
    auto seed = std::array<std::seed_seq::result_type, seed_len>();
    auto dev = std::random_device();
    std::generate_n(begin(seed), seed_len, std::ref(dev));
    auto seed_seq = std::seed_seq(begin(seed), end(seed));
    return TFRandom{seed_seq};
}

// ***************************************************************************
// generate string from random ASCII chars
// aLen   - length of new string 
// aChars - chars to pick from, if empty use chars in code
// return - new string
// **************************************************************************
std::string generateString(std::size_t aLen, const std::string &aChars) {
    std::string matchChars; 
    static constexpr auto chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    if (aChars == "")
      matchChars = chars;
    else 
      matchChars = aChars;
  
    thread_local auto rng = randomGenerator<>();
    auto dist = std::uniform_int_distribution{{}, std::strlen(chars) - 1};
    auto result = std::string(aLen, '\0');
    std::generate_n(begin(result), aLen, [&]() { return aChars[dist(rng)]; });
    return result;
}

}
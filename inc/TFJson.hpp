// ***************************************************************************
// JSON functionality in library
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef TFJSON_INCLUDED
#define TFJSON_INCLUDED

#include "definitions.hpp"

namespace Scripal {

constexpr int TYPE_FILE  = 1;
constexpr int TYPE_STRING  = 2;

class TFJson {
  std::string json;
  std::string path;

  TFJson(int aType, std::string &aStr);
  bool store();
  template <class... Args> std::vector<std::string> getKeys(Args &&...args);
};
};

#endif
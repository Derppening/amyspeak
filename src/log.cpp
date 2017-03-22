// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// Implementations for member functions of the Log class
//

#include "log.h"

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

bool Log::debug_mode_ = false;

void Log::OutputMessage(const string& s) {
  cout << s << endl;
}

void Log::OutputError(const string& s) {
  OutputMessage("ERROR: " + s);
}

void Log::OutputDebug(const string& s) {
  if (debug_mode_) {
    OutputMessage("DEBUG: " + s);
  }
}

void Log::OutputWarn(const string &s) {
  OutputMessage("WARNING: " + s);
}

// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// This file manages the program arguments, initializes the patterns and
// tokens, and does basic processing for the main loop.
//

#ifndef AMYSPEAK_LOG_H_
#define AMYSPEAK_LOG_H_

#include <string>
#include <vector>

class Log {
 public:
  static void OutputDebug(const std::string &s);
  static void OutputMessage(const std::string &s);

  static bool debug_mode_;
};

#endif  //AMYSPEAK_LOG_H_

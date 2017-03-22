// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// Log class houses the framework for different levels of output messages.
//

#ifndef AMYSPEAK_LOG_H_
#define AMYSPEAK_LOG_H_

#include <string>
#include <vector>

class Log {
 public:
  static void OutputDebug(const std::string& s);
  static void OutputError(const std::string& s);
  static void OutputMessage(const std::string& s);
  static void OutputWarn(const std::string& s);

  static bool debug_mode_;
};

#endif  //AMYSPEAK_LOG_H_

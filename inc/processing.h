// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// Header file for processing.cpp
//

#ifndef AMYSPEAK_PROCESSING_H_
#define AMYSPEAK_PROCESSING_H_

#include <string>
#include <vector>

enum struct State {
  EXIT = -1,
  APPEND = 0,
  SKIP = 1,
  PROCESS = 2,
};

/**
 * Processes the tokens (words) by matching patterns and merging tokens.
 *
 * @param i Vector of tokens
 */
void DoProcessing(std::vector<std::string>* i);

namespace delimit {
/**
 * Reads the only token and process it.
 *
 * @param s String of command
 * @return True if we are skipping the processing
 */
State ProcessCommand(std::string &s);
}  // namespace delimit

namespace concat {
/**
 * Reads the only token and process it for Concatenation Mode.
 *
 * @param s String of command
 * @return State
 */
State ProcessCommand(std::string &s);
}  // concat

#endif //AMYSPEAK_PROCESSING_H_

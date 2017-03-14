// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under GPLv3.
//
// Header file for processing.cpp
//

#ifndef AMYSPEAK_PROCESSING_H_
#define AMYSPEAK_PROCESSING_H_

#include <string>
#include <vector>

/**
 * Processes the tokens (words) by matching patterns and merging tokens.
 *
 * @param i Vector of tokens
 */
void DoProcessing(std::vector<std::string> *i);

#endif //AMYSPEAK_PROCESSING_H_

// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under GPLv3.
//
// Stores the major and minor version numbers.

#ifndef AMYSPEAK_VERSION_H_
#define AMYSPEAK_VERSION_H_

#include <string>

#define MAJOR_VERSION 1
#define MINOR_VERSION 1

const std::string kBuildString = std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION);

#endif //AMYSPEAK_VERSION_H_

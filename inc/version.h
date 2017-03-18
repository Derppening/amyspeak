// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// Stores the major and minor version numbers.
//

#ifndef AMYSPEAK_VERSION_H_
#define AMYSPEAK_VERSION_H_

#include <string>

#define MAJOR_VERSION 2
#define MINOR_VERSION 0
#define PATCH_VERSION 4

const std::string kBuildString = std::to_string(MAJOR_VERSION) + "." +
    std::to_string(MINOR_VERSION) + "." +
    std::to_string(PATCH_VERSION);

#endif //AMYSPEAK_VERSION_H_

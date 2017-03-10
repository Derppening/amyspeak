#ifndef AMYSPEAK_VERSION_
#define AMYSPEAK_VERSION_

#include <string>

#define MAJOR_VERSION 1
#define MINOR_VERSION 1

const std::string kBuildString = std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION);

#endif //AMYSPEAK_VERSION_

// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// File for the Patterns class.
//
// Patterns class houses the pointer to the vector of patterns,
// initializer, and pattern-related functions.
//

#ifndef AMYSPEAK_PATTERNS_H_
#define AMYSPEAK_PATTERNS_H_

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "tokens.h"

class Patterns {
 public:
  Patterns(std::ifstream&);
  ~Patterns();

  /**
   * @return Pointer to the vector of patterns
   */
  static std::vector<std::vector<std::string>>* GetPatterns() {
    return patterns_.get();
  }

  /**
   * Reads the type of pattern, and convert to single-char types.
   *
   * @return String of pattern with converted types.
   */
  static std::string ReadPatternType(const std::string&);

 private:
  /**
   * Checks whether a token in the pattern matches any record in tokens_ or
   * verb_tokens_.
   *
   * @return True if the token is valid
   */
  bool CheckTokenValidity(const std::string&);

  /**
   * Reads all the lines in the pattern file into the patterns vector.
   */
  void ConstructPatterns(const std::vector<std::string>&);

  /**
   * Read and outputs the version of the pattern file if available.
   */
  void ReadPatternsVersion(const std::string&);

  static std::shared_ptr<std::vector<std::vector<std::string>>> patterns_;
};

#endif  //AMYSPEAK_PATTERNS_H_

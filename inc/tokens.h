// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// File for the Tokens class.
//
// Tokens class houses the pointer to the map of tokens,
// initializer, and token-related functions.


#ifndef AMYSPEAK_TOKENS_H_
#define AMYSPEAK_TOKENS_H_

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Tokens {
 public:
  Tokens(std::ifstream &);
  ~Tokens();

  /**
   * Retrieves the vector with a given token type.
   *
   * @param s Token Type
   * @return Vector of strings with given token type
   */
  static std::vector<std::string> & GetTokenType(const std::string &s);

  /**
   * Searches a string in all verb tokens.
   *
   * @param s String
   * @return True if string is found
   */
  static bool SearchVerbTokens(const std::string &s);
  /**
   * Searches a string in a given verb token.
   *
   * @param s String
   * @param cat Category of verb token.
   * @return True if string is found
   */
  static bool SearchVerbTokens(const std::string &s, const std::string cat);

 private:
  /**
   * Reads all the lines in the tokens file, and put all tokens into the
   * tokens vector.
   */
  void ConstructTokens(const std::vector<std::string> &);

  /**
   * Reads all the lines in the tokens file, and put all verb tokens into the
   * verb tokens vector.
   */
  void ConstructVerbTokens(const std::vector<std::string> &);

  /**
   * Read and outputs the version of the token file if available.
   */
  void ReadTokensVersion(const std::string &);

  static std::shared_ptr<std::map<std::string, std::vector<std::string>>> tokens_;
  static std::shared_ptr<std::map<std::string, std::vector<std::string>>> verb_tokens_;

  friend class Patterns;
};

#endif  //AMYSPEAK_TOKENS_H_

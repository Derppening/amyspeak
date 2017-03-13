#ifndef AMYSPEAK_PATTERNS_H_
#define AMYSPEAK_PATTERNS_H_

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "tokens.h"

class Patterns {
 public:
  Patterns(std::ifstream &);
  ~Patterns();

  // accessors
  inline std::shared_ptr<std::vector<std::vector<std::string>>> GetPatterns() const { return patterns_; }

  static string ReadTokenType(const string &);

 private:
  bool CheckTokenValidity(const std::string &);
  void ConstructPatterns(const std::vector<std::string> &);
  std::unique_ptr<std::vector<std::string>> ParseFile(std::ifstream &file);
  void ReadPatternsVersion(const std::string &);

  static std::shared_ptr<std::vector<std::vector<std::string>>> patterns_;
};

#endif  //AMYSPEAK_PATTERNS_H_

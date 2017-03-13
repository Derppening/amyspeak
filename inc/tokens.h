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

  // accessors
  inline std::shared_ptr<std::map<std::string, std::vector<std::string>>> GetTokens() const { return tokens_; }
  inline std::shared_ptr<std::map<std::string, std::vector<std::string>>> GetVerbTokens() const { return verb_tokens_; }

  static bool SearchVerbTokens(const std::string &s);
  static bool SearchVerbTokens(const std::string &s, const std::string cat);

 private:
  std::unique_ptr<std::vector<std::string>> ParseFile(std::ifstream &);
  void ConstructTokens(const std::vector<std::string> &);
  void ConstructVerbTokens(const std::vector<std::string> &);
  void ReadTokensVersion(const std::string &);

  static std::shared_ptr<std::map<std::string, std::vector<std::string>>> tokens_;
  static std::shared_ptr<std::map<std::string, std::vector<std::string>>> verb_tokens_;

  friend class Patterns;
};

#endif  //AMYSPEAK_TOKENS_H_

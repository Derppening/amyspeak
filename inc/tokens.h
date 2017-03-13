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
  inline auto GetTokens() const { return tokens_; }
  inline auto GetVerbTokens() const { return verb_tokens_; }

 private:
  std::unique_ptr<std::vector<std::string>> ParseFile(std::ifstream &);
  void ConstructTokens(const std::vector<std::string> &);
  void ConstructVerbTokens(const std::vector<std::string> &);
  void ReadTokensVersion(const std::string &);

  std::shared_ptr<std::map<std::string, std::vector<std::string>>> tokens_;
  std::shared_ptr<std::map<std::string, std::vector<std::string>>> verb_tokens_;
};

#endif  //AMYSPEAK_TOKENS_H_

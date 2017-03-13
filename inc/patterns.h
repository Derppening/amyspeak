#ifndef AMYSPEAK_PATTERNS_H_
#define AMYSPEAK_PATTERNS_H_

#include <fstream>
#include <memory>
#include <string>
#include <vector>

class Patterns {
 public:
  Patterns(std::ifstream &);
  ~Patterns();

 private:
  std::shared_ptr<std::vector<std::vector<std::string>>> patterns_;
};

#endif  //AMYSPEAK_PATTERNS_H_

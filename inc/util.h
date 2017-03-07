#ifndef AMYSPEAK_UTIL_
#define AMYSPEAK_UTIL_

#include <map>
#include <memory>
#include <string>
#include <vector>

void MergeTokens(std::vector<std::string> *v, std::size_t i, std::size_t check_length = 6);
bool StringIsMatch(std::string s, const std::vector<std::string> &v);
void OutputTokens(const std::vector<std::string> &v);
bool CheckStringLength(const std::string &s, const std::size_t size);
bool SearchVerbTokens(const std::string &s, const std::unique_ptr<std::map<std::string, std::vector<std::string>>> &m);
bool SearchVerbTokens(const std::string &s,
                      const std::unique_ptr<std::map<std::string, std::vector<std::string>>> &m,
                      const std::string cat);

#endif  // AMYSPEAK_UTIL_

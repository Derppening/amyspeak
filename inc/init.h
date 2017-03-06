#ifndef AMYSPEAK_INIT_
#define AMYSPEAK_INIT_

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

std::unique_ptr<std::vector<std::vector<std::string>>> ParsePatterns(std::ifstream &input_file,
                                                                     const std::unique_ptr<std::map<std::string, std::vector<std::string>>> &m_token,
                                                                     const std::unique_ptr<std::map<std::string, std::vector<std::string>>> &v_token);
std::unique_ptr<std::vector<std::string>> ParseTokens(std::ifstream &input_file);
std::unique_ptr<std::vector<std::pair<size_t, std::string>>> ParseTokenCategories(const std::vector<std::string> &in);
std::unique_ptr<std::map<std::string, std::vector<std::string>>> ConstructMatchingTokens(
    const std::vector<std::string> &tokens,
    const std::vector<std::pair<size_t, std::string>> &token_cat);
std::unique_ptr<std::map<std::string, std::vector<std::string>>> ConstructVerbToken(
    const std::vector<std::string> &tokens, size_t start_pos);
std::string ReadTokensVersion(const std::vector<std::string> &in);
std::string ReadPatternsVersion(const std::vector<std::vector<std::string>> &in);

#endif  // AMYSPEAK_INIT_

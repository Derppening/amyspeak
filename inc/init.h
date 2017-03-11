#ifndef AMYSPEAK_INIT_H_
#define AMYSPEAK_INIT_H_

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

std::unique_ptr<std::vector<std::vector<std::string>>> ParsePatterns(std::ifstream &input_file,
                                                                     const std::shared_ptr<std::map<std::string, std::vector<std::string>>> &m_token,
                                                                     const std::shared_ptr<std::map<std::string, std::vector<std::string>>> &v_token);
std::string ReadPatternsVersion(const std::vector<std::vector<std::string>> &in);

#endif  // AMYSPEAK_INIT_H_

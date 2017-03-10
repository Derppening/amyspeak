#include "init.h"

#include <algorithm>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using std::ifstream;
using std::make_unique;
using std::map;
using std::pair;
using std::shared_ptr;
using std::size_t;
using std::stringstream;
using std::string;
using std::unique_ptr;
using std::vector;

namespace {
bool CheckTokens(string &s, const map<string, vector<string>> &m_token,
                 const map<string, vector<string>> &v_token) {
  if ((s == "to_*") ||
      (s.at(0) == '!') ||
      (s.at(0) == '*') ||
      (s.front() == '\"' && s.back() == '\"')) {
    return true;
  }
  if (s.size() > 3 && s.substr(0, 3) == "to_") {
    for (auto &&t : v_token) {
      if (s == t.first) {
        return true;
      }
    }
  } else {
    for (auto &&t : m_token) {
      if (s == t.first) {
        return true;
      }
    }
  }

  return false;
}
}  // namespace

unique_ptr<vector<vector<string>>> ParsePatterns(ifstream &input_file,
                                                 const shared_ptr<map<string, vector<string>>> &m_token,
                                                 const shared_ptr<map<string, vector<string>>> &v_token) {
  vector<vector<string>> v{};

  string buffer_line;
  for (size_t i = 0; getline(input_file, buffer_line);) {
    if (buffer_line.find("//") != string::npos) {  // read "//" as single-line comment
      continue;
    } else if (buffer_line == "") {  // do not read empty lines
      continue;
    }
    v.emplace_back(vector<string>{});
    string buffer_token{};
    stringstream ss(buffer_line);
    while (ss >> buffer_token) {
      if (CheckTokens(buffer_token, *m_token, *v_token)) {
        v.at(i).emplace_back(buffer_token);
      }
    }
    ++i;
  }

  auto ptr = make_unique<vector<vector<string>>>(v);
  return ptr;
}

string ReadPatternsVersion(const vector<vector<string>> &in) {
  string version_line = in.at(0).at(0);
  if (version_line.find("VERSION") == string::npos) {
    return "";
  }
  return version_line.substr(version_line.find("=") + 1, version_line.size());
}

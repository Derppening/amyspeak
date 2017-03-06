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
using std::stringstream;
using std::string;
using std::unique_ptr;
using std::vector;

unique_ptr<vector<vector<string>>> ParsePatterns(ifstream &input_file) {
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
      v.at(i).emplace_back(buffer_token);
    }
    ++i;
  }

  auto ptr = make_unique<vector<vector<string>>>(v);
  return ptr;
}

unique_ptr<vector<string>> ParseTokens(ifstream &input_file) {
  vector<string> v{};  // vector of tokens

  string buffer_line;
  while (getline(input_file, buffer_line)) {  // read everything from the file
    if (buffer_line.find("//") != string::npos) {  // read "//" as single-line comment
      continue;
    } else if (buffer_line == "") {  // do not read empty lines
      continue;
    }
    v.emplace_back(buffer_line);
  }

  auto ptr = make_unique<vector<string>>(v);
  return ptr;
}

unique_ptr<vector<pair<size_t, string>>> ParseTokenCategories(const vector<string> &in) {
  vector<pair<size_t, string>> v{};  // vector of token categories

  for (size_t i = 0; i < in.size(); ++i) {
    string buffer_line{in.at(i)};
    if (buffer_line.find(":", buffer_line.size() - 1) != string::npos) {  // found a category!
      string category = buffer_line.substr(0, buffer_line.size() - 1);
      v.emplace_back(make_pair(i, category));
    }
  }

  auto ptr = make_unique<vector<pair<size_t, string>>>(v);
  return ptr;
}

unique_ptr<map<string, vector<string>>> ConstructMatchingTokens(const vector<string> &tokens,
                                                                const vector<pair<size_t, string>> &token_cat) {
  map<string, vector<string>> m{};

  for (auto &&p : token_cat) {
    if (p.second == "verb") {
      continue;
    }
    m.emplace(make_pair(p.second, vector<string>{}));
    for (size_t i = p.first + 1; tokens.at(i).find(":", tokens.at(i).size() - 1) == string::npos; ++i) {
      m.at(p.second).emplace_back(tokens.at(i));
      if (i + 1 >= tokens.size()) {
        break;
      }
    }
  }

  auto ptr = make_unique<map<string, vector<string>>>(m);
  return ptr;
}

unique_ptr<map<string, vector<string>>> ConstructVerbToken(const vector<string> &tokens, size_t start_pos) {
  map<string, vector<string>> m{};  // map of tokens

  for (size_t i = start_pos + 1; tokens.at(i).find(":", tokens.at(i).size() - 1) == string::npos; ++i) {
    string line_buffer{};
    stringstream ss_buf(tokens.at(i));
    vector<string> line_tokens{};
    while (ss_buf >> line_buffer) {
      line_tokens.emplace_back(line_buffer);
    }

    m.emplace(make_pair(line_tokens.at(0), vector<string>{}));
    for (size_t j = 1; j < line_tokens.size(); ++j) {
      m.at(line_tokens.at(0)).emplace_back(line_tokens.at(j));
    }

    if (i + 1 >= tokens.size()) {
      break;
    }
  }

  auto ptr = make_unique<map<string, vector<string>>>(m);
  return ptr;
}

string ReadTokensVersionInfo(const vector<string> &in) {
  for (auto &&a : in) {
    if (a.find(":", a.size() - 1) != string::npos) {
      return "";
    }
    size_t i = a.find("VERSION=");
    if (i != string::npos) {
      return a.substr(8, a.size());
    }
  }
  return "";
}

string ReadPatternsVersionInfo(const vector<vector<string>> &in) {
  string version_line = in.at(0).at(0);
  if (version_line.find("VERSION=") == string::npos) {
    return "";
  }
  return version_line.substr(8, version_line.size());
}

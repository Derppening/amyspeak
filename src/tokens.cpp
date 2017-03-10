#include "tokens.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::make_shared;
using std::make_unique;
using std::map;
using std::pair;
using std::size_t;
using std::string;
using std::stringstream;
using std::vector;
using std::unique_ptr;

void Tokens::ReadTokensVersion(const string &l) {
  if (l.find("VERSION") != string::npos) {
    string version_string = l.substr(l.find("=") + 1, l.size());
    cout << "Tokens library version: " << version_string << endl;
  }
}

Tokens::Tokens(ifstream &file) {
  tokens_ = make_shared<map<string, vector<string>>>();
  verb_tokens_ = make_shared<map<string, vector<string>>>();

  unique_ptr<vector<string>> file_lines = ParseFile(file);
  ReadTokensVersion(file_lines->at(0));

  cout << "Initializing tokens... ";
  ConstructTokens(*file_lines);
  cout << "Done." << endl;

  cout << "Initializing verb tokens... ";
  ConstructVerbTokens(*file_lines);
  if (verb_tokens_->size() != 0) {
    cout << "Done." << endl;
  } else {
    cout << "Not found." << endl;
  }

  file_lines.reset(nullptr);
}

unique_ptr<vector<string>> Tokens::ParseFile(ifstream &file) {
  vector<string> v{};  // vector of tokens

  string buffer_line;
  while (getline(file, buffer_line)) {  // read everything from the file
    if (buffer_line.back() == '\r') {
      buffer_line.pop_back();
    }
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

void Tokens::ConstructTokens(const vector<string> &in) {
  string category{""};
  for (auto l : in) {
    if (l.back() == '\r') {
      l.pop_back();
    }
    if (l.find(":") != string::npos) {  // found a category!
      category = l.substr(0, l.size() - 1);
      tokens_->emplace(make_pair(category, vector<string>{}));
    } else if (category == "verb" ||
        category == "") {
      continue;
    } else {
      tokens_->at(category).emplace_back(l);
    }
  }
}

void Tokens::ConstructVerbTokens(const std::vector<std::string> &in) {
  size_t start_pos = 0;
  for (size_t i = 0; i < in.size(); ++i) {
    if (in.at(i).find("verb") != string::npos) {
      start_pos = i;
      break;
    }
  }
  for (size_t i = start_pos + 1; in.at(i).find(":") == string::npos; ++i) {
    string line_buffer{};
    stringstream ss_buf(in.at(i));
    vector<string> line_tokens{};
    while (ss_buf >> line_buffer) {
      line_tokens.emplace_back(line_buffer);
    }

    verb_tokens_->emplace(make_pair(line_tokens.at(0), vector<string>{}));
    for (size_t j = 1; j < line_tokens.size(); ++j) {
      verb_tokens_->at(line_tokens.at(0)).emplace_back(line_tokens.at(j));
    }

    if (i + 1 >= in.size()) {
      break;
    }
  }
}

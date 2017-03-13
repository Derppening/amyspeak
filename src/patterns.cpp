#include "patterns.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "tokens.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::make_unique;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

shared_ptr<vector<vector<string>>> Patterns::patterns_ = nullptr;

void Patterns::ReadPatternsVersion(const string &l) {
  if (l.find("VERSION") != string::npos) {
    string version_string = l.substr(l.find("=") + 1, l.size());
    cout << "Patterns library version: " << version_string << endl;
  }
}

bool Patterns::CheckTokenValidity(const string &s) {
  if ((s == "to_*") ||
      (s.at(0) == '!') ||
      (s.at(0) == '*') ||
      (s.front() == '\"' && s.back() == '\"')) {
    return true;
  }
  if (s.size() > 3 && s.substr(0, 3) == "to_") {
    for (auto &&token : *(Tokens::verb_tokens_)) {
      if (s == token.first) {
        return true;
      }
    }
  } else {
    for (auto &&token : *(Tokens::tokens_)) {
      if (s == token.first) {
        return true;
      }
    }
  }

  return false;
}

Patterns::Patterns(ifstream &file) {
  // create the vector of patterns
  patterns_ = make_unique<vector<vector<string>>>();

  // read the file and output the version (if exists)
  unique_ptr<vector<string>> file_lines = ParseFile(file);
  ReadPatternsVersion(file_lines->at(0));

  // place the patterns into the vector
  cout << "Initializing patterns...";
  ConstructPatterns(*file_lines);
  cout << "Done." << endl;

  file_lines.reset(nullptr);
}

Patterns::~Patterns() {
  patterns_.reset();
}

unique_ptr<vector<string>> Patterns::ParseFile(ifstream &file) {
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

  // write resulting vector into smart pointer
  auto ptr = make_unique<vector<string>>(v);
  return ptr;
}

void Patterns::ConstructPatterns(const vector<string> &in) {
  for (size_t i = 0; i < in.size(); ++i) {
    patterns_->emplace_back(vector<string>{});
    string buffer_token{};
    stringstream ss(in.at(i));
    while (ss >> buffer_token) {
      if (Patterns::CheckTokenValidity(buffer_token)) {
        patterns_->at(i).emplace_back(buffer_token);
      }
    }
  }
}

string Patterns::ReadTokenType(const string &s) {
  if (s.front() == '\"' && s.back() == '\"') {
    return ("L" + s.substr(1, s.size() - 2));
  } else if (s.substr(0, 3) == "to_") {
    if (s.at(3) == '*') {
      return "VW";
    } else {
      return ("V" + s);
    }
  } else if (s.at(0) == '*') {
    return "W";
  } else if (s.at(0) == '!') {
    return ("M" + s.substr(1, s.size()));
  } else {
    return ("T" + s);
  }
}

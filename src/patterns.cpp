// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// Implementations for member functions of the Patterns class.
//

#include "patterns.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "log.h"
#include "tokens.h"
#include "util.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

shared_ptr<vector<vector<string>>> Patterns::patterns_ = nullptr;

void Patterns::ReadPatternsVersion(const string& l) {
  if (l.find("VERSION") != string::npos) {
    string version_string = l.substr(l.find('=') + 1);
    Log::OutputMessage("Patterns library version: " + version_string);
  } else {
    Log::OutputDebug("Patterns library version not found.");
  }
}

bool Patterns::CheckTokenValidity(const string& s) {
  if ((s == "to_*") ||
      (s.at(0) == '!') ||
      (s.at(0) == '*') ||
      (s.front() == '\"' && s.back() == '\"')) {
    return true;
  }
  if (s.size() > 3 && s.substr(0, 3) == "to_") {
    for (auto&& token : *(Tokens::verb_tokens_)) {
      if (s == token.first) {
        return true;
      }
    }
  } else {
    for (auto&& token : *(Tokens::tokens_)) {
      if (s == token.first) {
        return true;
      }
    }
  }

  return false;
}

Patterns::Patterns(ifstream& file) {
  Log::OutputDebug("Patterns::Patterns()");

  // create the vector of patterns
  patterns_ = make_shared<vector<vector<string>>>();

  // read the file and output the version (if exists)
  unique_ptr<vector<string>> file_lines = ParseFile(file);
  ReadPatternsVersion(file_lines->at(0));

  // place the patterns into the vector
  Log::OutputDebug("Initialization of Patterns started");
  ConstructPatterns(*file_lines);
  Log::OutputDebug("Initialization of Patterns complete");

  file_lines.reset(nullptr);
}

Patterns::~Patterns() {
  Log::OutputDebug("Patterns::~Patterns()");
  patterns_.reset();
}

void Patterns::ConstructPatterns(const vector<string>& in) {
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

string Patterns::ReadPatternType(const string& s) {
  if (s.front() == '\"' && s.back() == '\"') {  // literals
    return ("L" + s.substr(1, s.size() - 2));
  }

  if (s.substr(0, 3) == "to_") {  // verbs
    if (s.at(3) == '*') {  // verb wildcard
      return "VW";
    }

    return ("V" + s);
  } else if (s.at(0) == '*') {  // wildcard
    return "W";
  } else if (s.at(0) == '!') {  // modifier
    return ("M" + s.substr(1));
  } else {  // token
    return ("T" + s);
  }
}

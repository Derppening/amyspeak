// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// Implementations for member functions of the Tokens class.
//

#include "tokens.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "log.h"
#include "util.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::make_shared;
using std::make_unique;
using std::pair;
using std::shared_ptr;
using std::size_t;
using std::string;
using std::stringstream;
using std::to_string;
using std::unordered_map;
using std::vector;
using std::unique_ptr;

shared_ptr<unordered_map<string, vector<string>>> Tokens::tokens_ = nullptr;
shared_ptr<unordered_map<string, vector<string>>> Tokens::verb_tokens_ = nullptr;

void Tokens::ReadTokensVersion(const string& l) {
  if (l.find("VERSION") != string::npos) {
    string version_string = l.substr(l.find("=") + 1, l.size());
    Log::OutputMessage("Tokens library version: " + version_string);
  }
}

Tokens::Tokens(ifstream& file) {
  Log::OutputDebug("Tokens::Tokens()");

  // create the maps of tokens
  tokens_ = make_shared<unordered_map<string, vector<string>>>();
  verb_tokens_ = make_shared<unordered_map<string, vector<string>>>();

  // read the file and output the version (if exists)
  unique_ptr<vector<string>> file_lines = ParseFile(file);
  ReadTokensVersion(file_lines->at(0));

  // place the tokens into the map
  Log::OutputDebug("Initialization of Tokens started");
  ConstructTokens(*file_lines);
  Log::OutputDebug("Initialization of Tokens complete");

  // place the verb tokens into the verb map
  Log::OutputDebug("Initialization of Verb Tokens started");
  ConstructVerbTokens(*file_lines);
  if (verb_tokens_->size() != 0) {
    Log::OutputDebug("Initialization of Verb Tokens complete");
  } else {
    Log::OutputDebug("Verb Tokens not found. Not initializing.");
  }

  file_lines.reset(nullptr);
}

Tokens::~Tokens() {
  Log::OutputDebug("Tokens::~Tokens()");
  tokens_.reset();
  verb_tokens_.reset();
}

void Tokens::ConstructTokens(const vector<string>& in) {
  string category{""};
  for (auto l : in) {  // read all lines from vector
    if (l.back() == '\r') {
      l.pop_back();
    }
    if (l.find(":") != string::npos) {  // found a new category!
      category = l.substr(0, l.size() - 1);
      tokens_->emplace(make_pair(category, vector<string>{}));  // make new map element
    } else if (category == "verb" ||
        category == "") {  // skip if null category, or category is "verb"
      continue;
    } else {
      tokens_->at(category).emplace_back(l);  // write into map.second
    }
  }
}

void Tokens::ConstructVerbTokens(const vector<string>& in) {
  size_t start_pos = 0;
  // determine starting position
  for (size_t i = 0; i < in.size(); ++i) {
    if (in.at(i).find("verb") != string::npos) {
      Log::OutputDebug("Found verb tokens at line " + to_string(i + 1));
      start_pos = i;
      break;
    }
  }

  // read tokens from starting position, until we hit a new category, or
  // we reach end of the vector
  for (size_t i = start_pos + 1; in.at(i).find(":") == string::npos; ++i) {
    string line_buffer{};
    stringstream ss_buf(in.at(i));
    vector<string> line_tokens{};
    while (ss_buf >> line_buffer) {
      line_tokens.emplace_back(line_buffer);
    }

    // read all tokens from vector, using first token as key
    verb_tokens_->emplace(make_pair(line_tokens.at(0), vector<string>{}));
    for (size_t j = 1; j < line_tokens.size(); ++j) {
      verb_tokens_->at(line_tokens.at(0)).emplace_back(line_tokens.at(j));
    }

    // leave if we reach end of vector
    if (i + 1 >= in.size()) {
      break;
    }
  }
}

bool Tokens::SearchVerbTokens(const string& s) {
  for (auto&& m_e : *verb_tokens_) {
    for (auto&& v_e : m_e.second) {
      if (v_e == s) {
        return true;
      }
    }
  }
  return false;
}

bool Tokens::SearchVerbTokens(const string& s, const string& cat) {
  if (verb_tokens_->find(cat) == verb_tokens_->end()) {
    return false;
  }
  for (auto&& v_e : verb_tokens_->at(cat)) {
    if (v_e == s) {
      return true;
    }
  }
  return false;
}

vector<string>& Tokens::GetTokenType(const string& s) {
  return tokens_->at(s);
}

// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under GPLv3.
//
// Implementations for member functions of the Tokens class.
//

#include "tokens.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "util.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::make_shared;
using std::make_unique;
using std::map;
using std::pair;
using std::shared_ptr;
using std::size_t;
using std::string;
using std::stringstream;
using std::vector;
using std::unique_ptr;

shared_ptr<map<string, vector<string>>> Tokens::tokens_ = nullptr;
shared_ptr<map<string, vector<string>>> Tokens::verb_tokens_ = nullptr;

void Tokens::ReadTokensVersion(const string &l) {
  if (l.find("VERSION") != string::npos) {
    string version_string = l.substr(l.find("=") + 1, l.size());
    cout << "Tokens library version: " << version_string << endl;
  }
}

Tokens::Tokens(ifstream &file) {
  // create the maps of tokens
  tokens_ = make_shared<map<string, vector<string>>>();
  verb_tokens_ = make_shared<map<string, vector<string>>>();

  // read the file and output the version (if exists)
  unique_ptr<vector<string>> file_lines = ParseFile(file);
  ReadTokensVersion(file_lines->at(0));

  // place the tokens into the map
  cout << "Initializing tokens... ";
  ConstructTokens(*file_lines);
  cout << "Done." << endl;

  // place the verb tokens into the verb map
  cout << "Initializing verb tokens... ";
  ConstructVerbTokens(*file_lines);
  if (verb_tokens_->size() != 0) {
    cout << "Done." << endl;
  } else {
    cout << "Not found." << endl;
  }

  file_lines.reset(nullptr);
}

Tokens::~Tokens() {
  tokens_.reset();
  verb_tokens_.reset();
}

void Tokens::ConstructTokens(const vector<string> &in) {
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

void Tokens::ConstructVerbTokens(const vector<string> &in) {
  size_t start_pos = 0;
  // determine starting position
  for (size_t i = 0; i < in.size(); ++i) {
    if (in.at(i).find("verb") != string::npos) {
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

    // read all tokens from vector, using first token as
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

bool Tokens::SearchVerbTokens(const string &s) {
  for (auto &&m_e : *verb_tokens_) {
    for (auto &&v_e : m_e.second) {
      if (v_e == s) {
        return true;
      }
    }
  }
  return false;
}

bool Tokens::SearchVerbTokens(const string &s, const string cat) {
  try {
    verb_tokens_->at(cat);
  } catch (std::out_of_range) {
    return false;
  }
  for (auto &&v_e : verb_tokens_->at(cat)) {
    if (v_e == s) {
      return true;
    }
  }
  return false;
}

vector<string> & Tokens::GetTokenType(const string &s) {
  return tokens_->at(s);
}

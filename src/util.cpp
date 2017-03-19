// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// Utilities to modularize the application.
//

#include "util.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "log.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::make_unique;
using std::setprecision;
using std::size_t;
using std::string;
using std::stringstream;
using std::transform;
using std::unique_ptr;
using std::vector;

namespace {
/**
 * Removes punctuation from a string
 *
 * @param s String
 */
void TruncatePunctuation(string* s) {
  for (size_t i = s->size(); i-- > 0;) {
    switch (s->at(i)) {
      case '!':
      case '?':
      case '.':
      case ',':
        s->pop_back();
      default:
        break;
    }
  }
}
}  // namespace

void MergeTokens(vector<string>* v, size_t i, size_t check_length) {
  if ((i + 1) >= v->size()) {
    return;
  }
  if (CheckStringLength(v->at(i), check_length) ||
      CheckStringLength(v->at(i + 1), check_length)) {
    Log::OutputDebug("Merge \"" + v->at(i) + "\" \"" + v->at(i + 1) + "\": Skipped");
    return;
  }
  Log::OutputDebug("Merge \"" + v->at(i) + "\" \"" + v->at(i + 1) + "\"");
  v->at(i) += (" " + v->at(i + 1));
  v->erase(cbegin(*v) + i + 1);
}

bool StringIsMatch(string s, const vector<string>& v) {
  TruncatePunctuation(&s);
  transform(s.begin(), s.end(), s.begin(), ::tolower);
  for (const string& s_t : v) {
    if (s_t == s) {
      return true;
    }
  }
  return false;
}

void OutputTokens(const vector<string>& v) {
  string tokens{};
  for (const string& s : v) {
    tokens += (s + "\n");
  }
  cout << tokens << endl;
}

bool CheckStringLength(const string& s, const size_t size) {
  return s.size() > size;
}

unique_ptr<vector<string>> ParseFile(ifstream& file) {
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

string FloatToString(double d) {
  stringstream ss;
  ss << setprecision(4) << d;
  return ss.str();
}

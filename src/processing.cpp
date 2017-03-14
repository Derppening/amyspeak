// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// Checks the input vector for matching patterns, and merge the tokens (words)
// if there are any matches.
//

#include "processing.h"

#include <iostream>
#include <string>
#include <vector>

#include "patterns.h"
#include "tokens.h"
#include "util.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

void DoProcessing(vector<string> *i) {
  for (size_t it = 0; it < i->size(); ++it) {  // loop vector of string
    // input vector size checks
    if (i->size() <= 1) {
      break;
    }

    // punctuation checks
    if (i->at(it).back() == ',') {  // (.+),
      i->at(it).pop_back();
      continue;
    } else if ((i->at(it).back()) == '.' &&
        (i->at(it).at(i->at(it).size() - 2)) != '.') {  // (.+)(\.\.)
      i->at(it).pop_back();
      continue;
    } else if (i->at(it).find("\'") != string::npos) {  // (.*\'.*) i.e. contractions
      MergeTokens(i, it);
      continue;
    }

    // string length checks
    if (CheckStringLength(i->at(it), 6)) {  // don't process if word is longer than 6-char
      continue;
    }

    // word pattern matches
    vector<string> *match = nullptr;
    string modifier{};
    for (auto &&p : *Patterns::GetPatterns()) {  // loop patterns
      if (match != nullptr) {
        break;
      }
      match = &p;
      for (size_t iit = 0; iit < p.size(); ++iit) {  // loop pattern tokens
        modifier = "";
        string pattern{Patterns::ReadPatternType(p.at(iit))};
        if (pattern.at(0) == 'T') {
          if (StringIsMatch(i->at(it + iit), Tokens::GetTokenType(pattern.substr(1, pattern.size())))) {
            continue;
          }
        } else if (pattern.at(0) == 'L') {
          if (StringIsMatch(i->at(it + iit), {pattern.substr(1, pattern.size())})) {
            continue;
          }
        } else if (pattern.at(0) == 'W') {
          continue;
        } else if (pattern.at(0) == 'V') {
          if (pattern.at(1) == 'W') {
            if (Tokens::SearchVerbTokens(i->at(it + iit))) {
              continue;
            }
          } else {
            if (Tokens::SearchVerbTokens(i->at(it + iit), pattern.substr(1, pattern.size()))) {
              continue;
            }
          }
        } else if ((pattern.at(0) == 'M') && (pattern.find("VERSION") == string::npos)) {
          modifier = pattern.substr(1, pattern.size());
          continue;
        }
        match = nullptr;
        break;
      }
    }

    if (match != nullptr) {
      for (size_t l = 1; l < match->size(); ++l)
        if (modifier != "") {
          ++l;
          if (modifier.substr(0, 2) == "l=") {
            MergeTokens(i, it, stoul(modifier.substr(2, modifier.size())));
          } else {
            MergeTokens(i, it);
          }
        } else {
          MergeTokens(i, it);
        }
    }
  }
}

bool ProcessCommand(string &s) {
  if (s.find("exit") != string::npos ||
      s.find("quit") != string::npos) {
    cout << "Type ':q' to quit" << endl << endl;
    return true;
  } else if (s == ":clear") {
    if (system("cls")) {
      system("clear");
    }
    return true;
  }

  return false;
}

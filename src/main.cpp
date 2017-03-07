#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "init.h"
#include "util.h"
#include "version.h"

using std::chrono::duration;
using std::chrono::steady_clock;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::setprecision;
using std::size_t;
using std::string;
using std::stringstream;
using std::to_string;
using std::unique_ptr;
using std::vector;

const string kBuildString = to_string(MAJOR_VERSION) + "." + to_string(MINOR_VERSION);

void DoProcessing(vector<string> *i,
                  const unique_ptr<map<string, vector<string>>> &m_token,
                  const unique_ptr<map<string, vector<string>>> &v_token) {
  for (size_t it = 0; it < i->size(); ++it) {
    // input vector size checks
    if (i->size() <= 1) {
      break;
    }

    // punctuation checks
    if (i->at(it).back() == ',') {  // (.+),
      i->at(it).pop_back();
      continue;
    } else if (i->at(it).back() == '.') {  // (.+)\.
      if (i->at(it).at(i->at(it).size() - 2) != '.') {  // (.+)(?!(\.+)\.)
        i->at(it).pop_back();
        continue;
      }
    }

    // string length checks
    if (CheckStringLength(i->at(it), 6)) {  // don't process if word is longer than 6-char
      continue;
    }

    // word pattern matches
    if (i->at(it).find("\'") != string::npos) {  // (.*\'.*) i.e. contractions
      MergeTokens(i, it);
    } else if (StringIsMatch(i->at(it), m_token->at("pronoun")) ||  // (?pronoun) .+
        StringIsMatch(i->at(it), m_token->at("preposition")) ||  // (?preposition) .+
        StringIsMatch(i->at(it), m_token->at("article")) ||  // (?article) .+
        StringIsMatch(i->at(it), m_token->at("single_match"))) {  // (?single_match) .+
      MergeTokens(i, it, 8);
    } else if (StringIsMatch(i->at(it), {"what"})) {  // what .+
      if (SearchVerbTokens(i->at(it + 1), v_token, "to_be")) {  // what (?to_be)
        MergeTokens(i, it);
      }
    } else if (StringIsMatch(i->at(it), {"and"})) {  // and .+
      if (StringIsMatch(i->at(it + 1), m_token->at("demonstrative"))) {  // and (?demonstrative)
        MergeTokens(i, it);
      }
    } else if (SearchVerbTokens(i->at(it), v_token, "to_go")) {  // (?to_go) .+
      MergeTokens(i, it);
    } else if (SearchVerbTokens(i->at(it), v_token)) {  // (?verb_token) .+
      if (StringIsMatch(i->at(it + 1), m_token->at("preposition"))) {  // (?verb_token) (?preposition)
        MergeTokens(i, it);
      } else if (StringIsMatch(i->at(it + 1), m_token->at("demonstrative"))) {  // (?verb_token) (?demonstrative)
        MergeTokens(i, it);
      } else if (StringIsMatch(i->at(it + 1), m_token->at("pronoun"))) {  // (?verb_token) (?pronoun)
        MergeTokens(i, it);
        if (SearchVerbTokens(i->at(it + 1), v_token, "to_be")) {  // (?verb_token) (?pronoun) (?to_be)
          MergeTokens(i, it);
        }
      }
    }
  }
}

void OutputHelp(const char c[]) {
  cout << "Usage: " << c << " [OPTION]..." << endl;
  cout << "Converts a sentence to line-delimited phrases" << endl << endl;
  cout << "  -t, --token=[FILE]\tset token file path to [FILE]" << endl;
  cout << "  -p, --pattern=[FILE]\tset pattern file path to [FILE]" << endl;
  cout << "      --help\t\tdisplay this help and exit" << endl;
  cout << "      --version\t\toutput version information and exit" << endl;
}

int main(int argc, char *argv[]) {
  // read input arguments
  vector<string> argvec{};
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      argvec.emplace_back(argv[i]);
    }
  }

  // analyze command line switches
  string token_src = "../tokens";
  string pattern_src = "../patterns";
  for (size_t i = 0; i < argvec.size(); ++i) {
    if (argvec.at(i) == "-t") {
      token_src = argvec.at(++i);
    } else if (argvec.at(i) == "-p") {
      pattern_src = argvec.at(++i);
    } else if (argvec.at(i).substr(0, 6) == "--token=") {
      token_src = argvec.at(i).substr(6, argvec.at(i).size());
    } else if (argvec.at(i).substr(0, 10) == "--pattern=") {
      pattern_src = argvec.at(++i);
    } else if (argvec.at(i) == "--help") {
      OutputHelp(argv[0]);
      return 0;
    } else if (argvec.at(i) == "--version") {
      cout << "Amyspeak " << kBuildString << endl;
      cout << "Copyright (C) 2017 Derppening" << endl;
      cout << "Written by David Mak." << endl;
      return 0;
    }
  }
  if (token_src.at(0) != '.' && token_src.at(1) != '.') {
    token_src.insert(0, "./");
  }
  if (pattern_src.at(0) != '.' && pattern_src.at(1) != '.') {
    pattern_src.insert(0, "./");
  }

  cout << "Reading tokens from " << token_src << endl;
  cout << "Reading patterns from " << pattern_src << endl;

  // see if the files exist
  unique_ptr<ifstream> token_file(new ifstream(token_src));
  if (!token_file->is_open()) {
    cout << "Error: Tokens file not found. Exiting." << endl;
    return 0;
  }
  unique_ptr<ifstream> pattern_file(new ifstream(pattern_src));
  if (!pattern_file->is_open()) {
    cout << "Error: Patterns file not found. Exiting." << endl;
    return 0;
  }

  auto time = steady_clock::now();

  // read and save normal tokens
  cout << "Initializing tokens... ";
  auto tokens = ParseTokens(*token_file);
  auto token_pos = ParseTokenCategories(*tokens);
  auto match_tokens = ConstructMatchingTokens(*tokens, *token_pos);
  cout << "Done." << endl;

  // read and save verb tokens
  cout << "Initializing verb tokens... ";
  unique_ptr<map<string, vector<string>>> match_verbs = nullptr;
  for (auto &&i : *token_pos) {
    if (i.second == "verb") {
      match_verbs = ConstructVerbToken(*tokens, i.first);
      break;
    }
  }
  if (match_verbs == nullptr) {
    cout << "Not found." << endl;
  } else {
    cout << "Done." << endl;
  }
  const string token_version_string = ReadTokensVersion(*tokens);

  tokens.reset(nullptr);
  token_file.reset(nullptr);
  token_pos.reset(nullptr);

  // read and save patterns
  cout << "Initializing patterns... ";
  auto patterns = ParsePatterns(*pattern_file, match_tokens, match_verbs);
  const string pattern_version_string = ReadPatternsVersion(*patterns);
  cout << "Done." << endl;

  pattern_file.reset(nullptr);

  if (token_version_string != "") {
    cout << "Tokens library version: " << token_version_string << endl;
  }
  if (pattern_version_string != "") {
    cout << "Patterns library version: " << pattern_version_string << endl;
  }

  auto time_taken = duration<double, std::milli>(steady_clock::now() - time);
  cout << setprecision(4) << "Initialization complete. Took "
       << time_taken.count() << " ms." << endl << endl;
  cout << "Type 'q' to quit" << endl;

  while (true) {
    cout << "> ";
    string input;
    getline(cin, input);

    if (input == "q") break;

    string buf;
    stringstream ss_buf(input);
    vector<string> input_tokens;
    while (ss_buf >> buf) {
      input_tokens.emplace_back(buf);
    }

    // do processing
    DoProcessing(&input_tokens, match_tokens, match_verbs);

    OutputTokens(input_tokens);
  }
  return 0;
}

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
#include "patterns.h"
#include "tokens.h"
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
using std::shared_ptr;
using std::size_t;
using std::stoul;
using std::string;
using std::stringstream;
using std::to_string;
using std::unique_ptr;
using std::vector;

void DoProcessing(vector<string> *i,
                  const shared_ptr<map<string, vector<string>>> &m_token,
                  const shared_ptr<map<string, vector<string>>> &v_token,
                  const unique_ptr<vector<vector<string>>> &patterns) {
  for (size_t it = 0; it < i->size(); ++it) {
    // input vector size checks
    if (i->size() <= 1) {
      break;
    }

    // punctuation checks
    if (i->at(it).back() == ',') {  // (.+),
      i->at(it).pop_back();
      continue;
    } else if ((i->at(it).back()) == '.' &&
        (i->at(it).at(i->at(it).size() - 2)) != '.') {  // (.+)(?!(\.+)\.)
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
    for (auto &&p : *patterns) {  // loop patterns
      if (match != nullptr) {
        break;
      }
      match = &p;
      for (size_t iit = 0; iit < p.size(); ++iit) {  // loop pattern tokens
        modifier = "";
        string pattern{ReadTokenType(p.at(iit))};
        if (pattern.at(0) == 'T') {
          if (StringIsMatch(i->at(it + iit), m_token->at(pattern.substr(1, pattern.size())))) {
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
            if (SearchVerbTokens(i->at(it + iit), v_token)) {
              continue;
            }
          } else {
            if (SearchVerbTokens(i->at(it + iit), v_token, pattern.substr(1, pattern.size()))) {
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

void OutputHelp(const string &s) {
  cout << "Usage: " << s << " [OPTION]..." << endl;
  cout << "Converts a sentence to line-delimited phrases" << endl << endl;
  cout << "  -t, --token=[FILE]\tset token file path to [FILE]" << endl;
  cout << "  -p, --pattern=[FILE]\tset pattern file path to [FILE]" << endl;
  cout << "      --help\t\tdisplay this help and exit" << endl;
  cout << "      --version\t\toutput version information and exit" << endl;
}

void OutputVersionInfo() {
  cout << "Amyspeak " << kBuildString << endl;
  cout << "Copyright (C) 2017 Derppening" << endl;
  cout << "Written by David Mak." << endl;
}

bool ReadCommandLineArgs(const vector<string> &args, string *token_filename, string *pattern_filename) {
  for (size_t i = 1; i < args.size(); ++i) {
    if (args.at(i) == "-t") {
      *token_filename = args.at(++i);
    } else if (args.at(i) == "-p") {
      *pattern_filename = args.at(++i);
    } else if (args.at(i).substr(0, 6) == "--token=") {
      *token_filename = args.at(i).substr(6, args.at(i).size());
    } else if (args.at(i).substr(0, 10) == "--pattern=") {
      *pattern_filename = args.at(++i);
    } else if (args.at(i) == "--help") {
      OutputHelp(args.at(0));
      return false;
    } else if (args.at(i) == "--version") {
      OutputVersionInfo();
      return false;
    }
  }

  if (token_filename->at(0) != '.' && token_filename->at(1) != '.') {
    token_filename->insert(0, "./");
  }
  if (pattern_filename->at(0) != '.' && pattern_filename->at(1) != '.') {
    pattern_filename->insert(0, "./");
  }
  cout << "Reading tokens from " << *token_filename << endl;
  cout << "Reading patterns from " << *pattern_filename << endl;

  return true;
}

int main(int argc, char *argv[]) {
  // read input arguments
  vector<string> argvec(argv, argv + argc);

  // analyze command line switches
  string token_src = "../tokens";
  string pattern_src = "../patterns";
  if (!ReadCommandLineArgs(argvec, &token_src, &pattern_src)) {
    return 0;
  }

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
  Tokens tokens(*token_file);
  token_file.reset(nullptr);

  // read and save patterns
  cout << "Initializing patterns... ";
  auto patterns = ParsePatterns(*pattern_file, tokens.GetTokens(), tokens.GetVerbTokens());
  const string pattern_version_string = ReadPatternsVersion(*patterns);
  cout << "Done." << endl;

  pattern_file.reset(nullptr);

  if (pattern_version_string != "") {
    cout << "Patterns library version: " << pattern_version_string << endl;
  }

  auto time_taken = duration<double, std::milli>(steady_clock::now() - time);
  cout << setprecision(4) << "Initialization complete. Took "
       << time_taken.count() << " ms." << endl << endl;
  cout << "Type ':clear' to clear the console, "<< endl
       << "     ':q' to quit." << endl;

  while (true) {
    cout << "> ";
    string input;
    getline(cin, input);

    if (input == ":q") break;

    string buf{};
    stringstream ss_buf(input);
    vector<string> input_tokens;
    while (ss_buf >> buf) {
      input_tokens.emplace_back(buf);
    }

    // do processing
    if (input_tokens.size() == 1) {
      if (input_tokens.at(0) == "exit") {
        cout << "Type ':q' to quit" << endl;
        continue;
      } else if (input_tokens.at(0) == ":clear") {
        if (system("cls")) {
          system("clear");
        }
        continue;
      }
    }
    DoProcessing(&input_tokens, tokens.GetTokens(), tokens.GetVerbTokens(), patterns);
    OutputTokens(input_tokens);
  }
  return 0;
}

// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under GPLv3.
//
// This file manages the program arguments, initializes the patterns and
// tokens, and does basic processing for the main loop.
//

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

#include "patterns.h"
#include "processing.h"
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

namespace {
/**
 * Outputs the help text to stdout.
 *
 * @param s Name of the program
 */
void OutputHelp(const string &s) {
  cout << "Usage: " << s << " [OPTION]..." << endl;
  cout << "Converts a sentence to line-delimited phrases" << endl << endl;
  cout << "  -t, --token=[FILE]\tset token file path to [FILE]" << endl;
  cout << "  -p, --pattern=[FILE]\tset pattern file path to [FILE]" << endl;
  cout << "      --help\t\tdisplay this help and exit" << endl;
  cout << "      --version\t\toutput version information and exit" << endl;
}

/**
 * Outputs the version information to stdout.
 */
void OutputVersionInfo() {
  cout << "Amyspeak " << kBuildString << endl;
  cout << "Copyright (C) 2017 David Mak" << endl;
  cout << "Licensed under GPLv3." << endl;
}

/**
 * Processes the command line arguments.
 *
 * @param args Vector of command-line arguments
 * @param token_filename Filename storing tokens
 * @param pattern_filename Filename storing patterns
 * @return False if program should terminate
 */
bool ReadCommandLineArgs(const vector<string> &args, string *token_filename,
                         string *pattern_filename) {
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
}  // namespace

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
  Patterns patterns(*pattern_file);
  pattern_file.reset(nullptr);

  auto time_taken = duration<double, std::milli>(steady_clock::now() - time);
  cout << setprecision(4) << "Initialization complete. Took "
       << time_taken.count() << " ms." << endl << endl;
  cout << "Type ':clear' to clear the console, " << endl
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
    // TODO: split the ':'-leading inputs to a separate function
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

    DoProcessing(&input_tokens);
    OutputTokens(input_tokens);
  }

  return 0;
}

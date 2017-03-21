// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// This file houses the concatenation mode of the program.
//

#include "modes.h"

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

#include "log.h"
#include "patterns.h"
#include "processing.h"
#include "tokens.h"
#include "util.h"

using std::chrono::duration;
using std::chrono::steady_clock;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::shared_ptr;
using std::size_t;
using std::string;
using std::stringstream;
using std::to_string;
using std::unique_ptr;
using std::vector;

void ConcatMode() {
  vector<string> tokens{};
  string sentence{};

  Log::OutputMessage("Type ':p' to process\n"
                     "Type ':q' to exit");
  while (true) {
    cout << "> ";
    string input{};
    cin >> input;

    switch (concat::ProcessCommand(input)) {
      case State::EXIT:
        Log::OutputMessage("Exiting...");
        return;
      case State::APPEND:
        ClearScreen();
        Log::OutputDebug("Append " + input);
        tokens.emplace_back(input);
        break;
      case State::PROCESS:
        ClearScreen();
        for (string s : tokens) {
          sentence += (s + " ");
        }
        Log::OutputMessage(sentence);
        tokens.clear();
      case State::SKIP:
        continue;
      default:
        // will not happen
        break;
    }
  }
}

void DelimitMode(const string& token_src, const string& pattern_src) {
  // see if the files exist
  unique_ptr<ifstream> token_file(new ifstream(token_src));
  if (!token_file->is_open()) {
    Log::OutputError("Tokens file not found. Exiting.");
    return;
  }
  unique_ptr<ifstream> pattern_file(new ifstream(pattern_src));
  if (!pattern_file->is_open()) {
    Log::OutputError("Patterns file not found. Exiting.");
    return;
  }

  auto time = steady_clock::now();

  // read and save normal tokens
  static Tokens tokens(*token_file);
  token_file.reset(nullptr);

  // read and save patterns
  static Patterns patterns(*pattern_file);
  pattern_file.reset(nullptr);

  auto time_taken = duration<double, std::milli>(steady_clock::now() - time);
  Log::OutputMessage("Initialization complete. Took " +
      FloatToString(time_taken.count()) + "ms.\n");
  Log::OutputMessage("Type ':clear' to clear the console, \n"
                         "     ':q' to quit.");

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

    // assume that if we have one token, it is a command
    if (input_tokens.size() == 1 &&
        delimit::ProcessCommand(input_tokens.at(0))) {
      continue;
    }

    DoProcessing(&input_tokens);
    OutputTokens(input_tokens);
  }
  Log::OutputMessage("Exiting...");
}

// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// This file manages the program arguments, initializes the patterns and
// tokens, and does basic processing for the main loop.
//

#include <string>
#include <vector>

#include "modes.h"
#include "log.h"
#include "version.h"

using std::size_t;
using std::string;
using std::vector;

namespace {
/**
 * Outputs the help text to stdout.
 *
 * @param s Name of the program
 */
void OutputHelp(const string& s) {
  string message{""};
  message += "Usage: " + s + " [OPTION]... [MODE]\n";
  message += "When [MODE] is 'merge', merges individual words into a sentence.\n";
  message += "Otherwise, converts a sentence to newline-delimited phrases.\n\n";
  message += "  -d, --debug\t\tdisplays additional debug messages\n";
  message += "      --help\t\tdisplay this help and exit\n";
  message += "      --version\t\toutput version information and exit\n\n";
  message += "If [MODE] is empty, the following switches are recognized:\n";
  message += "  -p, --pattern=[FILE]\tset pattern file path to [FILE]\n";
  message += "  -t, --token=[FILE]\tset token file path to [FILE]\n";
  Log::OutputMessage(message);
}

/**
 * Outputs the version information to stdout.
 */
void OutputVersionInfo() {
  string message{""};
  message += "Amyspeak " + kBuildString + "\n";
  message += "Copyright (C) 2017 David Mak\n";
  message += "Licensed under MIT.";
  Log::OutputMessage(message);
}

/**
 * Processes the command line arguments.
 *
 * @param args Vector of command-line arguments
 * @param token_filename Filename storing tokens
 * @param pattern_filename Filename storing patterns
 */
void ReadCommandLineArgs(const vector<string>& args, string* token_filename,
                         string* pattern_filename) {
  // read all command line args
  for (size_t i = 1; i < args.size(); ++i) {
    if (args.at(i) == "-t") {
      *token_filename = args.at(++i);
    } else if (args.at(i) == "-p") {
      *pattern_filename = args.at(++i);
    } else if (args.at(i).substr(0, 8) == "--token=") {
      *token_filename = args.at(i).substr(8);
    } else if (args.at(i).substr(0, 10) == "--pattern=") {
      *pattern_filename = args.at(i).substr(10);
    } else if (args.at(i) == "-d" || args.at(i) == "--debug") {
      Log::OutputMessage("Debug Mode Enabled");
      Log::debug_mode_ = true;
    } else if (args.at(i) == "--help") {
      OutputHelp(args.at(0));
      return;
    } else if (args.at(i) == "--version") {
      OutputVersionInfo();
      return;
    } else if ((args.at(i) == "merge" || args.at(i) == "concatenate")) {
      if  (i != args.size() - 1) {
        Log::OutputWarn("merge not placed at end of command");
      }
    } else {
      Log::OutputWarn("Unknown option " + args.at(i));
    }
  }

  if (args.back() == "merge") {
    ConcatMode();
    return;
  }

  if (token_filename->at(0) != '.' && token_filename->at(1) != '.') {
    token_filename->insert(0, "./");
  }
  if (pattern_filename->at(0) != '.' && pattern_filename->at(1) != '.') {
    pattern_filename->insert(0, "./");
  }
  Log::OutputMessage("Reading tokens from " + *token_filename);
  Log::OutputMessage("Reading patterns from " + *pattern_filename);

  DelimitMode(*token_filename, *pattern_filename);
}
}  // namespace

int main(int argc, char* argv[]) {
  // read input arguments
  vector<string> argvec(argv, argv + argc);

  // analyze command line switches
  string token_src = "./tokens";
  string pattern_src = "./patterns";

  ReadCommandLineArgs(argvec, &token_src, &pattern_src);

  return 0;
}

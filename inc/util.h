// Copyright (c) 2017 David Mak. All rights reserved.
// Licensed under MIT.
//
// Header file for util.h
//

#ifndef AMYSPEAK_UTIL_H_
#define AMYSPEAK_UTIL_H_

#include <fstream>
#include <memory>
#include <string>
#include <vector>

/**
 * Merges two tokens into one.
 *
 * @param v Vector of tokens
 * @param i Position of the token
 * @param check_length Lower bound of word length to reject merging
 */
void MergeTokens(std::vector<std::string>* v, std::size_t i,
                 std::size_t check_length = 6);

/**
 * Checks if a string matches any string in a vector.
 *
 * @param s String to check
 * @param v Vector of string to match
 * @return True if a match is found
 */
bool StringIsMatch(std::string s, const std::vector<std::string>& v);

/**
 * Outputs all tokens to stdout.
 *
 * @param v Vector of tokens
 */
void OutputTokens(const std::vector<std::string>& v);

/**
 * Checks the length of a string
 *
 * @param s String to check
 * @param size Max size of string
 * @return True if string is longer than size
 */
bool CheckStringLength(const std::string& s, const std::size_t size);

/**
   * Converts the ifstream into a vector storing lines of the file.
   *
   * @param file ifstream of the pattern file
   * @return Pointer to vector of lines
   */
std::unique_ptr<std::vector<std::string>> ParseFile(std::ifstream& file);

#endif  // AMYSPEAK_UTIL_H_

#include "util.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::transform;
using std::unique_ptr;
using std::vector;

void MergeTokens(vector<string> *v, size_t i, size_t check_length) {
  if ((i + 1) >= v->size()) {
    return;
  }
  if (CheckStringLength(v->at(i), check_length) || CheckStringLength(v->at(i + 1), check_length)) {
    return;
  }
  v->at(i) += (" " + v->at(i + 1));
  v->erase(cbegin(*v) + i + 1);
}

void TruncatePunctuation(string *s) {
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

bool StringIsMatch(string s, const vector<string> &v) {
  TruncatePunctuation(&s);
  transform(s.begin(), s.end(), s.begin(), ::tolower);
  for (string s_t : v) {
    if (s_t == s) {
      return true;
    }
  }
  return false;
}

void OutputTokens(const vector<string> &v) {
  string tokens{};
  for (string s : v) {
    tokens += (s + "\n");
  }
  cout << tokens << endl;
}

bool CheckStringLength(const string &s, const size_t size) {
  return s.size() > size;
}

bool SearchVerbTokens(const string &s,
                      const unique_ptr<map<string, vector<string>>> &m) {
  for (auto &&m_e : *m) {
    for (auto &&v_e : m_e.second) {
      if (v_e == s) {
        return true;
      }
    }
  }
  return false;
}

bool SearchVerbTokens(const string &s,
                      const unique_ptr<map<string, vector<string>>> &m,
                      const string cat) {
  try {
    m->at(cat);
  } catch(std::out_of_range) {
    return false;
  }
  for (auto &&v_e : m->at(cat)) {
    if (v_e == s) {
      return true;
    }
  }
  return false;
}

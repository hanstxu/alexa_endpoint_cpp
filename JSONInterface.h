#ifndef JSONINTERFACE_H
#define JSONINTERFACE_H

#include <string>
#include <ctype.h>
#include <ostream>  // not necessary on g++?

/*
 * Parsing functions and helpers
 */

// Make sure to pass in the index of the first quotation mark
inline std::string getKeyAndUpdateIndex(unsigned int& i, std::string str) {
  std::string key;
  i++;
  while (str[i] != '\"')
    key += str[i++];
  
  while (str[i] != ':')
    i++;
  i++; // move past the colon

  while (isspace(str[i]))
    i++;
  return key;
}

inline bool containsDecimal(unsigned int i, std::string str) {
  while (str[i] != ',' && !isspace(str[i]) && str[i] != '}' && str[i] != ']')
    if (str[i++] == '.')
      return true;
  return false;
}

template <typename T>
inline T getValueAndUpdateIndex(unsigned int& i, std::string std) {
  return getValueAndUpdateIndex<T>(i, std);
}

template <>
inline std::string
  getValueAndUpdateIndex<std::string>(unsigned int& i, std::string str) {
  // Conversion of null
  if (str[i] != '\"') {
    while (str[i] != ',' && !isspace(str[i]) && str[i] != '}' && str[i] != ']')
      i++;
    return "";
  }
  
  // Conversion of strings
  std::string value;
  i++;  // move past first quotation mark
  while (str[i] != '\"')
    value += str[i++];
  i++;  // move past second quotation mark
  return value;
}

template <>
inline int getValueAndUpdateIndex<int>(unsigned int& i, std::string str) {
  int value = 0;
  bool negative = false;
  if (str[i] == '-') {
    negative = true;
    i++;
  }
  while (str[i] != ',' && !isspace(str[i]) && str[i] != '}' && str[i] != ']')
    value = value * 10 + (double)(str[i++] - '0');
  return (negative ? -1 * value : value);
}

template <>
inline double
  getValueAndUpdateIndex<double>(unsigned int& i, std::string str) {
  double value = 0.0;
  bool negative = false;
  if (str[i] == '-') {
    negative = true;
    i++;
  }
  double multiplier = 1.0;
  while (str[i] != ',' && !isspace(str[i]) && str[i] != '}' && str[i] != ']') {
    if (str[i] == '.') {
      multiplier /= 10.0;
      i++;
      continue;
    }
    if (multiplier == 1.0)
      value = value * 10 + (double)(str[i++] - '0');
    else {
      value += (double)(str[i++] - '0') * multiplier;
      multiplier /= 10.0;
    }
  }
  return (negative ? -1 * value : value);
}

template <>
inline bool getValueAndUpdateIndex<bool>(unsigned int& i, std::string str) {
  std::string value;
  while (str[i] != ',' && !isspace(str[i]) && str[i] != '}' && str[i] != ']')
    value += str[i++];
  return (value == "true" ? true : false);
}

template <typename T>
inline T parseJSON(std::string str) {
  return parseJSON<T>(str);
}

/*
 * JSON construction interface and helpers
 */

inline std::string getJSONString(std::string value) {
  return "\"" + value + "\"";
}

class JSONInterface {
public:
  JSONInterface() { tab_spaces = ""; }
  virtual std::string toString() const = 0;
protected:
  std::string tab_spaces;
  void updateNumTabSpaces(int num_tabs) {
    if (num_tabs < 0) {
      tab_spaces = tab_spaces.substr(0, tab_spaces.size() - 2);
      return;
    }
    tab_spaces = "";
    for (int i = 0; i < num_tabs; i++)
      tab_spaces += "\x20\x20";
  }
};

#endif
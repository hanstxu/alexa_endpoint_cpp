#ifndef JSONARRAY_H
#define JSONARRAY_H

#include "JSONInterface.h"
#include <vector>

#include "JSONObject.h"
class JSONObject;

class JSONArray : public JSONInterface {
public:
  JSONArray();
  template <typename T>
  T get(size_t n);
  template <typename T>
  void add(T value);
  template <typename T>
  void remove(size_t n);
  void updateTabs(int num_tabs);
  std::string toString() const;
  friend std::ostream& operator<<(std::ostream& os, const JSONArray& arr);
private:
  std::vector<std::string> m_strings;
  std::vector<int> m_numbers;
  std::vector<double> m_decimals;
  std::vector<bool> m_booleans;
  std::vector<JSONArray> m_arrays;
  std::vector<JSONObject> m_objects;
};

JSONArray parseJSON_arrayHelper(unsigned int& i, std::string str);

#endif
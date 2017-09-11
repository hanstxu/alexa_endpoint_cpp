#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include "JSONInterface.h"
#include <map>

#include "JSONArray.h"
class JSONArray;

class JSONObject : public JSONInterface {
public:
  JSONObject();
  template <typename T>
  T get(std::string key);
  template <typename T>
  void add(std::string key, T value);
  template <typename T>
  void remove(std::string key);
  void updateTabs(int num_tabs);
  std::string toString() const;
  friend std::ostream& operator<<(std::ostream& os, const JSONObject& obj);
private:
  std::map<std::string, std::string> m_strings;
  std::map<std::string, int> m_numbers;
  std::map<std::string, double> m_decimals;
  std::map<std::string, bool> m_booleans;
  std::map<std::string, JSONObject> m_objects;
  std::map<std::string, JSONArray> m_arrays;
};

JSONObject parseJSON_objectHelper (unsigned int& i, std::string str);
#endif
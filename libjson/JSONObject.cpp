#include "JSONObject.h"

JSONObject::JSONObject() {}

template <typename T>
T JSONObject::get(std::string key) {
  get<T>(this, key);
}

template <>
std::string JSONObject::get<std::string>(std::string key) {
  return m_strings[key];
}

template <>
int JSONObject::get<int>(std::string key) {
  return m_numbers[key];
}

template <>
double JSONObject::get<double>(std::string key) {
  return m_decimals[key];
}

template <>
bool JSONObject::get<bool>(std::string key) {
  return m_booleans[key];
}

template <>
JSONObject JSONObject::get<JSONObject>(std::string key) {
  JSONObject temp = m_objects[key];
  temp.updateTabs(-1);
  return temp;
}

template <>
JSONArray JSONObject::get<JSONArray>(std::string key) {
  JSONArray temp = m_arrays[key];
  temp.updateTabs(-1);
  return temp;
}

template <typename T>
void JSONObject::add(std::string key, T value) {
  add<T>(this, key, value);
}

template <>
void JSONObject::add<std::string>(std::string key, std::string value) {
  m_strings[key] = value;
}

template <>
void JSONObject::add<int>(std::string key, int value) {
  m_numbers[key] = value;
}

template <>
void JSONObject::add<double>(std::string key, double value) {
  m_decimals[key] = value;
}

template <>
void JSONObject::add<bool>(std::string key, bool value) {
  m_booleans[key] = value;
}

template <>
void JSONObject::add<JSONObject>(std::string key, JSONObject value) {
  value.updateTabs(1);
  m_objects[key] = value;
}

template <>
void JSONObject::add<JSONArray>(std::string key, JSONArray value) {
  value.updateTabs(1);
  m_arrays[key] = value;
}

template <typename T>
void JSONObject::remove(std::string key) {
  remove<T>(this, key);
}

template <>
void JSONObject::remove<std::string>(std::string key) {
  m_strings.erase(key);
}

template <>
void JSONObject::remove<int>(std::string key) {
  m_numbers.erase(key);
}

template <>
void JSONObject::remove<double>(std::string key) {
  m_decimals.erase(key);
}

template <>
void JSONObject::remove<bool>(std::string key) {
  m_booleans.erase(key);
}

template <>
void JSONObject::remove<JSONObject>(std::string key) {
  m_objects.erase(key);
}

template <>
void JSONObject::remove<JSONArray>(std::string key) {
  m_arrays.erase(key);
}

void JSONObject::updateTabs(int num_tabs) {
  updateNumTabSpaces(num_tabs);
  std::map<std::string, JSONObject>::iterator obj_it;
  for (obj_it = m_objects.begin(); obj_it != m_objects.end(); ++obj_it)
    obj_it->second.updateTabs(num_tabs == -1 ? -1 : num_tabs + 1);
  std::map<std::string, JSONArray>::iterator arr_it;
  for (arr_it = m_arrays.begin(); arr_it != m_arrays.end(); ++arr_it)
    arr_it->second.updateTabs(num_tabs == -1 ? -1 : num_tabs + 1);
}

std::string JSONObject::toString() const {
  if (m_strings.size() + m_booleans.size() + m_numbers.size() +
    m_decimals.size() + m_objects.size() + m_arrays.size() < 1)
    return "{}";

  std::string result = "{";
  
  std::map<std::string, std::string>::const_iterator str_it = m_strings.begin();
  if (str_it != m_strings.end()) {
    result += "\n\x20\x20" + tab_spaces + getJSONString(str_it->first) + ": ";
    result += (str_it->second == "" ? "null" : getJSONString(str_it->second));
	++str_it;
  }
  for (; str_it != m_strings.end(); ++str_it) {
    result += ",\n\x20\x20" + tab_spaces + getJSONString(str_it->first) + ": ";
    result += (str_it->second == "" ? "null" : getJSONString(str_it->second));
  }
  
  std::map<std::string, int>::const_iterator num_it = m_numbers.begin();
  if (m_strings.size() < 1 && num_it != m_numbers.end()) {
    result += "\n\x20\x20" + tab_spaces + getJSONString(num_it->first);
    result += ": " + std::to_string(num_it->second);
    ++num_it;
  }
  for (; num_it != m_numbers.end(); ++num_it) {
    result += ",\n\x20\x20" + tab_spaces + getJSONString(num_it->first);
    result += ": " + std::to_string(num_it->second);
  }
  
  std::map<std::string, double>::const_iterator dec_it = m_decimals.begin();
  if (m_strings.size() < 1 && m_numbers.size() < 1 && dec_it != m_decimals.end()) {
    result += "\n\x20\x20" + tab_spaces + getJSONString(dec_it->first);
    result += ": " + std::to_string(dec_it->second);
    ++dec_it;
  }
  for (; dec_it != m_decimals.end(); ++dec_it) {
    result += ",\n\x20\x20" + tab_spaces + getJSONString(dec_it->first);
    result += ": " + std::to_string(dec_it->second);
  }
  
  std::map<std::string, bool>::const_iterator bool_it = m_booleans.begin();
  if (m_strings.size() < 1 && m_numbers.size() < 1 && m_decimals.size() < 1 &&
    bool_it != m_booleans.end()) {
    result += "\n\x20\x20" + tab_spaces + getJSONString(bool_it->first) + ": ";
    result += (bool_it->second ? "true" : "false");
	++bool_it;
  }
  for (; bool_it != m_booleans.end(); ++bool_it) {
    result += ",\n\x20\x20" + tab_spaces + getJSONString(bool_it->first) + ": ";
    result += (bool_it->second ? "true" : "false");
  }
  
  std::map<std::string, JSONObject>::const_iterator obj_it = m_objects.begin();
  if (m_strings.size() < 1 && m_numbers.size() < 1  && m_decimals.size() < 1 &&
     m_booleans.size() < 1 && obj_it != m_objects.end()) {
    result += "\n\x20\x20" + tab_spaces + getJSONString(obj_it->first);
    result += ": " + obj_it->second.toString();
    ++obj_it;
  }
  for (; obj_it != m_objects.end(); ++obj_it) {
    result += ",\n\x20\x20" + tab_spaces + getJSONString(obj_it->first);
    result += ": " + obj_it->second.toString();
  }
  
  std::map<std::string, JSONArray>::const_iterator arr_it = m_arrays.begin();
  if (m_strings.size() < 1 && m_numbers.size() < 1  && m_decimals.size() < 1 &&
    m_booleans.size() < 1 && m_objects.size() < 1 && arr_it != m_arrays.end()) {
    result += "\n\x20\x20" + tab_spaces + getJSONString(arr_it->first);
    result += ": " + arr_it->second.toString();
    ++arr_it;
  }
  for (; arr_it != m_arrays.end(); ++arr_it) {
    result += ",\n\x20\x20" + tab_spaces + getJSONString(arr_it->first);
    result += ": " + arr_it->second.toString();
  }
  
  return result + "\n" + tab_spaces + "}";
}

std::ostream& operator<<(std::ostream& os, const JSONObject& obj) {
  os << obj.toString();
  return os;
}



template <>
JSONObject parseJSON<JSONObject>(std::string str) {
  unsigned int i = 0;
  return parseJSON_objectHelper(i, str);
}

JSONObject parseJSON_objectHelper (unsigned int& i, std::string str) {
  JSONObject object;
  
  for (; i < str.size(); i++) {
    if (isspace(str[i]))
      continue;
    else if (str[i] == '\"') {
      std::string key = getKeyAndUpdateIndex(i, str);
      if (str[i] == '\"')
        object.add<std::string>(key, getValueAndUpdateIndex<std::string>(i, str));
      else if (str[i] == 'n')
        object.add<std::string>(key, getValueAndUpdateIndex<std::string>(i, str));
      else if (str[i] == 't' || str[i] == 'f')
        object.add<bool>(key, getValueAndUpdateIndex<bool>(i, str));
      else if (str[i] == '-' || isdigit(str[i])) {
        if (containsDecimal(i, str))
          object.add<double>(key, getValueAndUpdateIndex<double>(i, str));
        else
          object.add<int>(key, getValueAndUpdateIndex<int>(i, str));
      }
      else if (str[i] == '.')
        object.add<double>(key, getValueAndUpdateIndex<double>(i, str));
      else if (str[i] == '{')
        object.add<JSONObject>(key, parseJSON_objectHelper(i, str));
      else if (str[i] == '[')
        object.add<JSONArray>(key, parseJSON_arrayHelper(++i, str));
      else if (str[i] == '}')
        return object;
    }
    else if (str[i] == '}')
      return object;
  }
  return object;
}
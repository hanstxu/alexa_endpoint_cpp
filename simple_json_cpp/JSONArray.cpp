#include "JSONArray.h"

JSONArray::JSONArray() {}

template <typename T>
T JSONArray::get(size_t n) {
  return get<T>(this, n);
}

template <>
std::string JSONArray::get<std::string>(size_t n) {
  return m_strings[n];
}

template <>
int JSONArray::get<int>(size_t n) {
  return m_numbers[n];
}

template <>
double JSONArray::get<double>(size_t n) {
  return m_decimals[n];
}

template <>
bool JSONArray::get<bool>(size_t n) {
  return m_booleans[n];
}

template <>
JSONArray JSONArray::get<JSONArray>(size_t n) {
  return m_arrays[n];
}

template <>
JSONObject JSONArray::get<JSONObject>(size_t n) {
  return m_objects[n];
}

template <typename T>
void JSONArray::add(T value) {
  add<T>(this, value);
}

template <>
void JSONArray::add<std::string>(std::string value) {
  m_strings.push_back(value);
}

template <>
void JSONArray::add<int>(int value) {
  m_numbers.push_back(value);
}

template <>
void JSONArray::add<double>(double value) {
  m_decimals.push_back(value);
}

template <>
void JSONArray::add<bool>(bool value) {
  m_booleans.push_back(value);
}

template <>
void JSONArray::add<JSONArray>(JSONArray value) {
  value.updateTabs(1);
  m_arrays.push_back(value);
}

template <>
void JSONArray::add<JSONObject>(JSONObject value) {
  value.updateTabs(1);
  m_objects.push_back(value);
}

template <typename T>
void JSONArray::remove(size_t n) {
  remove<T>(this, n);
}

template <>
void JSONArray::remove<std::string>(size_t n) {
  m_strings.erase(m_strings.begin() + n);
}

template <>
void JSONArray::remove<int>(size_t n) {
  m_numbers.erase(m_numbers.begin() + n);
}

template <>
void JSONArray::remove<double>(size_t n) {
  m_decimals.erase(m_decimals.begin() + n);
}

template <>
void JSONArray::remove<bool>(size_t n) {
  m_booleans.erase(m_booleans.begin() + n);
}

template <>
void JSONArray::remove<JSONArray>(size_t n) {
  m_arrays.erase(m_arrays.begin() + n);
}

template <>
void JSONArray::remove<JSONObject>(size_t n) {
  m_objects.erase(m_objects.begin() + n);
}

void JSONArray::updateTabs(int num_tabs) {
  updateNumTabSpaces(num_tabs);
  for (unsigned int i = 0; i < m_arrays.size(); i++)
    m_arrays[i].updateTabs(num_tabs == -1 ? -1 : num_tabs + 1);
  for (unsigned int i = 0; i < m_objects.size(); i++)
    m_objects[i].updateTabs(num_tabs == -1 ? -1 : num_tabs + 1);
}


std::string JSONArray::toString() const {
  if (m_strings.size() + m_booleans.size() + m_numbers.size() +
    m_decimals.size() + m_arrays.size() + m_objects.size() < 1)
    return "[]";
  
  std::string result = "[";
  
  std::vector<std::string>::const_iterator str_it = m_strings.begin();
  if (str_it != m_strings.end()) {
    result += "\n\x20\x20" + tab_spaces;
    result += (*str_it == "" ? "null" : getJSONString(*str_it));
    ++str_it;
  }
  for (; str_it != m_strings.end(); ++str_it) {
    result += ",\n\x20\x20" + tab_spaces;
    result += (*str_it == "" ? "null" : getJSONString(*str_it));
  }
  
  std::vector<int>::const_iterator num_it = m_numbers.begin();
  if (m_strings.size() < 1 && num_it != m_numbers.end()) {
    result += "\n\x20\x20" + tab_spaces + std::to_string(*num_it);
    ++num_it;
  }
  for (; num_it != m_numbers.end(); ++num_it)
    result += ",\n\x20\x20" + tab_spaces + std::to_string(*num_it);

  std::vector<double>::const_iterator dec_it = m_decimals.begin();
  if (m_strings.size() < 1 && m_numbers.size() < 1 && dec_it != m_decimals.end()) {
    result += "\n\x20\x20" + tab_spaces + std::to_string(*dec_it);
    ++dec_it;
  }
  for (; dec_it != m_decimals.end(); ++dec_it)
    result += ",\n\x20\x20" + tab_spaces + std::to_string(*dec_it);
  
  std::vector<bool>::const_iterator bool_it = m_booleans.begin();
  if (m_strings.size() < 1 && m_numbers.size() < 1 && m_decimals.size() < 1 &&
    bool_it != m_booleans.end()) {
    result += "\n\x20\x20" + tab_spaces + (*bool_it ? "true" : "false");
    ++bool_it;
  }
  for (; bool_it != m_booleans.end(); ++bool_it)
    result += ",\n\x20\x20" + tab_spaces + (*bool_it ? "true" : "false");

  std::vector<JSONArray>::const_iterator arr_it = m_arrays.begin();
  if (m_strings.size() < 1 && m_numbers.size() < 1 && m_decimals.size() < 1 &&
    m_booleans.size() < 1 && arr_it != m_arrays.end()) {
    result += "\n\x20\x20" + tab_spaces + arr_it->toString();
    ++arr_it;
  }
  for (; arr_it != m_arrays.end(); ++arr_it)
    result += ",\n\x20\x20" + tab_spaces + arr_it->toString();
  
  std::vector<JSONObject>::const_iterator obj_it = m_objects.begin();
  if (m_strings.size() < 1 && m_numbers.size() < 1 && m_decimals.size() < 1 &&
    m_booleans.size() < 1 && m_arrays.size() < 1 && obj_it != m_objects.end()) {
    result += "\n\x20\x20" + tab_spaces + obj_it->toString();
    ++obj_it;
  }
  for (; obj_it != m_objects.end(); ++obj_it)
    result += ",\n\x20\x20" + tab_spaces + obj_it->toString();
  
  return result + "\n" + tab_spaces + "]";
}

std::ostream& operator<<(std::ostream& os, const JSONArray& arr) {
  os << arr.toString();
  return os;
}

template <>
JSONArray parseJSON<JSONArray>(std::string str) {
  unsigned int i = 1;
  return parseJSON_arrayHelper(i, str);
}

JSONArray parseJSON_arrayHelper(unsigned int& i, std::string str) {
  JSONArray array;
  for (; i < str.size(); i++) {
    if (isspace(str[i]))
      continue;
    else if (str[i] == '\"')
      array.add<std::string>(getValueAndUpdateIndex<std::string>(i, str));
    else if (str[i] == 'n')
      array.add<std::string>(getValueAndUpdateIndex<std::string>(i, str));
    else if (str[i] == 't' || str[i] == 'f')
      array.add<bool>(getValueAndUpdateIndex<bool>(i, str));
    else if (str[i] == '-' || isdigit(str[i])) {
      if (containsDecimal(i, str))
        array.add<double>(getValueAndUpdateIndex<double>(i, str));
      else
        array.add<int>(getValueAndUpdateIndex<int>(i, str));
    }
    else if (str[i] == '.')
      array.add<double>(getValueAndUpdateIndex<double>(i, str));
    else if (str[i] == '[')
      array.add<JSONArray>(parseJSON_arrayHelper(++i, str));
    else if (str[i] == '{')
      array.add<JSONObject>(parseJSON_objectHelper(i, str));
    else if (str[i] == ']')
      return array;
  }
  return array;
}
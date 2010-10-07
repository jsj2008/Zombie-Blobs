#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "utils.hpp"

#include <string>
#include <map>

#include <cassert>

class Settings {
public:
  Settings(int argc, char** argv);
  ~Settings();

  template <typename T>
  static T get(const std::string& name, const T& def = T()) {
    assert(s_instance);
    std::map<std::string, std::string>::iterator it = s_instance->m_map.find(name);
    return it == s_instance->m_map.end() ? def : Utils::fromString<T>(it->second);
  }

  template <typename T>
  static void set(const std::string& name, const T& value) {
    assert(s_instance);
    s_instance->m_map[name] = Utils::toString(value);
  }

private:
  std::map<std::string, std::string> m_map;
  static Settings* s_instance;
};

#endif // SETTINGS_HPP

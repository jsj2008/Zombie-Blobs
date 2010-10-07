#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "utils.hpp"

#include <string>
#include <map>

class Settings {
public:
  Settings(int argc, char** argv);

  template <typename T>
  T get(const std::string& name, const T& def = T()) {
    std::map<std::string, std::string>::iterator it = m_map.find(name);
    return it == m_map.end() ? def : Utils::fromString<T>(it->second);
  }

  template <typename T>
  void set(const std::string& name, const T& value) {
    m_map[name] = Utils::toString(value);
  }

private:
  std::map<std::string, std::string> m_map;
};

#endif // SETTINGS_HPP

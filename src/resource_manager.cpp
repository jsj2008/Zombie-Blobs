#include "resource_manager.hpp"
#include "utils.hpp"

#include <cassert>

#ifdef _WIN32
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

ResourceManager* ResourceManager::s_instance = 0;

ResourceManager::ResourceManager() {
  if (!s_instance)
    s_instance = this;
}

ResourceManager::~ResourceManager() {
  if (s_instance == this)
    s_instance = 0;
}

void ResourceManager::addPath(const std::string& prefix, const std::string& path) {
  assert(s_instance);
  std::string p = path;
#ifdef _WIN32
  for (std::string::iterator it = p.begin(); it != p.end(); ++it)
    if (*it == '/') *it = SEPARATOR;
#endif
  if(p.empty() || *p.rbegin() != SEPARATOR) {
    p += SEPARATOR;
  }

  s_instance->m_map[prefix].push_front(p);
}

void ResourceManager::removePath(const std::string& prefix, const std::string& path) {
  assert(s_instance);
  s_instance->m_map[prefix].remove(path);
}

void ResourceManager::remove(const std::string& prefix) {
  assert(s_instance);
  s_instance->m_map.erase(prefix);
}

std::string ResourceManager::find(const std::string& prefix, const std::string& path) {
  assert(s_instance);

  std::string p = path;
#ifdef _WIN32
  for (std::string::iterator it = p.begin(); it != p.end(); ++it)
    if (*it == '/') *it = SEPARATOR;
#endif

  const List& list = s_instance->m_map[prefix];
  for (List::const_iterator it = list.begin(); it != list.end(); ++it) {
    if (Utils::fileExists(*it + p))
      return *it + p;
  }
  return "";
}

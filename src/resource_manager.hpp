#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>
#include <list>
#include <map>

/**
 * Locates all resource files.
 *
 * Example use:
 * in main():
 *   ResourceManager rm; // make the singleton object
 *   ResourceManager::addPath("sounds", "/usr/share/ZombieBlobs/sounds");
 *   ResourceManager::addPath("sounds", "/home/foo/ZB-ExtraSoundsPack");
 *   ResourceManager::addPath("level", "/usr/share/ZombieBlobs/themes/common");
 *
 * Before starting a new level:
 *   ResourceManager::addPath("level", "/usr/share/ZombieBlobs/themes/winter");
 * Playing level:
 *   ... = ResourceManager::find("level", "ground/water.png");
 * Cleaning up after level:
 *   ResourceManager::removePath("level", "/usr/share/ZombieBlobs/themes/winter");
 */
class ResourceManager {
public:
  ResourceManager();
  ~ResourceManager();

  static void addPath(const std::string& prefix, const std::string& path);
  static void removePath(const std::string& prefix, const std::string& path);
  static void remove(const std::string& prefix);
  static std::string find(const std::string& prefix, const std::string& path);

private:
  static ResourceManager* s_instance;
  typedef std::list<std::string> List;
  typedef std::map<std::string, List> Map;
  Map m_map;
};

#endif // RESOURCE_MANAGER_HPP

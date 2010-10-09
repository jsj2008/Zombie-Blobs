#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>
#include <list>

/**
 * Locates all resource files.
 *
 * Example use:
 * in main():
 *   ResourceManager rm; // make the singleton object
 *   ResourceManager::addPath("/usr/share/ZombieBlobs/sounds");
 *   ResourceManager::addPath("/home/foo/ZB-ExtraSoundsPack");
 *   ResourceManager::addPath("/usr/share/ZombieBlobs/themes/common");
 *
 * Before starting a new level:
 *   ResourceManager::addPath("/usr/share/ZombieBlobs/themes/winter");
 * Playing level:
 *   ... = ResourceManager::find("ground/water.png");
 * Cleaning up after level:
 *   ResourceManager::removePath("/usr/share/ZombieBlobs/themes/winter");
 */
class ResourceManager {
public:
  ResourceManager();
  ~ResourceManager();

  static void addPath(const std::string& path);
  static void removePath(const std::string& path);
  static std::string find(const std::string& resource);

  static std::string readFile(const std::string& resource);

private:
  static ResourceManager* s_instance;
  typedef std::list<std::string> List;
  List m_list;
};

#endif // RESOURCE_MANAGER_HPP

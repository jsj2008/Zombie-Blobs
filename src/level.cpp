#include "level.hpp"

#include <cassert>
#include <string>

Level::Level()
{

}

void Level::load()
{
  assert( m_heightMap.load(std::string("map.tga")) );
}

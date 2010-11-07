#ifndef MARCHING_CUBES_HPP
#define MARCHING_CUBES_HPP

#include <vector>
#include <stdint.h>
#include <LinearMath/btVector3.h>

class MarchingCubes
{
public:
  // data[y*w + x] = intensity of metaellipsoid at (x,y)
  static bool triangulateGrid(const uint8_t * data,
                          int width, int height,
                          std::vector<btVector3>& verts, std::vector<btVector3>& normals);
private:
  MarchingCubes();
};

#endif // MARCHING_CUBES_HPP

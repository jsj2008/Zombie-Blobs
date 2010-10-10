#ifndef GRID_HPP
#define GRID_HPP

#include "forward.hpp"
#include "render_context.hpp"

#include <vector>

class Grid {
public:
  typedef std::set<Renderable*> GridSet;

  Grid(float width, float height, int resx, int resy);

  GridSet& grid(int x, int y);
  void objects(RenderContext::Objects& objs, Camera& camera);

private:
  int next(float points[][2], int size, int current, float direction);
  void fill(RenderContext::Objects& objs, float points[][2], int size);
  void scanLine(RenderContext::Objects& objs, int y, int x1, int x2);

  /// 1/cell_width, 1/cell_height
  float m_wInv, m_hInv;
  /// resolution of the grid
  int m_width, m_height;

  std::vector<GridSet> m_data;
};

#endif // GRID_HPP

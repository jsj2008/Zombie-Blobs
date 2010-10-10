#include "grid.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "entity.hpp"

#include <LinearMath/btVector3.h>

#include <cmath>

typedef std::set<Renderable*> GridSet;

Grid::Grid(float width, float height, int resx, int resy)
  : m_wInv(resx/width), m_hInv(resy/height),
    m_width(resx), m_height(resy),
    m_data(resx*resy) {}

Grid::GridSet& Grid::grid(int x, int y) {
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  if (x >= m_width) x = m_width-1;
  if (y >= m_height) y = m_height-1;

  return m_data[y*m_width+x];
}

void Grid::objects(RenderContext::Objects& objs, Camera& camera) {
  float tan_fovy = std::tan(camera.fov() * 0.5f);
  float n = camera.near(), f = camera.far(), a = camera.aspect();

  btVector3 cam = camera.pos(), dir = camera.front(), right = camera.right(), up = camera.up();

  // center of near plane
  btVector3 c = cam + dir * n;

  float near_corner[] = {a*n*tan_fovy, n*tan_fovy};
  float far_corner[] = {a*f*tan_fovy, f*tan_fovy};

  // project the eight corner points of the camera view frustum to the grid (y) plane

  float points[8][2];
  btVector3 tmp = c + right * near_corner[0] + up * near_corner[1];
  // project to y plane and scale to grid
  points[0][0] = tmp.x() * m_wInv; points[1][0] = tmp.z() * m_hInv;
  tmp = c + right * -near_corner[0] + up * near_corner[1];
  points[0][1] = tmp.x() * m_wInv; points[1][1] = tmp.z() * m_hInv;
  tmp = c + right * -near_corner[0] + up * -near_corner[1];
  points[0][2] = tmp.x() * m_wInv; points[1][2] = tmp.z() * m_hInv;
  tmp = c + right * near_corner[0] + up * -near_corner[1];
  points[0][3] = tmp.x() * m_wInv; points[1][3] = tmp.z() * m_hInv;

  // center of near plane
  c = cam + dir * f;

  tmp = c + right * far_corner[0] + up * far_corner[1];
  points[0][4] = tmp.x() * m_wInv; points[1][4] = tmp.z() * m_hInv;
  tmp = c + right * -far_corner[0] + up * far_corner[1];
  points[0][5] = tmp.x() * m_wInv; points[1][5] = tmp.z() * m_hInv;
  tmp = c + right * -far_corner[0] + up * -far_corner[1];
  points[0][6] = tmp.x() * m_wInv; points[1][6] = tmp.z() * m_hInv;
  tmp = c + right * far_corner[0] + up * -far_corner[1];
  points[0][7] = tmp.x() * m_wInv; points[1][7] = tmp.z() * m_hInv;

  fill(objs, points, 8);
}

int Grid::next(float points[][2], int size, int current, float direction) {
  int best = current == 0 ? 1 : 0;
  float* p = points[current];
  float* q = points[best];
  float qmp[] = {q[0]-p[0], q[1]-p[1]};
  for (int i = best+1; i < size; ++i) {
    if (i == current) continue;
    float rmp[] = {points[0][i]-p[0], points[1][i]-p[1]};

    if (direction * (qmp[0]*rmp[1]-qmp[1]*rmp[0]) > 0)
      continue;

    best = i;
    q = points[best];
    qmp[0] = q[0]-p[0];
    qmp[1] = q[1]-p[1];
  }

  return best;
}

// points are in grid coordinates
void Grid::fill(RenderContext::Objects& objs, float points[][2], int size) {
  // We need to 'draw' the convex hull of those points with a scan line
  // algorithm. To do that, we find two 'corner' points. After that we just
  // find the convex edge and walk that down on both sides.
  int topleft = 0, bottomright = 0;
  for (int i = 1; i < size; ++i) {
    if (points[1][i] > points[1][topleft] ||
        (points[1][i] == points[1][topleft] && points[0][i] < points[0][topleft]))
      topleft = i;
    if (points[1][i] < points[1][bottomright] ||
        (points[1][i] == points[1][bottomright] && points[0][i] > points[0][bottomright]))
      bottomright = i;
  }

  assert(topleft != bottomright);

  int ymax = std::ceil(points[1][topleft]), ymin = points[1][bottomright];
  assert(ymax >= ymin);

  int left = -1, right = -1;
  int leftnext = topleft, rightnext = topleft;

  for (int y = ymax; y >= ymin; --y) {
    while (left < 0 || (leftnext != bottomright && std::ceil(points[1][leftnext]) >= y)) {
      left = leftnext;
      leftnext = next(points, size, left, 1);
    }
    while (right < 0 || (rightnext != bottomright && std::ceil(points[1][rightnext]) >= y)) {
      right = rightnext;
      rightnext = next(points, size, right, -1);
    }

    float factor = (float(y) - points[1][leftnext]) / (points[1][left] - points[1][leftnext]);
    int x1 = factor * (points[0][left] - points[0][leftnext]);

    factor = (float(y) - points[1][rightnext]) / (points[1][right] - points[1][rightnext]);
    int x2 = std::ceil(factor * (points[0][right] - points[0][rightnext]));

    scanLine(objs, y, x1, x2);
  }
}

void Grid::scanLine(RenderContext::Objects& objs, int y, int x1, int x2) {
  assert(x1 <= x2);

  while (x1 <= x2) {
    GridSet& s = grid(x1, y);
    for (GridSet::iterator it = s.begin(); it != s.end(); ++it) {
      objs[(*it)->material()->shader()].insert(*it);
    }
    ++x1;
  }
}

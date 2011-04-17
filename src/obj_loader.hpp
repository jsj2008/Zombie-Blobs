#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include "forward.hpp"
#include "parser.hpp"

#include <LinearMath/btVector3.h>

#include <list>

std::istream& operator>>(std::istream& is, btVector3& v);

class ObjLoader : public Parser
{
public:
  ObjLoader();
  EntityPtr entity();

protected:
  void start();
  void finish();
  void generateObject();
  void object(std::istream& line);
  void index(std::istream& line);
  void face(std::istream& line);

  std::string m_mtllib;
  std::string m_material;
  std::string m_object;
  std::string m_smooth;

  struct Index {
    int v, vt, vn;
    Index(int v_ = 0, int vt_ = 0, int vn_ = 0) : v(v_), vt(vt_), vn(vn_) {}
    bool operator<(const Index& o) const { return v == o.v ? vt == o.vt ? vn < o.vn : vt < o.vt : v < o.v; }
  };

  btAlignedObjectArray<btVector3> m_vertices;
  btAlignedObjectArray<btVector3> m_normals;
  btAlignedObjectArray<unsigned int> m_indices;
  btAlignedObjectArray<Index> m_uniqueIndices;
  std::map<Index, unsigned int> m_indexMap;

  std::list<EntityPtr> m_objects;
  EntityPtr m_root;
};

#endif

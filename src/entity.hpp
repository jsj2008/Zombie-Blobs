#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "forward.hpp"

class Entity {
public:
  Entity();

protected:
  Model* m_model;
  Material* m_material;
};

#endif // ENTITY_HPP

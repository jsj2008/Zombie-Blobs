#ifndef MODEL_HPP
#define MODEL_HPP

#include <BulletDynamics/btBulletDynamicsCommon.h>

class Model {
public:
  Model(btCollisionObject * col=0);
  btCollisionObject * getCollisionObject()  { return m_collision; }
private:
  btCollisionObject * m_collision;
};

#endif // MODEL_HPP

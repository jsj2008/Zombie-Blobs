#include "entity.hpp"
#include "material.hpp"
#include "utils.hpp"

#include "game.hpp"
#include "player.hpp"
#include "model.hpp"
#include "physics.hpp"
#include "math.hpp"
#include "obj_loader.hpp"

#include <btBulletDynamicsCommon.h>

namespace {
  void unitSphere(int slices) {
    double theta, st, ct;
    double theta_ = -M_PI_2;
    double st_ = sin(theta_);
    double ct_ = cos(theta_);

    for (int i=0; i <= slices; ++i) {
      theta = theta_;
      st = st_;
      ct = ct_;
      theta_ = M_PI * (-0.5 + i/(double)slices);
      st_ = sin(theta_);
      ct_ = cos(theta_);

      glBegin(GL_QUAD_STRIP);
      for (int j=0; j <= slices; ++j) {
        double phi = 2 * M_PI * ((j-1)/(double)slices);
        double cp = cos(phi);
        double sp = sin(phi);
        double x = cp*ct;
        double y = sp*ct;
        double z = st;

        double x_ = cp*ct_;
        double y_ = sp*ct_;
        double z_ = st_;

        glNormal3f(x_, y_, z_);
        glVertex3f(x_, y_, z_);

        glNormal3f(x, y, z);
        glVertex3f(x, y, z);
      }
      glEnd();
    }
  }
}



Renderable::Renderable() : m_material(0) {}
Renderable::~Renderable() {}

void Renderable::update(float dt) {
  Children::iterator it = m_children.begin();
  while (it != m_children.end()) {
    (**it++).update(dt);
  }
}


Entity::Entity(Model* model) : m_model(model) {}

Entity::~Entity() {}

void Entity::render(RenderContext& r, bool bind_shader) {
  if (m_material)
    m_material->bind(r, *this, bind_shader);
  if (m_model)
    m_model->render();
}

void Entity::update(float dt) {
  Renderable::update(dt);
}

bool Entity::load(const std::string& file) {
  (void)file;

  float rad = 0.3f + 0.9f * (rand()/float(RAND_MAX));
  btCollisionShape * shape = new btSphereShape(rad);
  float mass = rad*rad;
  btRigidBody * col = new btRigidBody(mass, new btDefaultMotionState(), shape);
  Game::instance()->physics()->addRigidBody(col);
  m_model = new Model(col);
  return true;
}

EntityPtr Entity::loadFile(const std::string& file) {
  ObjLoader l;
  if (l.load(file)) return l.entity();
  return EntityPtr();
}


Enemy::Enemy() : Entity() {}

Enemy::~Enemy() {}

void Enemy::render(RenderContext& r, bool bind_shader) {
  Entity::render(r, bind_shader);


  if (!m_model || !m_model->getCollisionObject())
    return;

  btTransform & m = m_model->getCollisionObject()->getWorldTransform();

  btScalar transform[16];
  m.getOpenGLMatrix(transform);

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glMultMatrixf(transform);

  btSphereShape * sp2 = dynamic_cast<btSphereShape*>(m_model->getCollisionObject()->getCollisionShape());
  btScalar rad = sp2->getRadius();

  glScalef(rad, rad, rad);
  glColor4f(0, 1, 0, 0.6);
  unitSphere(30);

  glPopMatrix();
}

void Enemy::update(float dt) {
  PlayerPtr player = Game::instance()->player();
  btVector3 pos = player->getPosition();
  btTransform & t = m_model->getCollisionObject()->getWorldTransform();
  btVector3 dir = (pos - t.getOrigin()).normalized();
  btRigidBody * body = dynamic_cast<btRigidBody*>(m_model->getCollisionObject());
  assert( body );

  btSphereShape * sp2 = dynamic_cast<btSphereShape*>(body->getCollisionShape());
  assert(sp2);

  float r = sp2->getRadius();

  body->activate();
  //body->applyImpulse(r*r*r * 10 * dir * dt, btVector3(0.5, 0.5, 0));
  //body->applyCentralImpulse(10 * dir * dt);
  body->applyCentralForce(0.3 * r*r * dir * 1000*dt);

  Entity::update(dt);
}

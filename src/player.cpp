#include "player.hpp"
#include "utils.hpp"
#include "game.hpp"
#include "physics.hpp"

#include <LinearMath/btVector3.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include <algorithm>

Player::Player() : Camera(), m_theta(0), m_phi(0), m_body(0) {
  setFar(200.0f);
}

Player::~Player() {
  delete m_body;
}

void Player::init(btVector3 pos) {
  moveTo(pos);

  btTransform m;
  m.setOrigin(pos);

  btCollisionShape * shape = new btSphereShape(1.0f);
  float mass = 1.0f;
  btDefaultMotionState * ms = new btDefaultMotionState(m);

  m_body = new btRigidBody(mass, ms, shape);
  Game::instance()->physics()->addRigidBody(m_body);
}

void Player::update() {
  btTransform & m = m_body->getWorldTransform();
  moveTo(m.getOrigin());
}

void Player::move(float dx, float dy, float dz)
{
  btVector3 dir = m_eye - m_center;
  btVector3 n = dir.normalized();
  btVector3 move = -n * dy + btVector3(-n.y(), n.x(), 0) * dx + btVector3(0, 0, 1) * dz;

  m_body->activate();
  m_body->applyCentralForce(100*move);

  m_center += move;
  m_eye = m_center + dir;
}

void Player::rotate(float x, float y) {
  m_theta -= y/50; m_phi -= x/50;
  m_theta = std::max(-M_PI_2+1e-2, std::min(M_PI_2-1e-2, (double)m_theta));
  m_center = m_eye + btVector3(0, 1, 0).rotate(btVector3(1, 0, 0), m_theta).rotate(btVector3(0, 0, 1), m_phi);
}
btVector3 Player::getVelocity() {
  return m_body->getLinearVelocity();
}

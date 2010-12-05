#include "physics.hpp"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

Physics::Physics() : m_broadphase(0), m_world(0) {
}

void Physics::init() {
  // Build the broadphase
  m_broadphase = new btDbvtBroadphase();

  // Set up the collision configuration and dispatcher
  btCollisionConfiguration * collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

  // The actual physics solver
  btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

  // The world.
  m_world = new btSoftRigidDynamicsWorld(dispatcher, m_broadphase, solver, collisionConfiguration);
  btSoftBodyWorldInfo & softWorldInfo = m_world->getWorldInfo();
  softWorldInfo.m_gravity.setValue(0, 0, -10);
  softWorldInfo.m_sparsesdf.Initialize();
  m_world->setGravity(btVector3(0, 0, -10));
}

void Physics::update(float dt) {
  // softWorldInfo.m_sparsesdf.GarbageCollect();
  m_world->stepSimulation(dt*1000, 1);
}

void Physics::addTrimesh(btVector3 * vertices, int count) {
  btTriangleMesh* triMesh = new btTriangleMesh();
  triMesh->preallocateVertices(count / 3);

  for (int i=0; i < count; i += 3) {
    triMesh->addTriangle(vertices[i], vertices[i+1], vertices[i+2]);
  }

  btBvhTriangleMeshShape * shape = new btBvhTriangleMeshShape(triMesh, true);

  btRigidBody * body = new btRigidBody(0, new btDefaultMotionState(), shape);
  m_world->addRigidBody(body);
}

void Physics::addRigidBody(btRigidBody *body) {
  m_world->addRigidBody(body);
}


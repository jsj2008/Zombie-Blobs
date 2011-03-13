#include "physics.hpp"
#include "utils.hpp"

#include <btBulletDynamicsCommon.h>
#include <btBulletWorldImporter.h>
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

void Physics::addTrimesh(const btAlignedObjectArray<btVector3>& verts,
                         const btAlignedObjectArray<unsigned int>& indices,
                         const std::string& filename) {
  btIndexedMesh mesh;
  mesh.m_numTriangles = indices.size()/3;
  mesh.m_triangleIndexBase = (const unsigned char*)&indices[0];
  mesh.m_triangleIndexStride = (char*)&indices[3] - (char*)&indices[0];
  mesh.m_numVertices = verts.size();
  mesh.m_vertexBase = (const unsigned char*)&verts[0];
  mesh.m_vertexStride = (char*)&verts[1] - (char*)&verts[0];

  assert(sizeof(indices[0]) == 4);
  assert(sizeof(verts[0].x()) == 4);
  assert(sizeof(verts[0]) == 4*4);
  assert(mesh.m_vertexType == PHY_FLOAT);
  assert(mesh.m_triangleIndexStride == 4*3);
  assert(mesh.m_vertexStride == 4*4);

  btTriangleIndexVertexArray* triangles = new btTriangleIndexVertexArray;
  triangles->addIndexedMesh(mesh);

  btBvhTriangleMeshShape * shape = 0;
  btBulletWorldImporter import(0);
  if (import.loadFile(filename.c_str())) {
    if (import.getNumBvhs() == 1) {
      Log::info("Loading optimized bvh from %s", filename.c_str());
      shape = new btBvhTriangleMeshShape(triangles, false, false);
      shape->setOptimizedBvh(import.getBvhByIndex(0));
    }
  }

  if (!shape) {
    Log::info("Failed to load optimized bvh from disk, creating it");
    FILE* fp = fopen(filename.c_str(), "w");
    shape = new btBvhTriangleMeshShape(triangles, true);
    if (fp) {
      Log::info("Serializing optimized bvh to %s", filename.c_str());
      btDefaultSerializer* serializer = new btDefaultSerializer(50*1024*1024);
      serializer->startSerialization();
      shape->serializeSingleBvh(serializer);
      serializer->finishSerialization();
      fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, fp);
      fclose(fp);
    }
  }

  btRigidBody * body = new btRigidBody(0, new btDefaultMotionState(), shape);
  m_world->addRigidBody(body);
}

void Physics::addRigidBody(btRigidBody *body) {
  m_world->addRigidBody(body);
}


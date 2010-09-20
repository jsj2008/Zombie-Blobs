#include "Block.hpp"

Block::Block(btDiscreteDynamicsWorld &dynamicsWorld, float x, float y, float z, float width, float height, float depth) :
    world(dynamicsWorld),
    w(width),
    h(height),
    d(depth) {

    btVector3 fallInertia(0,0,0);
    float mass = 10.0f * width*height*depth;
    static btCollisionShape* blockShape = 0;
    if(!blockShape) {
      blockShape = new btBoxShape(btVector3(width/2.0f, height/2.0f, depth/2.0f));

    }
    blockShape->calculateLocalInertia(mass,fallInertia);

    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(x,y+height/2.0f,z)));
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,blockShape,fallInertia);
    fallRigidBodyCI.m_friction = 1.0f;
    Block::body = new btRigidBody(fallRigidBodyCI);
    body->setActivationState(ISLAND_SLEEPING);
    body->setDeactivationTime(1.0f);
    world.addRigidBody(body);
}

#include "Block.hpp"

Block::Block(btDiscreteDynamicsWorld &dynamicsWorld, float x, float y, float z, float width, float height, float depth, float mass) :
    world(dynamicsWorld),
    w(width),
    h(height),
    d(depth) {

    btCollisionShape* blockShape = new btBoxShape(btVector3(width/2.0f, height/2.0f, depth/2.0f));
    // Come up with a mass..
    if (mass <= 0) mass = 10.0f * width*height*depth;

    btVector3 fallInertia(0,0,0);
    blockShape->calculateLocalInertia(mass,fallInertia);
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(x,y+height/2.0f,z)));
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,blockShape,fallInertia);
    Block::body = new btRigidBody(fallRigidBodyCI);
    world.addRigidBody(body);
}

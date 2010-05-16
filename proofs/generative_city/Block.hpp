#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <btBulletDynamicsCommon.h>


class Block {

public:
    Block(btDiscreteDynamicsWorld &dynamicsWorld, float x, float y, float z, float width, float height, float depth, float mass=-1);
    btRigidBody* body;
    float w, h, d;

private:
    btDiscreteDynamicsWorld &world;

};


#endif // BLOCK_HPP_INCLUDED

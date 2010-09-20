#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <btBulletDynamicsCommon.h>


class Block {

public:
    Block(btDiscreteDynamicsWorld &dynamicsWorld, int i, int h, int j, float width, float height, float depth);
    btRigidBody* body;
    float w, h, d;
    int x, y, z;

private:
    btDiscreteDynamicsWorld &world;

};


#endif // BLOCK_HPP_INCLUDED

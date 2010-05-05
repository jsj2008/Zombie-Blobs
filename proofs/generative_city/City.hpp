#ifndef CITY_CPP_INCLUDED
#define CITY_CPP_INCLUDED

#include <vector>
#include <btBulletDynamicsCommon.h>

#include "Block.hpp"

#define CITY_W 10
#define CITY_H 7

class City {

public:
    City( btDiscreteDynamicsWorld &dynamicsWorld );
    int getHeight(int x, int y);
    void Generate(void);
    std::vector<Block*> blocks;


private:
    int hmap[CITY_W][CITY_H];
    btDiscreteDynamicsWorld &world;

};

#endif // CITY_CPP_INCLUDED

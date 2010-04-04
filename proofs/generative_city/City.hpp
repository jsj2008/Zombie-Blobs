#ifndef CITY_CPP_INCLUDED
#define CITY_CPP_INCLUDED

#include <vector>
#include "Block.hpp"

#define CITY_W 200
#define CITY_H 200

class City
{

public:
    City( void );
    int getHeight(int x, int y);
    void Generate( );


private:
    int hmap[CITY_W][CITY_H];
    std::vector<Block> blocks;

};

#endif // CITY_CPP_INCLUDED

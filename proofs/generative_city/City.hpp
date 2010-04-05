#ifndef CITY_CPP_INCLUDED
#define CITY_CPP_INCLUDED

#include <vector>
#include "Block.hpp"

#define CITY_W 150
#define CITY_H 150

class City {

public:
    City(void);
    int getHeight(int x, int y);
    void Generate(void);


private:
    int hmap[CITY_W][CITY_H];
    std::vector<Block> blocks;

};

#endif // CITY_CPP_INCLUDED

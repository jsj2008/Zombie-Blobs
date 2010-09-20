#include "City.hpp"

#include <cstdlib>
#include <map>

City::City( btDiscreteDynamicsWorld &dynamicsWorld ) : world(dynamicsWorld) {
    Generate();
}

int ckey(int i, int j, int h) {
  return CITY_W*CITY_H*h + CITY_W * j + i;
}

/**
 * Creates a randomly generated city.
 */
void City::Generate(void) {
    for(int i = 0; i < CITY_W; i++) {
        for(int j = 0; j < CITY_H; j++) {
            hmap[i][j] = 1;
            if(i == 0 || j == 0 || i == CITY_W-1 || j == CITY_H-1) {
                hmap[i][j] = 0;
            }
        }
    }

    // Subdivide the place with streets (0 = a tile of street)
    int max_divisions = CITY_W*CITY_H/10;
    int divisions = 0;
    int min_spread = 2;

    int max_tries = CITY_W*CITY_H;

    while(max_tries > 0 && divisions < max_divisions) {

        int divx = rand()%CITY_W;
        int divy = rand()%CITY_H;

        // let's see if the nearest tiles are not street already
        int street_check = 1;


        for(int i = divx - min_spread; i < divx + min_spread; i++) {
            for(int j = divy - min_spread; j < divy + min_spread; j++) {
                if(i >= 0 && j >= 0 && i < CITY_W && j < CITY_H) {
                    street_check *= hmap[i][j];
                }
            }
        }

        if(street_check == 0) {
            max_tries--;
            continue;
        }


        for(int dir = 0; dir < 4; dir ++) {
            int cx = divx;
            int cy = divy;

            int mxr = CITY_W > CITY_H ? CITY_W : CITY_H;

            while(mxr > 0) {
                // bumped into a street or border?
                if(cx < 0 || cy < 0 || cx >= CITY_W || cy >= CITY_H
                    || ( hmap[cx][cy] == 0 && (cx != divx || cy != divy) )) {
                    break;
                }

                // mark the tile as street
                hmap[cx][cy] = 0;

                // step accordingly (too haxy?)
                cx += (dir+1)%2 * (1 - int(dir + .5)%3);
                cy += (dir+2)%2 * (1 - int(dir + 2.5)%3);

                mxr --;
            }
        }
        divisions++;
        max_tries--;
    }

    // Come up with some heights for the remaining buildings between streets

    for(int i = 0; i < CITY_W; i++) {
        for(int j = 0; j < CITY_H; j++) {

            // Select a random height for the building, min 2
            int height = 1+rand()%12;

            int si, sj;
            for(si = i; si < CITY_W; si++) {
                for(sj = j; sj < CITY_H; sj++) {
                    if(hmap[si][sj] != 1) {
                        break;
                    }
                    hmap[si][sj] = height;
                }
            }
        }
    }

    std::map<int, Block*> cache;

    // Make bloxxx out of the height data!
    for(int i = 0; i < CITY_W; i++) {
      for(int j = 0; j < CITY_H; j++) {
        for(int h = 0; h < getHeight(i,j); h++) {
          Block * block = new Block(world, i, h, j, 1, 1, 1);
          cache[ckey(i, j, h)] = block;
          blocks.push_back(block);
          if (h == 0) {
            btPoint2PointConstraint * c = new btPoint2PointConstraint(
                  *block->body, btVector3(0, -0.5, 0));
            block->body->addConstraintRef(c);
            world.addConstraint(c, false);
          } else if (h > 0) {
            Block *bottom = *(blocks.end() - 2);
            btQuaternion a;
            a.setRotation(btVector3(0, 0, 1), 3.14159265358979/2);
            btSliderConstraint * c = new btSliderConstraint(
                  *bottom->body, *block->body,
                  btTransform(a, btVector3(0, 0, 0)),
                  btTransform(a, btVector3(0, 0, 0)), true);
            c->setLowerLinLimit(1);
            c->setUpperLinLimit(1);
            block->body->addConstraintRef(c);
            world.addConstraint(c, true);
          }

          Block* left = cache[ckey(i-1, j, h)];
          Block* back = cache[ckey(i, j-1, h)];
          if(left) {
            btQuaternion a(0, 0, 0, 1);
            btSliderConstraint * c = new btSliderConstraint(
                  *left->body, *block->body,
                  btTransform::getIdentity(), btTransform::getIdentity(),
                  true);
            c->setLowerLinLimit(1);
            c->setUpperLinLimit(1);
            block->body->addConstraintRef(c);
            world.addConstraint(c, true);
          }
          if(back) {
            btQuaternion a;
            a.setRotation(btVector3(0, 1, 0), 3.14159265358979/2);
            btSliderConstraint * c = new btSliderConstraint(
                  *block->body, *back->body,
                  btTransform(a, btVector3(0, 0, 0)),
                  btTransform(a, btVector3(0, 0, 0)), true);
            c->setLowerLinLimit(1);
            c->setUpperLinLimit(1);
            block->body->addConstraintRef(c);
            world.addConstraint(c, true);
          }
        }
      }
    }

}

/**
 * A simple function for getting the "height" data for a given x/y coordinate
 * (building height at the moment)
 */
int City::getHeight(int x, int y) {
    if(x < 0 || y < 0 || x > CITY_W || y > CITY_H) {
        return 0;
    }

    return hmap[x][y];
}

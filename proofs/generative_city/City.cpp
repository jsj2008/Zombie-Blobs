#include "City.hpp"

#include <cstdlib>

City::City( void ) {
    Generate();
}

void City::Generate( )
{
    for( int i = 0; i < CITY_W; i++ )
    {
        for( int j = 0; j < CITY_H; j++ )
        {
            hmap[i][j] = 1;
            if( i == 0 || j == 0 || i == CITY_W-1 || j == CITY_H-1 )
            {
                hmap[i][j] = 0;
            }
        }
    }

    // Subdivide the place with streets (0 = a tile of street)
    int max_divisions = CITY_W*CITY_H/1000;
    int divisions = 0;
    int min_spread = 10;

    int max_tries = CITY_W*CITY_H;

    while( max_tries > 0 && divisions < max_divisions )
    {

        int divx = rand()%CITY_W;
        int divy = rand()%CITY_H;

        // let's see if the nearest tiles are not street already
        int street_check = 1;


        for( int i = divx - min_spread; i < divx + min_spread; i++ )
        {
            for( int j = divy - min_spread; j < divy + min_spread; j++ )
            {
                if( i >= 0 && j >= 0 && i < CITY_W && j < CITY_H )
                {
                    street_check *= hmap[i][j];
                }
            }
        }

        if( street_check == 0 )
        {
            max_tries--;
            continue;
        }


        for ( int dir = 0; dir < 4; dir ++ )
        {
            int cx = divx;
            int cy = divy;

            int mxr = CITY_W > CITY_H ? CITY_W : CITY_H;

            while( mxr > 0 )
            {
                // bumped into a street or border?
                if( cx < 0 || cy < 0 || cx >= CITY_W || cy >= CITY_H  || ( hmap[cx][cy] == 0 && (cx != divx || cy != divy) ) )
                {
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

    for( int i = 0; i < CITY_W; i++ )
    {
        for( int j = 0; j < CITY_H; j++ )
        {
            int height = 2+rand()%10; // Give a random height for the building, min 2
            int si, sj;
            for( si = i; si < CITY_W; si++ )
            {
                for( sj = j; sj < CITY_H; sj++ )
                {
                    if( hmap[si][sj] != 1 )
                    {
                        break;
                    }
                    hmap[si][sj] = height;
                }
            }
        }
    }


}

int City::getHeight( int x, int y )
{
    if( x < 0 || y < 0 || x > CITY_W || y > CITY_H )
    {
        return 0;
    }

    return hmap[x][y];
}

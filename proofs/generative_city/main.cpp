#include <cstdlib>

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "City.hpp"


void putpixel( SDL_Surface * surface, int x, int y, int r, int g, int b )
{
    if( x >= 0 && y >= 0 && x < surface->w && y < surface->h )
    {
        *(Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch + (x << 2)) = SDL_MapRGB( surface->format, r, g, b );
    }
}

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(800, 600,32,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }
    srand(SDL_GetTicks());

    City *city = new City();

    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            } // end switch
        } // end of message processing

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 64, 32, 24));

        for( int i = 0; i < CITY_W; i++ )
        {
            for( int j = 0; j < CITY_H; j++ )
            {
                int h = city->getHeight(i,j)*15;
                h == 0 ? h = 255 : 0;
                putpixel(screen, 10+i, 10+j, h, h, h);
            }
        }

        // finally, update the screen :)
        SDL_Flip(screen);
    } // end main loop

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}

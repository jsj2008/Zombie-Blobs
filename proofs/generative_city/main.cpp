#include <cstdlib>

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "City.hpp"


void putpixel(SDL_Surface * surface, int x, int y, int r, int g, int b) {
    if(x >= 0 && y >= 0 && x < surface->w && y < surface->h) {
        *(Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch + (x << 2))
            = SDL_MapRGB( surface->format, r, g, b );
    }
}

int main(int argc, char** argv) {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        return 1;
    }

    atexit(SDL_Quit);

    SDL_Surface* screen = SDL_SetVideoMode(800, 600,32,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if(!screen) {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // seed rand (should be done better but this will do for now)
    srand(SDL_GetTicks());

    // Initialize the generated city
    City *city = new City();

    // Program main loop
    bool done = false;
    while(!done) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            }
        }

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 64, 32, 24));

        for(int i = 0; i < CITY_W; i++) {
            for(int j = 0; j < CITY_H; j++) {
                int h = city->getHeight(i,j)*15;
                h == 0 ? h = 255 : 0;
                putpixel(screen, 10+i, 10+j, h, h, h);
            }
        }

        SDL_Flip(screen);
    } // end main loop

    printf("Exited cleanly\n");
    return 0;
}

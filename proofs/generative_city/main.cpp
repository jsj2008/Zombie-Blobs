#include <cstdlib>

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "City.hpp"

City *city;


void putpixel(SDL_Surface * surface, int x, int y, int r, int g, int b) {
    if(x >= 0 && y >= 0 && x < surface->w && y < surface->h) {
        *(Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch + (x << 2))
            = SDL_MapRGB( surface->format, r, g, b );
    }
}

void ReshapeGL(int width, int height) {
	glViewport(0,0,(GLsizei)(width),(GLsizei)(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)(width)/(GLfloat)(height),1.0f,350.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


bool InitGL(SDL_Surface *S)
{
	glClearColor(0,0,0,0);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	return true;
}

void Draw2D(SDL_Surface *S) {

    // clear screen
    for(int i = 0; i < CITY_W; i++) {
        for(int j = 0; j < CITY_H; j++) {
            int h = city->getHeight(i,j)*15;
            h == 0 ? h = 255 : 0;
            putpixel(S, 10+i, 10+j, h, h, h);
        }
    }


}

void DrawBox(float w, float h, float d) {

    glBegin(GL_QUADS);

        glNormal3f(0,0,1);

        glVertex3f(-w/2.0f,0,d/2.0f);
        glVertex3f(-w/2.0f,h,d/2.0f);
        glVertex3f(w/2.0f,h,d/2.0f);
        glVertex3f(w/2.0f,0,d/2.0f);

        glNormal3f(0,0,-1);

        glVertex3f(w/2.0f,0,-d/2.0f);
        glVertex3f(w/2.0f,h,-d/2.0f);
        glVertex3f(-w/2.0f,h,-d/2.0f);
        glVertex3f(-w/2.0f,0,-d/2.0f);

        glNormal3f(-1,0,0);

        glVertex3f(-w/2.0f,0,-d/2.0f);
        glVertex3f(-w/2.0f,h,-d/2.0f);
        glVertex3f(-w/2.0f,h,d/2.0f);
        glVertex3f(-w/2.0f,0,d/2.0f);

        glNormal3f(1,0,0);

        glVertex3f(w/2.0f,0,-d/2.0f);
        glVertex3f(w/2.0f,0,d/2.0f);
        glVertex3f(w/2.0f,h,d/2.0f);
        glVertex3f(w/2.0f,h,-d/2.0f);

        glNormal3f(0,1,0);

        glVertex3f(w/2.0f,h,d/2.0f);
        glVertex3f(-w/2.0f,h,d/2.0f);
        glVertex3f(-w/2.0f,h,-d/2.0f);
        glVertex3f(w/2.0f,h,-d/2.0f);

    glEnd();
}

void Draw3D(SDL_Surface *S) {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

    // Move cam to some nice position
    glTranslatef(-CITY_W/2,0,-CITY_W*1.5);
    glRotatef(25,1,0,0);
    glRotatef(45,0,1,0);

    // Draw the city
    for(int i = 0; i < CITY_W; i++) {
        for(int j = 0; j < CITY_H; j++) {
            glPushMatrix();
                float h = city->getHeight(j,j);
                glColor3f(h, h, h);
                glTranslatef(i,0,j);
                DrawBox(1,city->getHeight(i,j),1);
            glPopMatrix();
        }
    }


	glFlush();
}

void Draw(SDL_Surface *Screen) {
	assert(Screen);
	Draw3D(Screen);
	Draw2D(Screen);
}

int main(int argc, char** argv) {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        return 1;
    }

    atexit(SDL_Quit);

    SDL_Surface* screen = SDL_SetVideoMode(800, 600,32,
                                           SDL_HWSURFACE|SDL_OPENGLBLIT);
    if(!screen) {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // seed rand (should be done better but this will do for now)
    srand(SDL_GetTicks());

    InitGL(screen);

    ReshapeGL(screen->w, screen->h);


    // Initialize the generated city
    city = new City();

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
                    if (event.key.keysym.sym == SDLK_RETURN)
                        city->Generate();
                    break;
                }
            }
        }

        Draw(screen);
        SDL_GL_SwapBuffers();

    } // end main loop

    printf("Exited cleanly\n");
    return 0;
}

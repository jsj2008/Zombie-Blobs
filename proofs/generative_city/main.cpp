#include <cstdlib>
#include <cassert>

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "City.hpp"

#include <btBulletDynamicsCommon.h>

#define HCpPI 57.295779513082320876798154814105

City *city;

float sphere_y = 0;
float sphere_x = 0;
float sphere_z = 0;


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
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

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

        glVertex3f(-w/2.0f,-h/2.0f,d/2.0f);
        glVertex3f(-w/2.0f,h/2.0f,d/2.0f);
        glVertex3f(w/2.0f,h/2.0f,d/2.0f);
        glVertex3f(w/2.0f,-h/2.0f,d/2.0f);

        glNormal3f(0,0,-1);

        glVertex3f(w/2.0f,-h/2.0f,-d/2.0f);
        glVertex3f(w/2.0f,h/2.0f,-d/2.0f);
        glVertex3f(-w/2.0f,h/2.0f,-d/2.0f);
        glVertex3f(-w/2.0f,-h/2.0f,-d/2.0f);

        glNormal3f(-1,0,0);

        glVertex3f(-w/2.0f,-h/2.0f,-d/2.0f);
        glVertex3f(-w/2.0f,h/2.0f,-d/2.0f);
        glVertex3f(-w/2.0f,h/2.0f,d/2.0f);
        glVertex3f(-w/2.0f,-h/2.0f,d/2.0f);

        glNormal3f(1,0,0);

        glVertex3f(w/2.0f,-h/2.0f,-d/2.0f);
        glVertex3f(w/2.0f,-h/2.0f,d/2.0f);
        glVertex3f(w/2.0f,h/2.0f,d/2.0f);
        glVertex3f(w/2.0f,h/2.0f,-d/2.0f);

        glNormal3f(0,1,0);

        glVertex3f(w/2.0f,h/2.0f,d/2.0f);
        glVertex3f(-w/2.0f,h/2.0f,d/2.0f);
        glVertex3f(-w/2.0f,h/2.0f,-d/2.0f);
        glVertex3f(w/2.0f,h/2.0f,-d/2.0f);

        glNormal3f(0,-1,0);

        glVertex3f(-w/2.0f,-h/2.0f,d/2.0f);
        glVertex3f(w/2.0f,-h/2.0f,d/2.0f);
        glVertex3f(w/2.0f,-h/2.0f,-d/2.0f);
        glVertex3f(-w/2.0f,-h/2.0f,-d/2.0f);

    glEnd();
}

void Draw3D(SDL_Surface *S) {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

    // Move cam to some nice position
    glTranslatef(-CITY_W/2,3,-CITY_W*CITY_W*0.75f);
    glRotatef(25,1,0,0);
//    glRotatef(25,1,0,0);
    glRotatef(45,0,1,0);

    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(sphere_x,sphere_y,sphere_z);
    DrawBox(1,1,1);
    glPopMatrix();

    // Draw the city

    for (unsigned int i=0; i<city->blocks.size(); i++) {
        glPushMatrix();

            btTransform trans;
            city->blocks[i]->body->getMotionState()->getWorldTransform(trans);
            btScalar m[16];
            trans.getOpenGLMatrix(m);
            glMultMatrixf(m);

            glColor3f(0.2f+city->blocks[i]->h*0.02f, 0.2f, 0.2f);

            DrawBox(city->blocks[i]->w,city->blocks[i]->h,city->blocks[i]->d);
        glPopMatrix();
   }
/*
    for(int i = 0; i < CITY_W; i++) {
        for(int j = 0; j < CITY_H; j++) {
            glPushMatrix();
                glColor3f(0.2f, 0.2f, 0.2f);
                glTranslatef(i,city->getHeight(i,j)/2.0f,j);
                DrawBox(1,city->getHeight(i,j),1);
            glPopMatrix();
        }
    }
*/
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


    // INIT BULLET
    // Build the broadphase
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // The actual physics solver
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    // The world.
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    btCollisionShape* fallShape = new btSphereShape(1);

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
        groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);


    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(3,25,3)));
    btScalar mass = 5000;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(fallRigidBody);


    // Initialize the generated city
    city = new City(*dynamicsWorld);

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
                    if (event.key.keysym.sym == SDLK_SPACE)
                        fallRigidBody->setMotionState(new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(3,25,3))));

                    break;
                }
            }
        }
        dynamicsWorld->stepSimulation(1/60.f,15);

        btTransform trans;
        fallRigidBody->getMotionState()->getWorldTransform(trans);
        sphere_y = trans.getOrigin().getY();
        sphere_z = trans.getOrigin().getZ();
        sphere_x = trans.getOrigin().getX();
        Draw(screen);
        SDL_GL_SwapBuffers();

    } // end main loop


    // Clean up behind ourselves like good little programmers
    dynamicsWorld->removeRigidBody(fallRigidBody);
    delete fallRigidBody->getMotionState();
    delete fallRigidBody;

    dynamicsWorld->removeRigidBody(groundRigidBody);
    delete groundRigidBody->getMotionState();
    delete groundRigidBody;


    delete fallShape;

    delete groundShape;


    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;



    printf("Exited cleanly\n");
    return 0;
}

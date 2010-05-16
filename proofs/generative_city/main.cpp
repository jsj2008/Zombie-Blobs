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

typedef std::vector<Block*> ProjectileList;
ProjectileList projectiles;

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

  gluPerspective(45.0f,(GLfloat)(width)/(GLfloat)(height), 0.1f,350.0f);

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

void unitSphere(int slices) {
	double theta, st, ct;
	double theta_ = -M_PI_2;
	double st_ = sin(theta_);
	double ct_ = cos(theta_);

	for (int i=0; i <= slices; ++i) {
		double theta = M_PI * (-0.5 + (i-1)/(double)slices);
		theta = theta_;
		st = st_;
		ct = ct_;
		theta_ = M_PI * (-0.5 + i/(double)slices);
		st_ = sin(theta_);
		ct_ = cos(theta_);

		glBegin(GL_QUAD_STRIP);
		for (int j=0; j <= slices; ++j) {
			double phi = 2 * M_PI * ((j-1)/(double)slices);
			double cp = cos(phi);
			double sp = sin(phi);
			double x = cp*ct;
			double y = sp*ct;
			double z = st;

			glNormal3f(x, y, z);
			glVertex3f(x, y, z);

			x = cp*ct_;
			y = sp*ct_;
			z = st_;
			
			glNormal3f(x, y, z);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void Draw3D(SDL_Surface *S) {
  // Move cam to some nice position
  // glTranslatef(-CITY_W/2,3,-CITY_W*CITY_W*0.75f);
  //glRotatef(25,1,0,0);
  //    glRotatef(25,1,0,0);
  //glRotatef(45,0,1,0);

  glPushMatrix();
  glColor3f(1,0,0);
  glTranslatef(sphere_x,sphere_y,sphere_z);
	unitSphere(40); 
  //    DrawBox(1,1,1);
  glPopMatrix();


  glColor3f(0.4, 0.9, 0.4);
  glPushMatrix();
  glScalef(50, 50, 50);
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3f(-1,  0,   1);
  glVertex3f(-1,  0,  -1);
  glVertex3f( 1,  0,   1);
  glVertex3f( 1,  0,  -1);
  glEnd();
  glPopMatrix();
  // Draw the city

  btTransform trans;
  for (unsigned int i=0; i<city->blocks.size(); i++) {
    glPushMatrix();

    city->blocks[i]->body->getMotionState()->getWorldTransform(trans);
    btScalar m[16];
    trans.getOpenGLMatrix(m);
    glMultMatrixf(m);

    glColor3f(0.2f+city->blocks[i]->h*0.02f, 0.2f, 0.2f);

    DrawBox(city->blocks[i]->w,city->blocks[i]->h,city->blocks[i]->d);
    glPopMatrix();
  }

  for (ProjectileList::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
    glPushMatrix();

    (**it).body->getMotionState()->getWorldTransform(trans);
    btScalar m[16];
    trans.getOpenGLMatrix(m);
    glMultMatrixf(m);

    glColor3f(0.8, 0.4, 0.4);
    DrawBox((**it).w, (**it).h, (**it).d);
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

  int width = 1024;
  int height = 768;

  SDL_Surface* screen = SDL_SetVideoMode(width, height, 32,
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

	float planes[] = {
		0, 1, 0, 0,
		1, 0, 0, 50,
		-1, 0, 0, 50,
		0, 0, 1, 50,
		0, 0, -1, 50
	};
	for (int i=0; i < 1; ++i) {
		// FIXME: törmäystasot planes[1..4] toimivat "väärinpäin"
		const float* plane = &planes[i*4];
  	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(plane[0], plane[1], plane[2]), plane[3]);
  	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
		btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
		btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
		dynamicsWorld->addRigidBody(groundRigidBody);
	}
  btCollisionShape* fallShape = new btSphereShape(1);

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

  Uint32 old_ticks = 0;
  Uint32 ticks;
  SDL_WM_GrabInput(SDL_GRAB_ON);
  SDL_ShowCursor(SDL_DISABLE);

  btVector3 eye(-CITY_W/2, 0.4, -CITY_W*CITY_W*.25f);
  double theta = 0.0;
	double phi = 0.0;

  int forward = 0;
  int sideways = 0;
  while(!done) {
    SDL_WarpMouse(width/2, height/2);
    ticks = SDL_GetTicks();
    float t = (ticks - old_ticks)/1000.0f;
    bool fire = false;

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_QUIT:
        done = true;
        break;
      case SDL_KEYDOWN:
        {
          switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            done = true;
            break;
          case SDLK_RETURN:
            city->Generate();
            break;
          case SDLK_SPACE:
            fallRigidBody->setMotionState(new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(3,25,3))));
            break;
          case SDLK_w:
            forward = 1;
            break;
          case SDLK_a:
            sideways = -1;
            break;
          case SDLK_s:
            forward = -1;
            break;
          case SDLK_d:
            sideways = 1;
            break;
          }
          break;
        }
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_w:
          if (forward == 1) forward = 0;
          break;
        case SDLK_a:
          if (sideways == -1) sideways = 0;
          break;
        case SDLK_s:
          if (forward == -1) forward = 0;
          break;
        case SDLK_d:
          if (sideways == 1) sideways = 0;
          break;
        }
        break;
								case SDL_MOUSEBUTTONDOWN:
        fire = true;
        break;
								case SDL_MOUSEMOTION:
        int x = event.motion.x;
        int y = event.motion.y;
        int dx = width/2 - x;
        int dy = height/2 - y;
        if (dx != 0 || dy != 0) {
          theta += dx/1000.0f;
          phi -= dy/1000.0f;
					phi = std::max(std::min(phi, M_PI_2), -M_PI_2);
        }
        break;
      }
    }
    btVector3 lookDir(0, 0, 1);
    lookDir = lookDir.rotate(btVector3(1, 0, 0), phi).rotate(btVector3(0, 1, 0), theta).normalize();

    const float move_per_sec = 5.0f;
    btVector3 mover(lookDir.x(), 0, lookDir.z());
    mover.normalize();
    btVector3 tmp((forward * mover) + (sideways * mover.rotate(btVector3(0, 1, 0), -M_PI_2)));
    if (tmp.length() > 1)
      tmp.normalize();
    eye += move_per_sec * t * tmp;

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye.x(), eye.y(), eye.z(),
              eye.x() + lookDir.x(),
              eye.y() + lookDir.y(),
              eye.z() + lookDir.z(),
              0, 1, 0);

    dynamicsWorld->stepSimulation(t,5);

    btTransform trans;
    fallRigidBody->getMotionState()->getWorldTransform(trans);
    sphere_y = trans.getOrigin().getY();
    sphere_z = trans.getOrigin().getZ();
    sphere_x = trans.getOrigin().getX();
    Draw(screen);

    if (fire) {
      Block * block = new Block(*dynamicsWorld,
                                eye.x(),
                                eye.y(),
                                eye.z(),
                                0.3f, 0.3f, 0.3f,
                                50.0f
                                );
      block->body->applyCentralImpulse(1000.0f*lookDir);
      projectiles.push_back(block);
    }
    SDL_GL_SwapBuffers();
    old_ticks = ticks;
  } // end main loop


  // Clean up behind ourselves like good little programmers
  dynamicsWorld->removeRigidBody(fallRigidBody);
  delete fallRigidBody->getMotionState();
  delete fallRigidBody;

	/* don't bother cleaning up
  dynamicsWorld->removeRigidBody(groundRigidBody);
  delete groundRigidBody->getMotionState();
  delete groundRigidBody;
	*/

  delete fallShape;

  //delete groundShape;


  delete dynamicsWorld;
  delete solver;
  delete collisionConfiguration;
  delete dispatcher;
  delete broadphase;



  printf("Exited cleanly\n");
  return 0;
}

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
#include <LinearMath/btConvexHull.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBody.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <iostream>
#include <typeinfo>

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





struct Projectile {
    //btSoftBody * body;
    btRigidBody * body2;
    float kaboom;
    btVector3 point;

    Projectile(btSoftRigidDynamicsWorld * world, btVector3 pos, btVector3 dir, btScalar mass, btScalar impulse)
      : body2(0), kaboom(-1.0f), point(0, 0, 0) {
/*
      body = projectile(world, 150);
      //body->setMass(0, 0.0f);
      body->m_cfg.piterations = 2;
      body->generateBendingConstraints(3);
      body->generateClusters(64);
      // softbody-softbody
      body->m_cfg.collisions |= btSoftBody::fCollision::CL_SS;
      // rigidbody-softbody
      //body->m_cfg.collisions |= btSoftBody::fCollision::CL_RS;

      // tässä välissä voisi säätää body->m_cfg.k*, kts. bulletin sorsat
      body->randomizeConstraints();

      body->setTotalMass(mass, false);

      btMatrix3x3 m;
      // orientaatiolla ei sinänsä väliä jos objektina on pallo,
      // pistetään kuitenkin
      m.setEulerZYX(dir.x(), dir.y(), dir.z());
      body->scale(btVector3(.5, .5, .5));
      body->transform(btTransform(m, pos));
      body->setVelocity(dir * impulse);
      body->addForce(btVector3(0, 1, 0) * impulse/4.0f);
      //body->getCollisionShape()->setMargin(0.1f);

      body->getCollisionShape()->setMargin(0.1);

      ghost = new btPairCachingGhostObject;
      ghost->setWorldTransform(body->getWorldTransform());
      ghost->setInterpolationLinearVelocity(body->getInterpolationLinearVelocity());
      ghost->setCollisionShape(new btSphereShape(0.5));
      //ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
      //world->addCollisionObject(ghost);
      world->addSoftBody(body);
*/

      btMatrix3x3 m;
      // orientaatiolla ei sinänsä väliä jos objektina on pallo,
      // pistetään kuitenkin
      m.setEulerZYX(dir.x(), dir.y(), dir.z());

      btDefaultMotionState* ms = new btDefaultMotionState(btTransform(m, pos));
      btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, ms, new btSphereShape(0.5f));
      body2 = new btRigidBody(fallRigidBodyCI);
      body2->applyCentralImpulse(dir * impulse);
      body2->applyCentralForce(btVector3(0, 1, 0) * impulse/4.0f);
      world->addRigidBody(body2);
    }

    void update() {
/*      ghost->setWorldTransform(body->getWorldTransform());
      //ghost->setInterpolationLinearVelocity(body->getInterpolationLinearVelocity());
      btBroadphasePairArray& pairArray = ghost->getOverlappingPairCache()->getOverlappingPairArray();
      int numPairs = pairArray.size();
*/
      //std::cout << numPairs << std::endl;
    }

    btSoftBody * projectile(btSoftRigidDynamicsWorld * world, int resolution) {
      return btSoftBodyHelpers::CreateEllipsoid(world->getWorldInfo(), btVector3(0,0,0), btVector3(1,1,1), resolution);

      double theta, st, ct;

      int n = (resolution+1) * (resolution+1);
      btVector3 * points = new btVector3[n];
      int p = 0;
      for (int i=0; i <= resolution; ++i) {
        theta = M_PI * (-0.5 + i/(double)resolution);
        st = sin(theta);
        ct = cos(theta);

        for (int j=0; j <= resolution; ++j) {
          double phi = 2 * M_PI * ((j-1)/(double)resolution);
          double cp = cos(phi);
          double sp = sin(phi);
          double x = cp*ct;
          double y = sp*ct;
          double z = st;
          points[p++] = btVector3(x, y, z);
        }
      }
      /*
        HullLibrary hl;
        HullDesc desc(QF_TRIANGLES, n, points, 0);
        HullResult res;
        hl.CreateConvexHull(desc, res);
        */

      btSoftBody * body = btSoftBodyHelpers::CreateFromConvexHull(world->getWorldInfo(), points, n);
      delete[] points;
      return body;
    }

};

typedef std::vector<Projectile*> ProjectileList;
ProjectileList projectiles;

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

    btTransform & trans = (**it).body2->getWorldTransform();
    btScalar m[16];
    trans.getOpenGLMatrix(m);
    glMultMatrixf(m);
#if 0
    btSoftBody* softbody = (**it).body;
    /* Each soft body contain an array of vertices (nodes/particles_mass)   */
    btSoftBody::tNodeArray& nodes(softbody->m_nodes);
    /* And edges (links/distances constraints)                        */
    btSoftBody::tLinkArray& links(softbody->m_links);
    /* And finally, faces (triangles)                                 */
    btSoftBody::tFaceArray& faces(softbody->m_faces);

    //glDisable(GL_LIGHTING);
    glColor3f(0.8, 0.4, 0.4);
    glBegin(GL_TRIANGLES);

    // output vertices in CCW order
    for (int j=0; j < faces.size(); j++) {
      glNormal3fv(faces[j].m_n[0]->m_n);
      glVertex3fv(faces[j].m_n[0]->m_x);

      glNormal3fv(faces[j].m_n[2]->m_n);
      glVertex3fv(faces[j].m_n[2]->m_x);

      glNormal3fv(faces[j].m_n[1]->m_n);
      glVertex3fv(faces[j].m_n[1]->m_x);
    }
    glEnd();
#endif
    //glEnable(GL_LIGHTING);

    glColor3f(0.8, 0.4, 0.4);
		glScalef(0.5f, 0.5f, 0.5f);
		unitSphere(10);

    //DrawBox((**it).w, (**it).h, (**it).d);
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

struct ExplosionCb : public btBroadphaseAabbCallback
{
  ExplosionCb(btRigidBody * b, const btVector3 & v, float d, float s) : obody(b), center(v), distance(d), step(s) {}
  const btRigidBody * obody;
  const btVector3 center;
  const float distance, step;
  virtual ~ExplosionCb() {}
  bool process(const btBroadphaseProxy* proxy) {
    btRigidBody * body = static_cast<btRigidBody*>(proxy->m_clientObject);
    if(body == obody)
      return true;
    btVector3 d = body->getCenterOfMassPosition() - center;
    float tmp = d.length();
    if(tmp > distance) return true;

    d /= tmp;
    tmp = (distance - tmp) / distance;
    //body->applyCentralForce(d * (tmp * tmp * 10000.0f * (step - 0.3f)));
    body->applyCentralForce(d * (tmp * tmp * 3000.0f * (step > 0.4f ? 1.0f : -1.0f)));
    return true;
  }
};

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
    printf("Unable to set %d x %d video: %s\n", width, height, SDL_GetError());
    return 1;
  }

  // seed rand (should be done better but this will do for now)
  srand(argc == 2 ? atoi(argv[0]) : SDL_GetTicks());

  InitGL(screen);

  ReshapeGL(screen->w, screen->h);


  // INIT BULLET
  // Build the broadphase
  btBroadphaseInterface* broadphase = new btDbvtBroadphase();

  // Set up the collision configuration and dispatcher
  btCollisionConfiguration * collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);


  // The actual physics solver
  btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

  // The world.

  btSoftRigidDynamicsWorld * dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
  btSoftBodyWorldInfo & softWorldInfo = dynamicsWorld->getWorldInfo();
  softWorldInfo.m_gravity.setValue(0, -10, 0);
  softWorldInfo.m_sparsesdf.Initialize();
  dynamicsWorld->setGravity(btVector3(0,-10,0));


  btVector3 planeCenters[] = {{0,0,0},{-50,0,0},{0,0,50},{50,0,0},{0,0,-50}};
  btMatrix3x3 planeRotations[] = {
    {1,0,0, 0,1,0, 0,0,1},
    {0,1,0, 0,0,1, 1,0,0},
    {1,0,0, 0,0,1, 0,-1,0},
    {0,-1,0, 0,0,1, -1,0,0},
    {-1,0,0, 0,0,1, 0,1,0}};
  btCollisionShape* edge = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
  for (int i=0; i < 5; ++i) {
    btDefaultMotionState * ms = new btDefaultMotionState(btTransform(planeRotations[i], planeCenters[i]));
    btRigidBody * edgeRigidBody = new btRigidBody(0, ms, edge);
    dynamicsWorld->addRigidBody(edgeRigidBody);
  }
  btCollisionShape* fallShape = new btSphereShape(1);

  btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(3,25,3)));
  btScalar mass = 500;
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
//  SDL_ShowCursor(SDL_DISABLE);

  btVector3 eye(-CITY_W/2, 0.4, -CITY_W*CITY_W*.25f);
  double theta = 0.0;
	double phi = 0.0;

  int forward = 0;
  int sideways = 0;
  int up = 0;
  while(!done) {
    SDL_WarpMouse(width/2, height/2);
    ticks = SDL_GetTicks();
    float t = (ticks - old_ticks)/1000.0f;
    bool fire = false;

    softWorldInfo.m_sparsesdf.GarbageCollect();

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
          case SDLK_r:
            up = 1;
            break;
          case SDLK_f:
            up = -1;
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
        case SDLK_r:
          if (up == 1) up = 0;
          break;
        case SDLK_f:
          if (up == -1) up = 0;
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

    const float move_per_sec = 25.0f;
    btVector3 mover_forward(lookDir);
    btVector3 mover_sideways(-lookDir.z(), 0, lookDir.x());
    mover_forward.normalize();
    mover_sideways.normalize();
    btVector3 tmp((up * btVector3(0, 1, 0))+(forward * mover_forward) + (sideways * mover_sideways));
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

    static float t2;
    t2 = 0.0f;

    struct foo {
      static void cb(btDynamicsWorld *, btScalar timeStep) {
        t2 = timeStep;
      }
    };

    dynamicsWorld->setInternalTickCallback(&foo::cb);
    dynamicsWorld->stepSimulation(t,1);

    t = t2;

    /*for (ProjectileList::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
      (*it)->update();
    }*/

    int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i=0;i<numManifolds;i++)
    {
      btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
      btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
      btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

      ProjectileList::iterator p = projectiles.end();

      for (ProjectileList::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
        if ((*it)->kaboom >= 0) continue;
        if (static_cast<btCollisionObject*>((*it)->body2) == obA || static_cast<btCollisionObject*>((*it)->body2) == obB) {
          p = it;
          break;
        }
      }

      if (p == projectiles.end())
        continue;

      int numContacts = contactManifold->getNumContacts();
      for (int j=0;j<numContacts;j++)
      {
        btManifoldPoint& pt = contactManifold->getContactPoint(j);
        if (pt.getDistance()<0.f)
        {
          //const btVector3& point = pt.getPositionWorldOnB();
          (*p)->kaboom = 0.0f;
          (*p)->point = (*p)->body2->getCenterOfMassPosition();
          dynamicsWorld->removeRigidBody((*p)->body2);
          break;
        }
      }
    }

    for (int i = 0; i < projectiles.size(); ++i) {
      Projectile * p = projectiles[i];
      if (p->kaboom < 0) continue;
      btVector3 point = p->point;
      ExplosionCb explosionCb(p->body2, point, 10, p->kaboom > 1.0f ? 1.0f : p->kaboom);
      broadphase->aabbTest(point - btVector3(10, 10, 10), point + btVector3(10, 10, 10), explosionCb);
      if (p->kaboom >= 1.0f) {
        projectiles.erase(projectiles.begin() + i);
        delete p;
        --i;
      } else {
        p->kaboom += 5.0f * t;
      }
    }

    btTransform trans;
    fallRigidBody->getMotionState()->getWorldTransform(trans);
    sphere_y = trans.getOrigin().getY();
    sphere_z = trans.getOrigin().getZ();
    sphere_x = trans.getOrigin().getX();
    Draw(screen);

    if (fire) {
      Projectile * projectile = new Projectile(dynamicsWorld, eye, lookDir, 20.0f, 500.0f);

      if (projectiles.size() >= 5) {
        Projectile * p = projectiles.front();
        dynamicsWorld->removeRigidBody(p->body2);
        //dynamicsWorld->removeSoftBody(p->body);
        delete p->body2;
        delete p;
        projectiles.erase(projectiles.begin());
        projectiles.push_back(projectile);
      } else {
        projectiles.push_back(projectile);
      }

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

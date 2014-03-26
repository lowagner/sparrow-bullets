#ifndef __WORLD__
#define __WORLD__

//#include "objects.h"
#include "btBulletDynamicsCommon.h"
#include "csparrow.h"

class World
{

public:
    // biggest PHYSICS class
    // the general world which calculates everything, does dynamics, etc.
	btDynamicsWorld*		m_dworld;

private:
    SDL_Surface* checkertexture;

    // more physics classes

    // this collision shapes holds all the unique collision shapes.
    // so don't add to it unless you have a different shape to collide.
    // (it's probably ok, it's just less efficient.)
	btAlignedObjectArray<btCollisionShape*>	m_colshapes;

    // methods to calculate things...
	btBroadphaseInterface*	m_broadphase;

    // something to dispatch... collision events?
	btCollisionDispatcher*	m_dispatcher;

    // methods to calculate constraints
	btSequentialImpulseConstraintSolver*	m_solver;

    // not sure what this is
	btDefaultCollisionConfiguration* m_colconfig;
   
    // standard box shape
	btBoxShape* cubeshape;
    // standard box inertia -- or moment of inertia off-diagonals, not exactly sure!
    btVector3 cubeinertia;
	
// drawing methods, hidden to the metaworld.

    void draw_textured_box( Sint32 halfsize = SP_ONE, Uint16 color = 0xFFF );
    void draw_box( Sint32 halfsize = SP_ONE, Uint16 color = 0xFFF );
    
public:

    World();

    void
    update( float dt );

    btRigidBody*
    add_cube( float x=0, float y=0, float z=0 );

    btRigidBody*
    add_box( float size_x, float size_y, float size_z,
             float x=0, float y=0, float z=0, 
             float mass_=0.0f );

    void remove_body( btRigidBody* rb );

    void draw();

    ~World();
};

class Cube
{
private:
    int iamdone;
    Uint16 color;
    float lastx, lasty, lastz;
    btDynamicsWorld* m_dworld;
    btRigidBody* m_rb;

public:
    Cube( float x=0, float y=0, float z=0, 
          Uint16 color_=0xFFF );

    void remove();
    
    int done(); // am i done and can be removed?
    
    void get_position_orientation( btVector3& pos, btQuaternion& rot );

    void add_to_world( World& world );

    void remove_from_world();

    int crashed(); // did i crash in the last iteration

    ~Cube();
};




#endif

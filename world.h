#ifndef __WORLD__
#define __WORLD__

#include "basic.h"

class World
{

public:
    // biggest PHYSICS class
    // the general world which calculates everything, does dynamics, etc.
	btDynamicsWorld*		m_dworld;

private:
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

public:

    World();

    void
    update( float dt );

    btRigidBody*
    add_cube( sbVector3 pos );

    btRigidBody*
    add_box( float size_x, float size_y, float size_z,
             sbVector3 pos,
             float mass_=0.0f );

    void remove_body( btRigidBody* rb );

    ~World();
};

class Cube
{
private:
    int iamdone;
    Uint16 color;

    sbVector3 lastpos;
    btQuaternion lastrot;

    btDynamicsWorld* m_dworld;
    btRigidBody* m_rb;

public:
    Cube( sbVector3 pos=sbVector3(), Uint16 color_=0xFFF );

    void remove();
    
    int done(); // am i done and can be removed?
    
    void get_position_orientation( sbVector3& pos, btQuaternion& rot );

    void add_to_world( World& world );

    void remove_from_world();

    int crashed(); // did i crash in the last iteration

    ~Cube();
};




#endif

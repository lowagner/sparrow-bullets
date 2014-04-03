#ifndef __PHYSICS__
#define __PHYSICS__

#include "basic.h"

class Physics
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
	
public:

    Physics();

    void init();
    void deinit();

    void
    update( float dt );

    btRigidBody*
    add_cube( btTransform transform );

    btRigidBody*
    add_box( btTransform transform, sbVector size, 
             Uint32 mass_=0 );
    
    btRigidBody*
    add_ramp( btTransform transform, sbVector size, 
              Uint32 mass_=0 );

    void remove_body( btRigidBody* rb );

    ~Physics();
};


#endif

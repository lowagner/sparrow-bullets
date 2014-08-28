#ifndef __PHYSICS__
#define __PHYSICS__

#include "btBulletDynamicsCommon.h"
#include <iostream>

// borrowing a strange class from bullet2/Demos/CharacterDemo/DynamicCharacterController.cpp
class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
{
protected:
    btRigidBody* m_me;

public:
    ClosestNotMe(btRigidBody* me)
    :
        btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
    {
        m_me = me;
    }

    virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
    {
        if (rayResult.m_collisionObject == m_me)
            return 1.0;

        return ClosestRayResultCallback::addSingleResult( rayResult, normalInWorldSpace );
    }
};



class Physics
{

public:
    // biggest PHYSICS class
    // the general world which calculates everything, does dynamics, etc.
    btDynamicsWorld* m_dworld;

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

    int 
    set_object_dynamics( btRigidBody* body, btVector3 velocity, btVector3 omega = btVector3() );

    btRigidBody*
    add_cube( btTransform transform, 
              btVector3 velocity = btVector3(), 
              btVector3 omega = btVector3(), btScalar mass_=1 );

    btRigidBody*
    add_box( btVector3 size, btTransform transform, 
             btVector3 velocity = btVector3(), btVector3 omega = btVector3(),
             btScalar mass_=0 );
    
    btRigidBody*
    add_ramp( btVector3 size, btTransform transform, 
              btVector3 velocity = btVector3(), btVector3 omega = btVector3(),
              btScalar mass_=0 );

    void remove_body( btRigidBody* rb );

    ~Physics();
};


#endif

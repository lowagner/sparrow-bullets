#ifndef __PHYSICS__
#define __PHYSICS__

#include "btBulletDynamicsCommon.h"
#include <iostream>

// borrowing a strange class from bullet2/Demos/CharacterDemo/DynamicCharacterController.cpp
class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
{
protected:
    btCollisionObject* me;

public:
    ClosestNotMe(btCollisionObject* me_)
    :
        btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
    {
        me = me_;
    }

    virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
    {
        if (rayResult.m_collisionObject == me)
            return 1.0;

        return ClosestRayResultCallback::addSingleResult( rayResult, normalInWorldSpace );
    }

    const btCollisionObject* getHitter()
    {
        return m_collisionObject;
    }
};



class Physics
{

public:
    // biggest PHYSICS class
    // the general world which calculates everything, does dynamics, etc.
    btDynamicsWorld* dworld;

private:
    // more physics classes

    // this collision shapes holds all the unique collision shapes.
    // so don't add to it unless you have a different shape to collide.
    // (it's probably ok, it's just less efficient.)
    btAlignedObjectArray<btCollisionShape*> colshapes;

    // methods to calculate things...
    btBroadphaseInterface* broadphase;

    // something to dispatch... collision events?
    btCollisionDispatcher* dispatcher;

    // methods to calculate constraints
    btSequentialImpulseConstraintSolver* solver;

    // not sure what this is
    btDefaultCollisionConfiguration* colconfig;

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
    add_body( short int& dynamics,
              btCollisionShape* newbox, 
              btTransform transform, 
              btVector3 velocity, btVector3 omega,
              btScalar mass );

    btRigidBody*
    add_cube( short int& dynamics,
              btTransform transform, 
              btVector3 velocity = btVector3(), 
              btVector3 omega = btVector3(), btScalar mass_=1 );

    btRigidBody*
    add_box( short int& dynamics,
             btVector3 size, btTransform transform, 
             btVector3 velocity = btVector3(), btVector3 omega = btVector3(),
             btScalar mass_=0 );
    
    btRigidBody*
    add_ramp( short int& dynamics,
              btVector3 size, btTransform transform, 
              btVector3 velocity = btVector3(), btVector3 omega = btVector3(),
              btScalar mass_=0 );

    void remove_body( btRigidBody* rb );

    ~Physics();
};


#endif

/*
Check the LICENSE file included for copyright information.  
*/
#include "physics.h"
#include <iostream>


void
Physics::init()
{
    //initialization
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    colconfig = new btDefaultCollisionConfiguration();

    dispatcher = new btCollisionDispatcher( colconfig );

    broadphase = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver;

    dworld = new btDiscreteDynamicsWorld( dispatcher,
                                          broadphase,
                                          solver,
                                          colconfig );
    //dworld->setDebugDrawer(&gDebugDraw);

    dworld->setGravity( btVector3(0,0,-130) ); 
    // gravity should be greater (in magnitude) than about 1


    // standard box shape
    cubeshape = new btBoxShape( btVector3(1,1,1) );

    // put it in the collision shapes list
    colshapes.push_back( cubeshape );

    // standard box inertia, initialize to zero.
    cubeinertia = btVector3(0,0,0);
    // give the shape a mass and inertia
    cubeshape->calculateLocalInertia( btScalar(1), // mass of these boxes
                                      cubeinertia ); // "local" inertia.  moment of inertia???
    // that probably makes cubeinertia be something else!
}

Physics::Physics() 
{
    colconfig = NULL;
    dispatcher = NULL;
    broadphase = NULL;
    solver = NULL;
    dworld =  NULL;
}

btRigidBody*
Physics::add_cube( short int& dynamics, btTransform transform, btVector3 velocity, btVector3 omega, 
                   btScalar mass, btScalar sidelength )
{
    if ( sidelength == 1.0f ) 
        return add_body( dynamics, cubeshape, transform, velocity, omega, mass );
    else
    {
        btBoxShape* thiscubeshape = new btBoxShape( btVector3(sidelength,sidelength,sidelength) );
        colshapes.push_back( thiscubeshape );
        return add_body( dynamics, thiscubeshape, transform, velocity, omega, mass );
    }
}

btRigidBody*
Physics::add_box( short int& dynamics,
                  btVector3 size, btTransform transform, 
                  btVector3 velocity, btVector3 omega,
                  btScalar mass )
{
    btBoxShape* newbox = new btBoxShape( size );

    colshapes.push_back( newbox );
    
    return add_body( dynamics, newbox, transform, velocity, omega, mass );
}

btRigidBody*
Physics::add_ramp( short int& dynamics, btVector3 size, 
                   btTransform transform, 
                   btVector3 velocity, btVector3 omega,
                   btScalar mass )
{
    btConvexHullShape* newbox = new btConvexHullShape();
    newbox->addPoint( btVector3(0,0,0) );
    newbox->addPoint( btVector3(size.x(),0,0) );
    newbox->addPoint( btVector3(size.x(),size.y(),0) );
    newbox->addPoint( btVector3(0,size.y(),0) );
    newbox->addPoint( btVector3(size.x(),0,size.z()) );
    newbox->addPoint( btVector3(size.x(),size.y(),size.z()) );
    
    colshapes.push_back( newbox );

    //btTransform transform;
    //transform.setIdentity();
    //transform.setOrigin(   btVector3()   );

    return add_body( dynamics, newbox, transform, velocity, omega, mass );
}


btRigidBody*
Physics::add_body( short int& dynamics, 
                   btCollisionShape* newbox, 
                   btTransform transform, 
                   btVector3 velocity, btVector3 omega,
                   btScalar mass )
{
    // ONE SHOULD MAKE SURE TO HAVE YOUR collision shape added to the list of collision shapes,
    // if it's not already there, i.e.
    //      colshapes.push_back( newbox );
    // we don't do this automatically in this method.  this is because cubes are already there,
    // we don't want to add them multiple times.

    //rigidbody is fully dynamic (dynamics=3) if and only if mass is non zero
    btVector3 linertia(0,0,0); // not sure what this guy is, but everyone's doing it so let's go for it!
    if (mass != 0.f)
    {
        dynamics = 3;  // massive objects have full physics dynamics
        newbox->calculateLocalInertia(mass,linertia);
    }

    if ( dynamics < 0 )
    {
//        // There be ghosts.
//
//        btCollisionObject* body = new btPairCachingGhostObject();
//        body.setWorldTransform( transform );
//        body->setCollisionShape( newbox );
//        if ( ( velocity.length2() > 0 || omega.length2() > 0 ) || ( dynamics == -2 ) )
//        {
//            dynamics = -2;
//            body->setCollisionFlags( body->getCollisionFlags() |
//                                     btCollisionObject::CF_KINEMATIC_OBJECT );
//            body->setActivationState( DISABLE_DEACTIVATION );
//        }
//        else
//        {       
//            dynamics = -1;
//            body->setCollisionFlags( body->getCollisionFlags() |
//                                     btCollisionObject::CF_STATIC_OBJECT );
//        }
//        
////        ///only collide with static for now (no interaction with dynamic objects)
////        m_dynamicsWorld->addCollisionObject( body,
////            btBroadphaseProxy::CharacterFilter, 
////            btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter
////        );
//        // only let ghost objects collide with dynamic objects
//        dworld->addCollisionObject(
//            body,
//            // ghost objects are in the static group.
//            short(btBroadphaseProxy::StaticFilter),
//            // kinematic objects collide with everything but other static objects:
//            short(btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter)
//            //   a ^ b = a XOR b, bitwise.
//        );

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* motionstate = new btDefaultMotionState( transform );
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,motionstate,newbox,linertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        if ( velocity.length2() > 0 || omega.length2() > 0 )
            // since the object is moving, make the object kinematic if it is not already dynamic
            dynamics = -2;

        if ( dynamics == -2 )
        {
            body->setLinearVelocity( velocity );
            body->setAngularVelocity( omega );

            body->setCollisionFlags( body->getCollisionFlags() |
                                     btCollisionObject::CF_KINEMATIC_OBJECT |
                                     btCollisionObject::CF_NO_CONTACT_RESPONSE );
            body->setActivationState( DISABLE_DEACTIVATION );
            dworld->addRigidBody( body );
        }
        else
        {
            // dynamics == -1
            dynamics = -1;
            body->setActivationState( ISLAND_SLEEPING );
            body->setCollisionFlags( body->getCollisionFlags() |
                                     btCollisionObject::CF_NO_CONTACT_RESPONSE );
            dworld->addRigidBody( body );
        }
        return body;

    }
    else
    {
        // we either have a dynamic, kinematic, or static object...

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* motionstate = new btDefaultMotionState( transform );
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,motionstate,newbox,linertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        if ( ( velocity.length2() > 0 || omega.length2() > 0 ) && ( dynamics != 3 ) )
            // since the object is moving, make the object kinematic if it is not already dynamic
            dynamics = 2;

        if ( dynamics > 1 )
        {
            body->setLinearVelocity( velocity );
            body->setAngularVelocity( omega );

            if ( dynamics == 2 )
            {
                body->setCollisionFlags( body->getCollisionFlags() |
                                         btCollisionObject::CF_KINEMATIC_OBJECT );
                body->setActivationState( DISABLE_DEACTIVATION );
                dworld->addRigidBody( body );
            }
            else
            {
                dworld->addRigidBody( body );
            }
        }
        else
        {   
            // dynamics == 1
            body->setCollisionFlags( body->getCollisionFlags() |
                                     btCollisionObject::CF_STATIC_OBJECT );
            dworld->addRigidBody( body );
        }
        return body;
    }

}


void Physics::remove_body( btRigidBody* rb )
{
    dworld->removeRigidBody(rb);
    delete rb->getMotionState();
    delete rb;
}


void
Physics::update( float dt )
{
    // get the dynamics world to go through the time steps
    //std::cout << "World  dt = " << dt << std::endl;
    dworld->stepSimulation( dt, 13 );  
    // second argument has to do with target FPS...?  
    // not sure, but higher is good to get fullscreen and small screen
    // running at the same physicsy time, even if different FPS.
}

void
Physics::deinit()
{
    if ( dworld )
    {
        //cleanup in the reverse order of creation/initialization

        //remove the rigidbodies from the dynamics world and delete them
        int i;
        for (i=dworld->getNumCollisionObjects()-1; i>=0 ;i--)
        {
            btCollisionObject* obj = dworld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }
            dworld->removeCollisionObject( obj );
            delete obj;
        }

        //delete collision shapes
        for (int j=0;j<colshapes.size();j++)
        {
            btCollisionShape* shape = colshapes[j];
            delete shape;
        }
        colshapes.clear();

        delete dworld;
    }

    if ( solver )
        delete solver;

    if ( broadphase )
        delete broadphase;

    if ( dispatcher )
        delete dispatcher;

    if ( colconfig )
        delete colconfig;
    
    colconfig = NULL;
    dispatcher = NULL;
    broadphase = NULL;
    solver = NULL;
    dworld =  NULL;
}


Physics::~Physics()
{
    deinit();
}


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

    dworld->setGravity( btVector3(0,0,-150) ); 
    // gravity should be greater (in magnitude) than about 50


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
Physics::add_cube( short int& dynamics, btTransform transform, btVector3 velocity, btVector3 omega, btScalar mass )
{
    return add_body( dynamics, cubeshape, transform, velocity, omega, mass );
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


//	{
//		//create a few dynamic rigidbodies
//		// Re-using the same collision is better for memory usage and performance
//        float SCALING = 1.0;
//        int ARRAY_SIZE_X = 5;
//        int ARRAY_SIZE_Y = 5;
//        int ARRAY_SIZE_Z = 2;
//        float START_POS_X = 0.3;
//        float START_POS_Y = 0.3;
//        float START_POS_Z = 0.3;
//
//		btBoxShape* colShape = new btBoxShape(btVector3(SCALING*1,SCALING*1,SCALING*1));
//		//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
//		colshapes.push_back(colShape);
//
//		/// Create Dynamic Objects
//		btTransform startTransform;
//		startTransform.setIdentity();
//
//		btScalar	mass(1.f);
//
//		//rigidbody is dynamic if and only if mass is non zero, otherwise static
//		bool isDynamic = (mass != 0.f);
//
//		btVector3 linertia(0,0,0);
//		if (isDynamic)
//			colShape->calculateLocalInertia(mass,linertia);
//
//		float start_x = START_POS_X - ARRAY_SIZE_X/2;
//		float start_y = START_POS_Y;
//		float start_z = START_POS_Z - ARRAY_SIZE_Z/2;
//
//		for (int k=0;k<ARRAY_SIZE_Y;k++)
//		{
//			for (int i=0;i<ARRAY_SIZE_X;i++)
//			{
//				for(int j = 0;j<ARRAY_SIZE_Z;j++)
//				{
//					startTransform.setOrigin(SCALING*btVector3(
//										btScalar(2.0*i + start_x),
//										btScalar(20+2.0*k + start_y),
//										btScalar(2.0*j + start_z)));
//
//			
//					//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//					btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,linertia);
//					btRigidBody* body = new btRigidBody(rbInfo);
//					
//
//					dworld->addRigidBody(body);
//				}
//			}
//		}
//	}


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


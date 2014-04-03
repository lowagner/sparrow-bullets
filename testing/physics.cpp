#include "physics.h"
#include <iostream>

void
Physics::init()
{
    //initialization
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_colconfig = new btDefaultCollisionConfiguration();

	m_dispatcher = new	btCollisionDispatcher(m_colconfig);

	m_broadphase = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = new btSequentialImpulseConstraintSolver;

	m_dworld = new btDiscreteDynamicsWorld( m_dispatcher,
                                            m_broadphase,
                                            m_solver,
                                            m_colconfig );
	//m_dworld->setDebugDrawer(&gDebugDraw);
	
	m_dworld->setGravity( btVector3(0,0,-2.5) ); // -2 to -3 is pretty good.
    // gravity should be greater (in magnitude) than about 0.4


    // standard box shape
    cubeshape = new btBoxShape( btVector3(1,1,1) );
    
    // put it in the collision shapes list	
	m_colshapes.push_back( cubeshape );
   
    // standard box inertia, initialize to zero.
    cubeinertia = btVector3(0,0,0);
    // give the shape a mass and inertia
    cubeshape->calculateLocalInertia( btScalar(1), // mass of these boxes
                                      cubeinertia ); // "local" inertia.  moment of inertia???
    // that probably makes cubeinertia be something else!
}

Physics::Physics() 
{
    m_colconfig = NULL;
	m_dispatcher = NULL;
	m_broadphase = NULL;
    m_solver = NULL;
	m_dworld =  NULL;
}


btRigidBody*
Physics::add_cube( btTransform transform )
{
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState( transform );
    btRigidBody::btRigidBodyConstructionInfo rbInfo(btScalar(1),myMotionState,cubeshape,cubeinertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    m_dworld->addRigidBody(body);

    return body; // keep track of the bodies however you want
}

btRigidBody*
Physics::add_box( btTransform transform, sbVector size, 
                  Uint32 mass_ )
{
	btBoxShape* newbox = new btBoxShape(   btVector3( spFixedToFloat(size.x), 
                                                      spFixedToFloat(size.y), 
                                                      spFixedToFloat(size.z) )   );
	
	m_colshapes.push_back( newbox );

	//btTransform transform;
	//transform.setIdentity();
	//transform.setOrigin(   btVector3()   );


    btScalar mass( spFixedToFloat(mass_) );

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    btVector3 localInertia(0,0,0);
    if (mass != 0.f)
        newbox->calculateLocalInertia(mass,localInertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState( transform );
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,newbox,localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    m_dworld->addRigidBody(body);

    return body;
}

btRigidBody*
Physics::add_ramp( btTransform transform, sbVector size, 
                   Uint32 mass_ )
{
	btConvexHullShape* newbox = new btConvexHullShape();
    newbox->addPoint( btVector3(0,0,0) );
    newbox->addPoint( btVector3(spFixedToFloat(size.x),0,0) );
    newbox->addPoint( btVector3(spFixedToFloat(size.x),spFixedToFloat(size.y),0) );
    newbox->addPoint( btVector3(0,spFixedToFloat(size.y),0) );
    newbox->addPoint( btVector3(spFixedToFloat(size.x),0,spFixedToFloat(size.z)) );
    newbox->addPoint( btVector3(spFixedToFloat(size.x),spFixedToFloat(size.y),spFixedToFloat(size.z)) );
	
	m_colshapes.push_back( newbox );

	//btTransform transform;
	//transform.setIdentity();
	//transform.setOrigin(   btVector3()   );


    btScalar mass( spFixedToFloat(mass_) );

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    btVector3 localInertia(0,0,0);
    if (mass != 0.f)
        newbox->calculateLocalInertia(mass,localInertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState( transform );
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,newbox,localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    m_dworld->addRigidBody(body);

    return body;
}


void Physics::remove_body( btRigidBody* rb )
{
    m_dworld->removeRigidBody(rb);
    delete rb->getMotionState();
    delete rb;
}


void
Physics::update( float dt )
{
    // get the dynamics world to go through the time steps
    //std::cout << "World  dt = " << dt << std::endl;
    m_dworld->stepSimulation( dt, 13 );  
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
//		m_colshapes.push_back(colShape);
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
//		btVector3 localInertia(0,0,0);
//		if (isDynamic)
//			colShape->calculateLocalInertia(mass,localInertia);
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
//					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
//					btRigidBody* body = new btRigidBody(rbInfo);
//					
//
//					m_dworld->addRigidBody(body);
//				}
//			}
//		}
//	}


void
Physics::deinit()
{
    if ( m_dworld )
    {
        //cleanup in the reverse order of creation/initialization

        //remove the rigidbodies from the dynamics world and delete them
        int i;
        for (i=m_dworld->getNumCollisionObjects()-1; i>=0 ;i--)
        {
            btCollisionObject* obj = m_dworld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }
            m_dworld->removeCollisionObject( obj );
            delete obj;
        }

        //delete collision shapes
        for (int j=0;j<m_colshapes.size();j++)
        {
            btCollisionShape* shape = m_colshapes[j];
            delete shape;
        }
        m_colshapes.clear();

        delete m_dworld;
    }

    if ( m_solver )
	    delete m_solver;

    if ( m_broadphase )
	    delete m_broadphase;

    if ( m_dispatcher )
	    delete m_dispatcher;

    if ( m_colconfig )
	    delete m_colconfig;
    
    m_colconfig = NULL;
	m_dispatcher = NULL;
	m_broadphase = NULL;
    m_solver = NULL;
	m_dworld =  NULL;
}


Physics::~Physics()
{
    deinit();
}


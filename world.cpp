#include "world.h"
#include <iostream>

World::World() 
{
    // non physics type stuff
	checkertexture = spLoadSurfaceZoom( "./data/check.png", spFloatToFixed(1.0f));


    //initialization
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_collisionConfiguration = new btDefaultCollisionConfiguration();

	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	m_broadphase = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld( m_dispatcher,
                                                   m_broadphase,
                                                   m_solver,
                                                   m_collisionConfiguration );
	//m_dynamicsWorld->setDebugDrawer(&gDebugDraw);
	
	m_dynamicsWorld->setGravity( btVector3(0,0,-10) );
}

void
World::add_box( float x, float y, float z, 
                float size_x, float size_y, float size_z, 
                float mass_, 
                Uint16 color )
{
	btBoxShape* newbox = new btBoxShape( btVector3(size_x, size_y, size_z) );
	
	m_collisionShapes.push_back( newbox );

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin( btVector3(x,y,z) );

    btScalar mass(mass_);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        newbox->calculateLocalInertia(mass,localInertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState( transform );
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,newbox,localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    m_dynamicsWorld->addRigidBody(body);
}

void
World::update( float dt )
{
    // get the dynamics world to go through the time steps
    //std::cout << "World  dt = " << dt << std::endl;

    m_dynamicsWorld->stepSimulation( dt, 7 );  
    // second argument has to do with how many frames we do no matter what.
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
//		m_collisionShapes.push_back(colShape);
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
//					m_dynamicsWorld->addRigidBody(body);
//				}
//			}
//		}
//	}




void 
World::draw_textured_box( Sint32 halfsize, Uint16 color )
{

	spBindTexture( checkertexture );
    // Top / Bottom
	spQuadTex3D( -halfsize, halfsize, halfsize, 0, checkertexture->h,
				-halfsize, -halfsize, halfsize, 0, 0,
				halfsize, -halfsize, halfsize,  checkertexture->w, 0,
				halfsize, halfsize, halfsize,   checkertexture->w, checkertexture->h, color );
	spQuadTex3D( halfsize, halfsize, -halfsize,  0, checkertexture->h,
				halfsize, -halfsize, -halfsize,  0, 0,
				-halfsize, -halfsize, -halfsize, checkertexture->w, 0,
				-halfsize, halfsize, -halfsize,  checkertexture->w, checkertexture->h, color );
	//Left / Right
	spQuadTex3D( -halfsize, halfsize, halfsize,  0, checkertexture->h,
				-halfsize, halfsize, -halfsize,  0, 0,
				-halfsize, -halfsize, -halfsize, checkertexture->w, 0,
				-halfsize, -halfsize, halfsize,  checkertexture->w, checkertexture->h, color );
	spQuadTex3D( halfsize, -halfsize, halfsize,  0, checkertexture->h,
				halfsize, -halfsize, -halfsize,  0, 0,
				halfsize, halfsize, -halfsize,   checkertexture->w, 0,
				halfsize, halfsize, halfsize,    checkertexture->w, checkertexture->h, color );
	//Up / Down  // play_texture->w - 1, play_texture->h - 1, 
	spQuadTex3D( halfsize, halfsize, halfsize,   0, checkertexture->h,
				halfsize, halfsize, -halfsize,   0, 0,
				-halfsize, halfsize, -halfsize,  checkertexture->w, 0,
				-halfsize, halfsize, halfsize,   checkertexture->w, checkertexture->h, color );
	spQuadTex3D( -halfsize, -halfsize, halfsize,  0, checkertexture->h,
				-halfsize, -halfsize, -halfsize,  0, 0,
				halfsize, -halfsize, -halfsize,   checkertexture->w, 0,
				halfsize, -halfsize, halfsize,    checkertexture->w, checkertexture->h, color );
}

void
World::draw()
{
	spSetLight( 0 ); // or 1
	spSetAlphaTest( 0 );  // this makes purple not invisible
	
    spTranslate( 0, 0, spFloatToFixed( -12.0f ) );
	spRotateX( spFloatToFixed(-1.25f) );
    spTranslate( 0, 0, spFloatToFixed( -1.0f ) ); // go a bit up from the player
	//spRotateY( rotation );
    //spRotateZ( rotation );

	//spDeactivatePattern();
	spSetPerspectiveTextureMapping(0);

    Sint32 size = spFloatToFixed ( 1.5f );
    Uint16 color = 0x8F2F;
	spSetAlphaPattern4x4(200,8);
    draw_box( size, color );
	
	spTranslate( spFloatToFixed ( -3.0f ), 0, 0 );
    
    size = SP_ONE;
    color = 0x8F2F;
	//spRotateX( 0.5*rotation );
	spSetAlphaPattern4x4(255,8);  // max is 255.  not sure what the second arg does.  seems to help with texture.

    draw_textured_box( size, color );
}

void 
World::draw_box( Sint32 halfsize, Uint16 color )
{
    // Top / Bottom
    Uint16 topcolor = 0xFFFF;
	spQuad3D( -halfsize, halfsize, halfsize,    
				-halfsize, -halfsize, halfsize, 
				halfsize, -halfsize, halfsize,  
				halfsize, halfsize, halfsize,   topcolor );
	spQuad3D( halfsize, halfsize, -halfsize,     
				halfsize, -halfsize, -halfsize,  
				-halfsize, -halfsize, -halfsize, 
				-halfsize, halfsize, -halfsize,  color );
	//Left / Right
	spQuad3D( -halfsize, halfsize, halfsize,    
				-halfsize, halfsize, -halfsize, 
				-halfsize, -halfsize, -halfsize,
				-halfsize, -halfsize, halfsize, color );
	spQuad3D( halfsize, -halfsize, halfsize,   
				halfsize, -halfsize, -halfsize,
				halfsize, halfsize, -halfsize, 
				halfsize, halfsize, halfsize,  color );
	//Up / Down  // play_texture->w - 1, play_texture->h - 1, 
	spQuad3D( halfsize, halfsize, halfsize,    
				halfsize, halfsize, -halfsize, 
				-halfsize, halfsize, -halfsize,
				-halfsize, halfsize, halfsize, color );
	spQuad3D( -halfsize, -halfsize, halfsize,   
				-halfsize, -halfsize, -halfsize,
				halfsize, -halfsize, -halfsize, 
				halfsize, -halfsize, halfsize,  color );
}




World::~World()
{
	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	delete m_dynamicsWorld;
	
	delete m_solver;
	
	delete m_broadphase;
	
	delete m_dispatcher;

	delete m_collisionConfiguration;
}

#include "world.h"
#include <iostream>


void 
draw_textured_cube( SDL_Surface* texture, Sint32 halfsize, Uint16 color )
{

	spBindTexture( texture );
    // Top / Bottom
	spQuadTex3D( -halfsize, halfsize, halfsize, 0, texture->h,
				-halfsize, -halfsize, halfsize, 0, 0,
				halfsize, -halfsize, halfsize,  texture->w, 0,
				halfsize, halfsize, halfsize,   texture->w, texture->h, color );
	spQuadTex3D( halfsize, halfsize, -halfsize,  0, texture->h,
				halfsize, -halfsize, -halfsize,  0, 0,
				-halfsize, -halfsize, -halfsize, texture->w, 0,
				-halfsize, halfsize, -halfsize,  texture->w, texture->h, color );
	//Left / Right
	spQuadTex3D( -halfsize, halfsize, halfsize,  0, texture->h,
				-halfsize, halfsize, -halfsize,  0, 0,
				-halfsize, -halfsize, -halfsize, texture->w, 0,
				-halfsize, -halfsize, halfsize,  texture->w, texture->h, color );
	spQuadTex3D( halfsize, -halfsize, halfsize,  0, texture->h,
				halfsize, -halfsize, -halfsize,  0, 0,
				halfsize, halfsize, -halfsize,   texture->w, 0,
				halfsize, halfsize, halfsize,    texture->w, texture->h, color );
	//Up / Down  // play_texture->w - 1, play_texture->h - 1, 
	spQuadTex3D( halfsize, halfsize, halfsize,   0, texture->h,
				halfsize, halfsize, -halfsize,   0, 0,
				-halfsize, halfsize, -halfsize,  texture->w, 0,
				-halfsize, halfsize, halfsize,   texture->w, texture->h, color );
	spQuadTex3D( -halfsize, -halfsize, halfsize,  0, texture->h,
				-halfsize, -halfsize, -halfsize,  0, 0,
				halfsize, -halfsize, -halfsize,   texture->w, 0,
				halfsize, -halfsize, halfsize,    texture->w, texture->h, color );
}

void 
draw_box( Sint32 halfsize_x, Sint32 halfsize_y, Sint32 halfsize_z,  Uint16 color )
{
    // Top / Bottom
    //Uint16 topcolor = 0xFFFF;
	spQuad3D( -halfsize_x,  halfsize_y, halfsize_z,    
			  -halfsize_x, -halfsize_y, halfsize_z, 
			   halfsize_x, -halfsize_y, halfsize_z,  
			   halfsize_x,  halfsize_y, halfsize_z, color );
	spQuad3D(  halfsize_x,  halfsize_y, -halfsize_z,     
			   halfsize_x, -halfsize_y, -halfsize_z,  
			  -halfsize_x, -halfsize_y, -halfsize_z, 
			  -halfsize_x,  halfsize_y, -halfsize_z, color );
	//Left / Right
	spQuad3D( -halfsize_x,  halfsize_y,  halfsize_z,    
			  -halfsize_x,  halfsize_y, -halfsize_z, 
			  -halfsize_x, -halfsize_y, -halfsize_z,
			  -halfsize_x, -halfsize_y,  halfsize_z, color );
	spQuad3D( halfsize_x, -halfsize_y,  halfsize_z,   
			  halfsize_x, -halfsize_y, -halfsize_z,
			  halfsize_x,  halfsize_y, -halfsize_z, 
			  halfsize_x,  halfsize_y,  halfsize_z, color );
	//Up / Down  // play_texture->w - 1, play_texture->h - 1, 
	spQuad3D(  halfsize_x, halfsize_y,  halfsize_z,    
			   halfsize_x, halfsize_y, -halfsize_z, 
			  -halfsize_x, halfsize_y, -halfsize_z,
			  -halfsize_x, halfsize_y,  halfsize_z, color );
	spQuad3D( -halfsize_x, -halfsize_y,  halfsize_z,   
			  -halfsize_x, -halfsize_y, -halfsize_z,
			   halfsize_x,   -halfsize_y, -halfsize_z, 
			   halfsize_x,   -halfsize_y,  halfsize_z, color );
}



World::World() 
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
	
	m_dworld->setGravity( btVector3(0,0,-3) );


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


btRigidBody*
World::add_cube( sbVector pos )
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(   btVector3( spFixedToFloat(pos.x),
                                      spFixedToFloat(pos.y),
                                      spFixedToFloat(pos.z) )   );

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState( transform );
    btRigidBody::btRigidBodyConstructionInfo rbInfo(btScalar(1),myMotionState,cubeshape,cubeinertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    m_dworld->addRigidBody(body);

    return body; // keep track of the bodies however you want
}


btRigidBody*
World::add_box( sbVector size, 
                sbVector pos,
                Uint32 mass_ )
{
	btBoxShape* newbox = new btBoxShape(   btVector3( spFixedToFloat(size.x), 
                                                      spFixedToFloat(size.y), 
                                                      spFixedToFloat(size.z) )   );
	
	m_colshapes.push_back( newbox );

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(   btVector3( spFixedToFloat(pos.x),
                                      spFixedToFloat(pos.y),
                                      spFixedToFloat(pos.z) )   );


    btScalar mass( spFixedToFloat(mass_) );

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
    m_dworld->addRigidBody(body);

    return body;
}


void World::remove_body( btRigidBody* rb )
{
    m_dworld->removeRigidBody(rb);
    delete rb->getMotionState();
    delete rb;
}


void
World::update( float dt )
{
    // get the dynamics world to go through the time steps
    //std::cout << "World  dt = " << dt << std::endl;
    m_dworld->stepSimulation( dt, 7 );  
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






World::~World()
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
	
	delete m_solver;
	
	delete m_broadphase;
	
	delete m_dispatcher;

	delete m_colconfig;
}



Cube::Cube( sbVector pos, 
          Uint16 color_ )
{
    lastpos = pos;
    //std::cout << " pos = " << pos.z << std::endl;
    color = color_;
    m_dworld = NULL;
}


void
Cube::add_to_world( World& world )
{	
    if (m_dworld)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        m_dworld = world.m_dworld;
        m_rb = world.add_cube( lastpos );
    }
}


void
Cube::get_position_orientation( sbVector& pos, btQuaternion& rot )
{
    if (m_dworld)
    {
        btTransform transform;
        m_rb->getMotionState()->getWorldTransform( transform );
        lastpos = sbVector( transform.getOrigin() );
        lastrot = transform.getRotation();
        //std::cout << " cube z = " << lastpos.z << std::endl;
    }
    pos = lastpos;
    rot = lastrot;
}


void
Cube::remove_from_world()
{
    if (m_dworld)
    {
        m_dworld->removeRigidBody(m_rb);
        delete m_rb->getMotionState();
        delete m_rb;
        m_dworld = NULL;
        std::cout << " i am not physicsy anymore. " << std::endl;
    }
}

void 
Cube::change_matrix_and_draw( SDL_Surface* texture )
{
    spTranslate( lastpos.x, lastpos.y, lastpos.z );
	//spSetAlphaPattern4x4(200,8);
	spSetAlphaPattern4x4(255,8);  // max is 255.  not sure what the second arg does.  seems to help with texture.
    if (texture)
        draw_textured_cube( texture, SP_ONE, color );
    else
        draw_box( SP_ONE, SP_ONE, SP_ONE, color );
    spTranslate( -lastpos.x, -lastpos.y, -lastpos.z );
}

int
Cube::done()
{
    return iamdone;
}

Cube::~Cube()
{
    remove_from_world();
}

Box::Box( sbVector size_, 
          sbVector pos, 
          Uint16 color_ )
{
    size = size_;
    lastpos = pos;
    color = color_;
    m_dworld = NULL;
}


void
Box::add_to_world( World& world )
{	
    if (m_dworld)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        m_dworld = world.m_dworld;
        m_rb = world.add_box( size, lastpos );
    }
}


void
Box::get_position_orientation( sbVector& pos, btQuaternion& rot )
{
    if (m_dworld)
    {
        btTransform transform;
        m_rb->getMotionState()->getWorldTransform( transform );
        lastpos = sbVector( transform.getOrigin() );
        lastrot = transform.getRotation();
        std::cout << " box z = " << lastpos.z << std::endl;
    }
    pos = lastpos;
    rot = lastrot;
}


void
Box::remove_from_world()
{
    if (m_dworld)
    {
        m_dworld->removeRigidBody(m_rb);
        delete m_rb->getMotionState();
        delete m_rb;
        m_dworld = NULL;
    }
}

void
Box::change_matrix_and_draw()
{
    spTranslate( lastpos.x, lastpos.y, lastpos.z );
    draw_box( size.x, size.y, size.z, color );
}


int
Box::done()
{
    return iamdone;
}

Box::~Box()
{
    remove_from_world();
}





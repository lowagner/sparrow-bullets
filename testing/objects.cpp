#include "objects.h"

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

void 
draw_ramp( Sint32 sizex, Sint32 sizey, Sint32 sizez,  Uint16 color )
{
    // bottom
	spQuad3D( 0, 0, 0,    
			  0,  sizey, 0, 
			  sizex, sizey, 0,  
			  sizex, 0, 0, 
              color );
    // ramp
	spQuad3D( 0, 0, 0,     
			  sizex, 0, sizez,  
			  sizex, sizey, sizez, 
			  0,  sizey, 0, color );
    // back part
	spQuad3D( sizex, 0, 0,     
			  sizex, sizey, 0,  
			  sizex, sizey, sizez, 
			  sizex, 0, sizez, color );
    // left part
	spTriangle3D( 0, 0, 0,     
			      sizex, 0, 0,  
			      sizex, 0, sizez, 
			      color );
    // right part
	spTriangle3D( 0, sizey, 0,     
			      sizex, sizey, sizez, 
			      sizex, sizey, 0,  
			      color );
}

BaseObject::BaseObject()
{
    std::cout << " calling base object constructor " << id << std::endl;
    debug = false;
    m_physics = NULL;
    m_rb = NULL;
}

BaseObject::~BaseObject()
{
    // even in inherited classes, this method is called.
    
    // WARNING WARNING.
    // remove_physics();
    // DO NOT UNCOMMENT THE ABOVE LINE.
    // DO NOT remove_physics in any of the child class destructors, either.
    // YOU HAVE BEEN WARNED.
    // we shouldn't remove physics except explicitly:  in a vector of objects,
    // c++ destroys and recreates all higher elements if you erase an internal element.
    // you have no way to check if c++ is deleting the physics of an object that should
    // still exist.

    m_physics = NULL;
    m_rb = NULL;
    // it's called after the inherited class' own destructor.
    // don't put any virtual methods in it.
    std::cout << " finished calling base object " << id << " destructor " << std::endl;
}

sbVector 
BaseObject::last_position()
{
    sbVector pos;
    pos.x = lastpor[12];
    pos.y = lastpor[13];
    pos.z = lastpor[14];
    return pos;
}

sbVector 
BaseObject::last_velocity()
{
    return lastvelocity;
}

void
BaseObject::locate()
{
    if (m_physics)
    {
        // get the openGL matrix from the object
        btScalar m[16];
        btTransform transform;
        m_rb->getMotionState()->getWorldTransform( transform );
        transform.getOpenGLMatrix(m); 
        // we will use it in the draw_mess method.
        for (int i=0; i<16; i++)
        {
            lastpor[i] = spFloatToFixed( m[i] );
        }
    }
}

void 
BaseObject::rotate( sbVector axis, Sint32 angle )
{
    btScalar btm[16]; // old transform matrix in btScalar form
    for (int i=0; i<16; i++)
    {
        btm[i] = spFixedToFloat( lastpor[i] );
    }
    btTransform transform;
    transform.setFromOpenGLMatrix( btm ); 
    btQuaternion qrot = transform.getRotation();
    btQuaternion dqrot( btVector3( spFixedToFloat(axis.x),
                                   spFixedToFloat(axis.y),
                                   spFixedToFloat(axis.z) ), spFixedToFloat(angle) );
    qrot *= dqrot;
    // perform the transformation 
    transform.setRotation( qrot );
    
    // set the transformation back into the lastpor open GL orientation matrix
    transform.getOpenGLMatrix( btm );
    for (int i=0; i<16; i++)
    {
        lastpor[i] = spFloatToFixed( btm[i] );
    }

    if (m_physics)
    {
        // get the openGL matrix from the object
        m_rb->getMotionState()->setWorldTransform( transform );
        m_rb->setCenterOfMassTransform( transform );
    }
}

void 
BaseObject::rotateZ( Sint32 rad )
{
//Rotation matrix: (from sparrowRender spRotateZ)
    Sint32 s = spSin( rad );
    Sint32 c = spCos( rad );

    Sint32 rotate[16];
    rotate[ 0] = c;
    rotate[ 4] = -s;
    rotate[ 8] = 0;
    rotate[12] = 0;
    rotate[ 1] = s;
    rotate[ 5] = c;
    rotate[ 9] = 0;
    rotate[13] = 0;
    rotate[ 2] = 0;
    rotate[ 6] = 0;
    rotate[10] = SP_ONE;
    rotate[14] = 0;
    rotate[ 3] = 0;
    rotate[ 7] = 0;
    rotate[11] = 0;
    rotate[15] = SP_ONE;

    Sint32 result[8];
    result[ 0] = spMul( lastpor[0], rotate[0] ) + spMul( lastpor[4], rotate[1] );
    result[ 1] = spMul( lastpor[1], rotate[0] ) + spMul( lastpor[5], rotate[1] );
    result[ 2] = spMul( lastpor[2], rotate[0] ) + spMul( lastpor[6], rotate[1] );
    result[ 3] = spMul( lastpor[3], rotate[0] ) + spMul( lastpor[7], rotate[1] );

    result[ 4] = spMul( lastpor[0], rotate[4] ) + spMul( lastpor[4], rotate[5] );
    result[ 5] = spMul( lastpor[1], rotate[4] ) + spMul( lastpor[5], rotate[5] );
    result[ 6] = spMul( lastpor[2], rotate[4] ) + spMul( lastpor[6], rotate[5] );
    result[ 7] = spMul( lastpor[3], rotate[4] ) + spMul( lastpor[7], rotate[5] );

    memcpy( lastpor, result, sizeof(Sint32)*8 );

    if (m_physics)
    {
        btScalar btm[16]; // transform matrix in btScalar form
        for (int i=0; i<16; i++)
        {
            btm[i] = spFixedToFloat( lastpor[i] );
        }
        btTransform transform;
        transform.setFromOpenGLMatrix( btm ); 
        // get the openGL matrix from the object
        m_rb->getMotionState()->setWorldTransform( transform );
        m_rb->setCenterOfMassTransform( transform );
    }
}



void
BaseObject::translate( sbVector dist )
{
    lastpor[12] += dist.x;
    lastpor[13] += dist.y;
    lastpor[14] += dist.z;

    if (m_physics)
    {
        btScalar btm[16]; // transform matrix in btScalar form
        for (int i=0; i<16; i++)
        {
            btm[i] = spFixedToFloat( lastpor[i] );
        }

        btTransform transform;
        transform.setFromOpenGLMatrix( btm ); 
        // get the openGL matrix from the object
        m_rb->getMotionState()->setWorldTransform( transform );
        m_rb->setCenterOfMassTransform( transform );
    }
}

void
BaseObject::transform( Sint32* m )
{
    btScalar dbtm[16]; // transformation matrix in btScalar form
    btScalar obtm[16]; // old transform matrix in btScalar form
    for (int i=0; i<16; i++)
    {
        dbtm[i] = spFixedToFloat( m[i] );
        obtm[i] = spFixedToFloat( lastpor[i] );
    }
    btTransform transform;
    transform.setFromOpenGLMatrix( obtm ); 
    btTransform dtransform;
    dtransform.setFromOpenGLMatrix( dbtm ); 
    // perform the transformation 
    transform *= dtransform;
    
    // set the transformation back into the lastpor open GL orientation matrix
    transform.getOpenGLMatrix( obtm );
    for (int i=0; i<16; i++)
    {
        lastpor[i] = spFloatToFixed( obtm[i] );
    }

    if (m_physics)
    {
        // get the openGL matrix from the object
        m_rb->getMotionState()->setWorldTransform( transform );
        m_rb->setCenterOfMassTransform( transform );
    }
}

void
BaseObject::set_por( Sint32* m )
{
    if (m_physics)
    {
        btScalar btm[16]; // transform matrix in btScalar form
        for (int i=0; i<16; i++)
        {
            lastpor[i] = m[i];
            btm[i] = spFixedToFloat( m[i] );
        }
        btTransform transform;
        transform.setFromOpenGLMatrix( btm ); 

        // get the openGL matrix from the object
        m_rb->getMotionState()->setWorldTransform( transform );
        m_rb->setCenterOfMassTransform( transform );
    }
    else
    {
        for (int i=0; i<16; i++)
            lastpor[i] = m[i];
    }
}

btTransform
BaseObject::my_transform()
{
    btScalar btm[16]; // transform matrix in btScalar form
    for (int i=0; i<16; i++)
    {
        btm[i] = spFixedToFloat( lastpor[i] );
    }
    btTransform transform;
    // get the openGL matrix from the lastpor, now in btscalar form
    transform.setFromOpenGLMatrix( btm ); 
    return transform;
}


void
BaseObject::add_physics( Physics& physics )
{}

bool
BaseObject::out_of_bounds( sbVector outofbounds )
{
    if ( fabs(lastpor[14]) > outofbounds.z )
        return true;
    if ( fabs(lastpor[12]) > outofbounds.x )
        return true;
    if ( fabs(lastpor[13]) > outofbounds.y )
        return true;
    return false;
}

void
BaseObject::remove_physics()
{
    if (m_physics)
    {
        m_physics->m_dworld->removeRigidBody(m_rb);
        delete m_rb->getMotionState();
        delete m_rb;
        m_physics = NULL;
    }
}


void
BaseObject::update( float dt )
{}

void
BaseObject::reset_camera( Sint32* matrix )
{
    memcpy(spGetMatrix(),matrix,16*sizeof(Sint32)); //reload camera matrix after every draw
}

void
BaseObject::draw( Sint32* original_camera_matrix, int alpha )
{}


Cube::Cube( sbVector pos, Uint16 color_, SDL_Surface* texture_ )
{
    id = 0;
    speed = 2.8;
    rotspeed = 0.5;
    canjump = false;
    onground = false;
    debug = false;
    // setup GL orientation/transform matrix
    for (int i=0; i<16; i++)
        lastpor[i] = 0;
    for (int i=0; i<4; i++)
        lastpor[4*i+i] = SP_ONE;
    lastpor[12] = pos.x;
    lastpor[13] = pos.y;
    lastpor[14] = pos.z;

    color = color_;
    texture = texture_;
    m_physics = NULL;
}


void
Cube::add_physics( Physics& physics )
{	
    if (m_physics)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        m_physics = &physics;
        m_rb = physics.add_cube( my_transform() );
        //init_physics_por();
    }
}


bool 
Cube::feet_on_something() // ray cast down in cube-coords
{
    btVector3 rayfrom( spFixedToFloat(lastpor[12]), 
                       spFixedToFloat(lastpor[13]), 
                       spFixedToFloat(lastpor[14]) );
    // get the cube-z direction in world coordinates
    //btVector3 cubeup = m_rb->getWorldTransform().getBasis()[2];
    //cubeup.normalize(); // this shouldn't be necessary
    //btVector3 rayto = rayfrom - cubeup;

	btTransform cubeform;
	m_rb->getMotionState()->getWorldTransform( cubeform );
	btVector3 down = -cubeform.getBasis()[2];
	//btVector3 forward = cubeform.getBasis()[0];

    btVector3 rayto = rayfrom + down;


    ClosestNotMe ray(m_rb);

    m_physics->m_dworld->rayTest( rayfrom, rayto, ray ); 

    if ( ray.hasHit() )
        return true;
    else
        return false;
}

bool 
Cube::on_ground() // ray cast downwards in world-coords
{
    btVector3 rayfrom( spFixedToFloat(lastpor[12]), 
                       spFixedToFloat(lastpor[13]), 
                       spFixedToFloat(lastpor[14]) );
    btVector3 rayto = rayfrom - btVector3(0,0,1);
    
    ClosestNotMe ray(m_rb);

    m_physics->m_dworld->rayTest( rayfrom, rayto, ray ); 

    if ( ray.hasHit() )
        return true;
    else
        return false;
}

void
Cube::jump()
{
//
    if ( canjump )
    {
        canjump = false;

        // get the cube-z direction in world coordinates
        btVector3 cubeup = m_rb->getWorldTransform().getBasis()[2];
//        cubeup.setX( 1.0* int( cubeup.x()*100 ) / 100 );
//        cubeup.setY( 1.0* int( cubeup.y()*100 ) / 100 );
//        cubeup.setZ( 1.0* int( cubeup.z()*100 ) / 100 );
//        cubeup.normalize(); // this shouldn't be necessary
        // add worldup to cubeup to get a jump vector
        btVector3 jumpvec = (btVector3(0,0,1) + cubeup);
        
        m_rb->applyCentralImpulse( jumpvec );
    }
}


void
Cube::walk( float dt )
{
    if (canjump && onground)
    {
        // get the cube-x direction in world coordinates
        btVector3 forward = m_rb->getWorldTransform().getBasis()[0];
        forward.normalize();
        std::cout << " forward = " << forward.x() << ", "
                                   << forward.y() << ", "
                                   << forward.z() << "\n";

//        btTransform cubeform;
//        m_rb->getMotionState()->getWorldTransform( cubeform );
//        //btVector3 down = -cubeform.getBasis()[2];
//        btVector3 forward = cubeform.getBasis()[0];

        m_rb->applyCentralImpulse( speed*forward*dt );
    }
}

void
Cube::turn( float dt, int dir )
{
//    if (canjump && onground)
    {
        // get the cube-z direction in world coordinates
        btVector3 up = m_rb->getWorldTransform().getBasis()[2];

        //btTransform cubeform;
        //m_rb->getMotionState()->getWorldTransform( cubeform );
        //btVector3 up = cubeform.getBasis()[2];

//        btVector3 up = m_rb->getInvInertiaTensorWorld().inverse()*
//                                ( m_rb->getWorldTransform().getBasis()*btVector3(0,0,1) );

        //m_rb->applyTorque( rotspeed*up*dir ); 
//        m_rb->applyTorqueImpulse( rotspeed*up*dt*dir ); 
        Physics* standin = m_physics;
        remove_physics();
        rotateZ( spFloatToFixed(rotspeed*dt*dir) ); 
        add_physics(*standin);

    }
}

void
Cube::quick_turn()
{

}

void
Cube::update( float dt )
{
    // find where cube is now.
    locate();

    canjump = feet_on_something();
    onground = on_ground();
}


void 
Cube::draw( Sint32* matrix, int alpha )
{
    if ( alpha > 0 )
    {
        spMulMatrix( lastpor );
        spSetAlphaPattern4x4(alpha,8);
        //spSetAlphaPattern4x4(255,8);  // max is 255.  not sure what the second arg does.  seems to help with texture.
        if (texture)
            draw_textured_cube( texture, SP_ONE, color );
        else
            draw_box( SP_ONE, SP_ONE, SP_ONE, color );

        if ( debug )
        {
            spLine3D(0,0,0, 2*SP_ONE,0,0, 0xF00F);
            spLine3D(0,0,0, 0,2*SP_ONE,0, 0x0F0F);
            spLine3D(0,0,0, 0,0,2*SP_ONE, 0x00FF);
        }

        reset_camera( matrix );
    }
}

Cube::~Cube()
{
    //remove_physics(); // DO NOT UNCOMMENT.
    texture = NULL;
    std::cout << " finishing removing cube " << id << std::endl;
}

Cube::Cube( const Cube& other ) // copy constructor
{
    //std::cout << " calling cube copy constructor from " << other.id << " to " << id << std::endl;
    id = other.id + 100;
    speed = other.speed;
    rotspeed = other.rotspeed;
    debug = other.debug;
    texture = other.texture;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    color = other.color;
    lastvelocity = other.lastvelocity;
    iamdone = 0;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
}

Cube& 
Cube::operator = ( Cube other ) // Copy Assignment Operator
{
    //std::cout << " calling cube copy assignment from " << other.id << " to " << id << std::endl;
    id = other.id + 100;
    speed = other.speed;
    rotspeed = other.rotspeed;
    debug = other.debug;
    texture = other.texture;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    color = other.color;
    lastvelocity = other.lastvelocity;
    iamdone = 0;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
    return *this;
}

Box::Box( sbVector size_, 
          sbVector pos, 
          Uint16 color_ )
{
    id = 0;
    debug = false;
    for (int i=0; i<16; i++)
        lastpor[i] = 0;
    for (int i=0; i<4; i++)
        lastpor[4*i+i] = SP_ONE;
    lastpor[12] = pos.x;
    lastpor[13] = pos.y;
    lastpor[14] = pos.z;

    size = size_;
    color = color_;
    m_physics = NULL;
}


void
Box::add_physics( Physics& physics )
{	
    if (m_physics)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        m_physics = &physics;
        m_rb = physics.add_box( my_transform(), size );
        //init_physics_por();
    }
}


void
Box::update( float dt )
{
    // find where box is now.
    locate();
}

void
Box::draw( Sint32* matrix, int alpha )
{
    //spTranslate( lastpos.x, lastpos.y, lastpos.z );
    if ( alpha > 0 )
    {
        spMulMatrix( lastpor );
        spSetAlphaPattern4x4(alpha,8);
        draw_box( size.x, size.y, size.z, color );
        reset_camera( matrix );
    }
}

Box::~Box()
{
    //std::cout << " removing box " << std::endl;
    //remove_physics(); // DO NOT UNCOMMENT.
}

Box::Box( const Box& other ) // copy constructor
{
    //std::cout << " calling box copy constructor " << std::endl;
    id = other.id;
    debug = other.debug;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    size = other.size;
    color = other.color;
    lastvelocity = other.lastvelocity;
    iamdone = 0;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
}

Box& 
Box::operator = ( Box other ) // Copy Assignment Operator
{
    //std::cout << " calling box copy assignment " << std::endl;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    debug = other.debug;
    id = other.id;
    size = other.size;
    color = other.color;
    lastvelocity = other.lastvelocity;
    iamdone = 0;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
    return *this;
}




Ramp::Ramp( sbVector size_, 
          sbVector pos, 
          Uint16 color_ )
{
    id = 0;
    debug = false;
    for (int i=0; i<16; i++)
        lastpor[i] = 0;
    for (int i=0; i<4; i++)
        lastpor[4*i+i] = SP_ONE;
    lastpor[12] = pos.x;
    lastpor[13] = pos.y;
    lastpor[14] = pos.z;

    size = size_;
    color = color_;
    m_physics = NULL;
}

void
Ramp::add_physics( Physics& physics )
{	
    if (m_physics)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        m_physics = &physics;
        m_rb = physics.add_ramp( my_transform(), size );
        //init_physics_por();
    }
}

void
Ramp::update( float dt )
{
    // find where box is now.
    locate();
}

void
Ramp::draw( Sint32* matrix, int alpha )
{
    if ( alpha > 0 )
    {
        spSetAlphaPattern4x4(alpha,8);
        spMulMatrix( lastpor );
        draw_ramp( size.x, size.y, size.z, color );
        reset_camera( matrix );
    }
}

Ramp::~Ramp()
{
    //remove_physics(); // DO NOT UNCOMMENT.
    //std::cout << " removing ramp " << std::endl;
}

Ramp::Ramp( const Ramp& other ) // copy constructor
{
    //std::cout << " calling ramp copy constructor " << std::endl;
    id = other.id;
    debug = other.debug;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    size = other.size;
    color = other.color;
    lastvelocity = other.lastvelocity;
    iamdone = 0;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
}

Ramp& 
Ramp::operator = ( Ramp other ) // Copy Assignment Operator
{
    //std::cout << " calling ramp copy assignment " << std::endl;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    id = other.id;
    debug = other.debug;
    size = other.size;
    color = other.color;
    lastvelocity = other.lastvelocity;
    iamdone = 0;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
    return *this;
}

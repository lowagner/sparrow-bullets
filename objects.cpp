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
{}

void
BaseObject::remove()
{
    remove_physics();
}

BaseObject::~BaseObject()
{
    remove();
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
    if (m_dworld)
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

    if (m_dworld)
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

    if (m_dworld)
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

    if (m_dworld)
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

    if (m_dworld)
    {
        // get the openGL matrix from the object
        m_rb->getMotionState()->setWorldTransform( transform );
        m_rb->setCenterOfMassTransform( transform );
    }
}

void
BaseObject::set_por( Sint32* m )
{
    if (m_dworld)
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
    if (m_dworld)
    {
        m_dworld->removeRigidBody(m_rb);
        delete m_rb->getMotionState();
        delete m_rb;
        m_dworld = NULL;
    }
}


void
BaseObject::update( Uint32 dt )
{}

void
BaseObject::draw_mess()
{}




Cube::Cube( sbVector pos, Uint16 color_, SDL_Surface* texture_ )
{
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
    m_dworld = NULL;
}


void
Cube::add_physics( Physics& physics )
{	
    if (m_dworld)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        m_dworld = physics.m_dworld;
        m_rb = physics.add_cube( my_transform() );
        //init_physics_por();
    }
}


void
Cube::update( Uint32 dt )
{
    // find where cube is now.
    locate();
}


void 
Cube::draw_mess()
{
    //spTranslate( lastpos.x, lastpos.y, lastpos.z );
    spMulMatrix( lastpor );
	//spSetAlphaPattern4x4(200,8);
	spSetAlphaPattern4x4(255,8);  // max is 255.  not sure what the second arg does.  seems to help with texture.
    if (texture)
        draw_textured_cube( texture, SP_ONE, color );
    else
        draw_box( SP_ONE, SP_ONE, SP_ONE, color );
    //spTranslate( -lastpos.x, -lastpos.y, -lastpos.z );
}

void
Cube::remove()
{
    remove_physics();
}

Box::Box( sbVector size_, 
          sbVector pos, 
          Uint16 color_ )
{
    for (int i=0; i<16; i++)
        lastpor[i] = 0;
    for (int i=0; i<4; i++)
        lastpor[4*i+i] = SP_ONE;
    lastpor[12] = pos.x;
    lastpor[13] = pos.y;
    lastpor[14] = pos.z;

    size = size_;
    color = color_;
    m_dworld = NULL;
}


void
Box::add_physics( Physics& physics )
{	
    if (m_dworld)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        m_dworld = physics.m_dworld;
        m_rb = physics.add_box( my_transform(), size );
        //init_physics_por();
    }
}


void
Box::update( Uint32 dt )
{
    // find where box is now.
    locate();
}

void
Box::draw_mess()
{
    //spTranslate( lastpos.x, lastpos.y, lastpos.z );
    spMulMatrix( lastpor );
    draw_box( size.x, size.y, size.z, color );
}

void
Box::remove()
{
    remove_physics();
}


Ramp::Ramp( sbVector size_, 
          sbVector pos, 
          Uint16 color_ )
{
    for (int i=0; i<16; i++)
        lastpor[i] = 0;
    for (int i=0; i<4; i++)
        lastpor[4*i+i] = SP_ONE;
    lastpor[12] = pos.x;
    lastpor[13] = pos.y;
    lastpor[14] = pos.z;

    size = size_;
    color = color_;
    m_dworld = NULL;
}


void
Ramp::add_physics( Physics& physics )
{	
    if (m_dworld)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        m_dworld = physics.m_dworld;
        m_rb = physics.add_ramp( my_transform(), size );
        //init_physics_por();
    }
}


void
Ramp::update( Uint32 dt )
{
    // find where box is now.
    locate();
}

void
Ramp::draw_mess()
{
    //spTranslate( lastpos.x, lastpos.y, lastpos.z );
    spMulMatrix( lastpor );
    draw_ramp( size.x, size.y, size.z, color );
}

void
Ramp::remove()
{
    remove_physics();
}



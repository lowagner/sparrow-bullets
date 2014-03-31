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


BaseObject::BaseObject()
{}

BaseObject::~BaseObject()
{
    remove_physics();
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
BaseObject::add_physics( Physics& physics )
{}

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
        m_rb = physics.add_cube( last_position() );
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

Cube::~Cube()
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
        m_rb = physics.add_box( size, last_position() );
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


Box::~Box()
{
    remove_physics();
}





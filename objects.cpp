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




Cube::Cube( sbVector pos, 
          Uint16 color_ )
{
    lastpos = pos;
    for (int i=0; i<16; i++)
        lastorientation[i] = 0;
    for (int i=0; i<4; i++)
        lastorientation[4*i+i] = SP_ONE;
    lastorientation[12] = pos.x;
    lastorientation[13] = pos.y;
    lastorientation[14] = pos.z;

    //std::cout << " pos = " << pos.z << std::endl;
    color = color_;
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
        m_rb = physics.add_cube( lastpos );
    }
}


void
Cube::get_position_orientation( sbVector& pos, btQuaternion& rot )
{
    if (m_dworld)
    {
        btScalar m[16];
        btTransform transform;
        m_rb->getMotionState()->getWorldTransform( transform );
        lastpos = sbVector( transform.getOrigin() );
        lastrot = transform.getRotation();
        transform.getOpenGLMatrix(m);
        
        for (int i=0; i<16; i++)
        {
            lastorientation[i] = spFloatToFixed( m[i] );
        }

        //std::cout << " cube z = " << lastpos.z << std::endl;
    }
    pos = lastpos;
    rot = lastrot;
}


void
Cube::remove_physics()
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
    //spTranslate( lastpos.x, lastpos.y, lastpos.z );
    spMulMatrix( lastorientation );
	//spSetAlphaPattern4x4(200,8);
	spSetAlphaPattern4x4(255,8);  // max is 255.  not sure what the second arg does.  seems to help with texture.
    if (texture)
        draw_textured_cube( texture, SP_ONE, color );
    else
        draw_box( SP_ONE, SP_ONE, SP_ONE, color );
    //spTranslate( -lastpos.x, -lastpos.y, -lastpos.z );
}

int
Cube::done()
{
    return iamdone;
}

Cube::~Cube()
{
    remove_physics();
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
        m_rb = physics.add_box( size, lastpos );
    }
}


void
Box::get_position_orientation( sbVector& pos, btQuaternion& rot )
{
    if (m_dworld)
    {
        btScalar m[16];
        btTransform transform;
        m_rb->getMotionState()->getWorldTransform( transform );
        lastpos = sbVector( transform.getOrigin() );
        lastrot = transform.getRotation();
        //std::cout << " box z = " << lastpos.z << std::endl;
        
        transform.getOpenGLMatrix(m); 
        for (int i=0; i<16; i++)
        {
            lastorientation[i] = spFloatToFixed( m[i] );
        }

    }
    pos = lastpos;
    rot = lastrot;
}


void
Box::remove_physics()
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
    //spTranslate( lastpos.x, lastpos.y, lastpos.z );
    spMulMatrix( lastorientation );
    draw_box( size.x, size.y, size.z, color );
}


int
Box::done()
{
    return iamdone;
}

Box::~Box()
{
    remove_physics();
}





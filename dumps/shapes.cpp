/*
Check the LICENSE file included for copyright information.  
*/
#include "shapes.h"

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

Cube::Cube( btVector3 pos, Uint16 color_, SDL_Surface* texture_,
            btScalar mass_ )
{    
    mass = mass_;
    //std::cout << " Cube mass = " << mass << std::endl;
    id = 0;
    debug = false;
    // setup GL orientation/transform matrix
    for (int i=0; i<16; i++)
        lastpor[i] = 0;
    for (int i=0; i<4; i++)
        lastpor[4*i+i] = SP_ONE;
    lastpor[12] = spFloatToFixed( pos.getX() );
    lastpor[13] = spFloatToFixed( pos.getY() );
    lastpor[14] = spFloatToFixed( pos.getZ() );

    color = color_;
    texture = texture_;
    physics = NULL;
}


void
Cube::add_physics( Physics& new_physics, short int dynamics_ )
{
    if (physics)
    {
        // do we want to readd it by destroying it first?
        // yes.
        remove_physics();
    }
    dynamics = dynamics_;

    // non-kinematic, non dynamic guy
    // grab the physics pointer
    physics = &new_physics;
    // we haven't been keeping up with the transforms
    // get the transform from the por
    fix_transform();
    // add physics
    body = physics->add_cube( dynamics, transform, lastvelocity, lastomega, mass );
    //init_physics_por();
    //std::cerr << " dynamics Cube is " << dynamics << std::endl;
}


void
Cube::update( float dt )
{
    // find where cube is now.
    locate_and_move( dt );
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
    //std::cout << " finishing removing cube " << id << std::endl;
}

Cube::Cube( const Cube& other ) // copy constructor
{
    //std::cout << " calling cube copy constructor from " << other.id << " to " << id << std::endl;
    dynamics = other.dynamics;
    mass = other.mass;
    id = other.id + 100;
    debug = other.debug;
    texture = other.texture;
    physics = other.physics;
    body = other.body;
    color = other.color;
    transform = other.transform;
    lastposition = other.lastposition;
    lastvelocity = other.lastvelocity;
    lastomega = other.lastomega;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
}

Cube& 
Cube::operator = ( Cube other ) // Copy Assignment Operator
{
    dynamics = other.dynamics;
    mass = other.mass;
    id = other.id + 100;
    debug = other.debug;
    texture = other.texture;
    physics = other.physics;
    body = other.body;
    color = other.color;
    transform = other.transform;
    lastposition = other.lastposition;
    lastvelocity = other.lastvelocity;
    lastomega = other.lastomega;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
    return *this;
}

Box::Box( btVector3 size_, 
          btVector3 pos, 
          Uint16 color_, btScalar mass_ )
{
    mass = mass_;
    id = 0;
    debug = false;
    for (int i=0; i<16; i++)
        lastpor[i] = 0;
    for (int i=0; i<4; i++)
        lastpor[4*i+i] = SP_ONE;
    lastpor[12] = spFloatToFixed( pos.getX() );
    lastpor[13] = spFloatToFixed( pos.getY() );
    lastpor[14] = spFloatToFixed( pos.getZ() );

    size = size_;
    sizex = spFloatToFixed(size.x()); 
    sizey = spFloatToFixed(size.y());
    sizez = spFloatToFixed(size.z());

    color = color_;
    physics = NULL;
}


void
Box::add_physics( Physics& new_physics, short int dynamics_ )
{
    if (physics)
    {
        // do we want to readd it by destroying it first?
        // yes.
        remove_physics();
    }
    dynamics = dynamics_;

    // non-kinematic, non dynamic guy
    // grab the physics pointer
    physics = &new_physics;
    // we haven't been keeping up with the transforms
    // get the transform from the por
    fix_transform();
    // add physics
    body = physics->add_box( dynamics, size, transform, lastvelocity, lastomega, mass );
    //std::cerr << " dynamics Box is " << dynamics << std::endl;
}


void
Box::update( float dt )
{
    // find where box is now.
    locate_and_move( dt );
}

void
Box::draw( Sint32* matrix, int alpha )
{
    //spTranslate( lastpos.x, lastpos.y, lastpos.z );
    if ( alpha > 0 )
    {
        spMulMatrix( lastpor );
        spSetAlphaPattern4x4(alpha,8);
        draw_box( sizex, sizey, sizez, color );
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
    dynamics = other.dynamics;
    //std::cout << " calling box copy constructor " << std::endl;
    mass = other.mass;
    id = other.id;
    debug = other.debug;
    physics = other.physics;
    body = other.body;
    size = other.size;
    sizex = other.sizex;
    sizey = other.sizey;
    sizez = other.sizez;
    color = other.color;
    transform = other.transform;
    lastposition = other.lastposition;
    lastvelocity = other.lastvelocity;
    lastomega = other.lastomega;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
}

Box& 
Box::operator = ( Box other ) // Copy Assignment Operator
{
    //std::cout << " calling box copy assignment " << std::endl;
    dynamics = other.dynamics;
    physics = other.physics;
    mass = other.mass;
    body = other.body;
    debug = other.debug;
    id = other.id;
    size = other.size;
    sizex = other.sizex;
    sizey = other.sizey;
    sizez = other.sizez;
    color = other.color;
    transform = other.transform;
    lastposition = other.lastposition;
    lastvelocity = other.lastvelocity;
    lastomega = other.lastomega;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
    return *this;
}




Ramp::Ramp( btVector3 size_, 
          btVector3 pos, 
          Uint16 color_, btScalar mass_ )
{
    mass = mass_;
    id = 0;
    debug = false;
    for (int i=0; i<16; i++)
        lastpor[i] = 0;
    for (int i=0; i<4; i++)
        lastpor[4*i+i] = SP_ONE;
    lastpor[12] = spFloatToFixed( pos.getX() );
    lastpor[13] = spFloatToFixed( pos.getY() );
    lastpor[14] = spFloatToFixed( pos.getZ() );

    size = size_;
    sizex = spFloatToFixed(size.x()); 
    sizey = spFloatToFixed(size.y());
    sizez = spFloatToFixed(size.z());

    color = color_;
    physics = NULL;
}

void
Ramp::add_physics( Physics& new_physics, short int dynamics_ )
{
    if (physics)
    {
        // do we want to readd it by destroying it first?
        // yes.
        remove_physics();
    }

    dynamics = dynamics_;

    // non-kinematic, non dynamic guy
    // grab the physics pointer
    physics = &new_physics;
    // we haven't been keeping up with the transforms
    // get the transform from the por
    fix_transform();
    // add physics
    body = physics->add_ramp( dynamics, size, transform, lastvelocity, lastomega, mass );
    //std::cerr << " dynamics Ramp is " << dynamics << std::endl;
}

void
Ramp::update( float dt )
{
    // find where box is now.
    locate_and_move( dt );
}

void
Ramp::draw( Sint32* matrix, int alpha )
{
    if ( alpha > 0 )
    {
        spSetAlphaPattern4x4(alpha,8);
        spMulMatrix( lastpor );
        draw_ramp( sizex, sizey, sizez, color );
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
    dynamics = other.dynamics;
    mass = other.mass;
    id = other.id;
    debug = other.debug;
    physics = other.physics;
    body = other.body;
    size = other.size;
    sizex = other.sizex;
    sizey = other.sizey;
    sizez = other.sizez;
    color = other.color;
    transform = other.transform;
    lastposition = other.lastposition;
    lastvelocity = other.lastvelocity;
    lastomega = other.lastomega;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
}

Ramp& 
Ramp::operator = ( Ramp other ) // Copy Assignment Operator
{
    //std::cout << " calling ramp copy assignment " << std::endl;
    dynamics = other.dynamics;
    mass = other.mass;
    physics = other.physics;
    body = other.body;
    id = other.id;
    debug = other.debug;
    size = other.size;
    sizex = other.sizex;
    sizey = other.sizey;
    sizez = other.sizez;
    color = other.color;
    transform = other.transform;
    lastposition = other.lastposition;
    lastvelocity = other.lastvelocity;
    lastomega = other.lastomega;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
    return *this;
}

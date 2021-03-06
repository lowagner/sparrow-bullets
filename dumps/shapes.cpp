/*
Check the LICENSE file included for copyright information.  
*/
#include "shapes.h"

void 
draw_textured_cube( SDL_Surface* texture, Sint32 halfsize, Uint16 color )
{

    spBindTexture( texture );
    // Top / Bottom
    spQuadTex3D( -halfsize, halfsize, halfsize, texture->w, 0, // TOP
                -halfsize, -halfsize, halfsize, 0, 0,
                halfsize, -halfsize, halfsize,  0, texture->h,
                halfsize, halfsize, halfsize,   texture->w, texture->h, color );
    spQuadTex3D( halfsize, halfsize, -halfsize,  texture->w, 0, // BOTTOM
                halfsize, -halfsize, -halfsize,  0, 0,
                -halfsize, -halfsize, -halfsize, 0, texture->h,
                -halfsize, halfsize, -halfsize,  texture->w, texture->h, color );
    // Front / Back  
    spQuadTex3D( -halfsize, halfsize, halfsize,  0, 0, // BACK
                -halfsize, halfsize, -halfsize,  0, texture->h,
                -halfsize, -halfsize, -halfsize, texture->w, texture->h,
                -halfsize, -halfsize, halfsize,  texture->w, 0, color );
    spQuadTex3D( halfsize, -halfsize, halfsize,  0, 0, // FRONT
                halfsize, -halfsize, -halfsize,  0, texture->h,
                halfsize, halfsize, -halfsize,   texture->w, texture->h,
                halfsize, halfsize, halfsize,    texture->w, 0, color );
    // Left / Right
    spQuadTex3D( halfsize, halfsize, halfsize,   0, 0, // LEFT
                halfsize, halfsize, -halfsize,   0, texture->h,
                -halfsize, halfsize, -halfsize,  texture->w, texture->h,
                -halfsize, halfsize, halfsize,   texture->w, 0, color );
    spQuadTex3D( -halfsize, -halfsize, halfsize,  0, 0, // RIGHT
                -halfsize, -halfsize, -halfsize,  0, texture->h,
                halfsize, -halfsize, -halfsize,   texture->w, texture->h,
                halfsize, -halfsize, halfsize,    texture->w, 0, color );
}

void 
draw_flip_textured_cube( SDL_Surface* texture, Sint32 halfsize, Uint16 color )
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
    // Front / Back  
    spQuadTex3D( -halfsize, halfsize, halfsize,  0, texture->h,
                -halfsize, halfsize, -halfsize,  0, 0,
                -halfsize, -halfsize, -halfsize, texture->w, 0,
                -halfsize, -halfsize, halfsize,  texture->w, texture->h, color );
    spQuadTex3D( halfsize, -halfsize, halfsize,  0, texture->h,
                halfsize, -halfsize, -halfsize,  0, 0,
                halfsize, halfsize, -halfsize,   texture->w, 0,
                halfsize, halfsize, halfsize,    texture->w, texture->h, color );
    //Left / Right
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

Cube::Cube( btVector3 pos, Uint16 color_, 
            int id_,
            SDL_Surface* texture_, 
            bool fliptexture_,
            btScalar mass_,
            Sint32 sidelength_ )
{    
    sidelength = sidelength_;
    float side = spFixedToFloat(sidelength_);
    size = btVector3( side, side, side );
    mass = mass_;
    lastposition = pos;
    //std::cout << " Cube mass = " << mass << std::endl;
    id = id_;
    fliptexture = fliptexture_;
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
    selfa = 255;
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
    // add dynamics if the cyclepositions are there
    if ( cyclepositions.size() && abs(dynamics) != 2 )
        dynamics = 2;
    // add physics
    body = physics->add_cube( dynamics, transform, lastvelocity, lastomega, mass, size.x() );
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
Cube::draw( SDL_Surface* screen, Sint32* matrix, spFontPointer font, int alpha_ )
{
    short int alpha = selfa * alpha_ / 255;
    if ( alpha > 0 )
    {
        spMulMatrix( lastpor );
            
        Sint32 Z = spGetMatrix()[14];
        if ( Z < -5*SP_ONE )
        {
            spSetAlphaPattern4x4(alpha,8);
            //spSetAlphaPattern4x4(255,8);  // max is 255.  not sure what the second arg does.  seems to help with texture.
            if (texture)
            {
                if ( fliptexture )
                    draw_flip_textured_cube( texture, sidelength, color );
                else
                    draw_textured_cube( texture, sidelength, color );
            }
            else
                draw_box( sidelength, sidelength, sidelength, color );

            if ( debug )
            {
//                std::cerr << " x, y, z camera = " << spGetMatrix()[12] << ", "
//                                                  << spGetMatrix()[13] << ", "
//                                                  << spGetMatrix()[14] << "; -5*ONE = " << -5*SP_ONE << "\n"; 
//                std::cerr << " w, h screen = " << screen->w << ", "
//                                                  << screen->h << "\n"; 
//                int X = screen->w/2 + (spGetMatrix()[12] / 800);
//                int Y = screen->h/2 - (spGetMatrix()[13] / 800);
//                std::cerr << " X, Y screen = " << X << ", "
//                                                  << Y << "\n"; 
                spLine3D(0,0,0, 2*SP_ONE,0,0, 0xF00F);
                spLine3D(0,0,0, 0,2*SP_ONE,0, 0x0F0F);
                spLine3D(0,0,0, 0,0,2*SP_ONE, 0x00FF);
            }
            
            if ( text.size() ) 
            {
                write_text( screen, font );
            }
                
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
    text = other.text;
    size = other.size;
    cyclepositions = other.cyclepositions;
    cyclepositionindex = other.cyclepositionindex;
    objecttime = other.objecttime;
    sidelength = other.sidelength;
    dynamics = other.dynamics;
    mass = other.mass;
    id = other.id;
    fliptexture = other.fliptexture;
    debug = other.debug;
    texture = other.texture;
    physics = other.physics;
    body = other.body;
    color = other.color;
    selfa = other.selfa;
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
    text = other.text;
    size = other.size;
    cyclepositions = other.cyclepositions;
    cyclepositionindex = other.cyclepositionindex;
    objecttime = other.objecttime;
    sidelength = other.sidelength;
    dynamics = other.dynamics;
    mass = other.mass;
    id = other.id;
    fliptexture = other.fliptexture;
    debug = other.debug;
    texture = other.texture;
    physics = other.physics;
    body = other.body;
    color = other.color;
    selfa = other.selfa;
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
          Uint16 color_, 
          int id_,
          btScalar mass_ )
{
    mass = mass_;
    id = id_;
    debug = false;
    for (int i=0; i<16; i++)
        lastpor[i] = 0;
    for (int i=0; i<4; i++)
        lastpor[4*i+i] = SP_ONE;
    lastpor[12] = spFloatToFixed( pos.getX() );
    lastpor[13] = spFloatToFixed( pos.getY() );
    lastpor[14] = spFloatToFixed( pos.getZ() );
    lastposition = pos;

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
    selfa = 255;
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
    // add dynamics if the cyclepositions are there
    if ( cyclepositions.size() && abs(dynamics) != 2 )
        dynamics = 2;
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
Box::draw( SDL_Surface* screen, Sint32* matrix, spFontPointer font, int alpha_ )
{
    short int alpha = selfa * alpha_ / 255;
    //spTranslate( lastpos.x, lastpos.y, lastpos.z );
    if ( alpha > 0 )
    {
        spMulMatrix( lastpor );
        Sint32 Z = spGetMatrix()[14];
        if ( Z < -5*SP_ONE )
        {
            spSetAlphaPattern4x4(alpha,8);
            draw_box( sizex, sizey, sizez, color );
            if ( text.size() ) 
            {
                write_text( screen, font );
            }
        }
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
    text = other.text;
    dynamics = other.dynamics;
    //std::cout << " calling box copy constructor " << std::endl;
    cyclepositions = other.cyclepositions;
    cyclepositionindex = other.cyclepositionindex;
    objecttime = other.objecttime;
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
    selfa = other.selfa;
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
    text = other.text;
    //std::cout << " calling box copy assignment " << std::endl;
    cyclepositions = other.cyclepositions;
    cyclepositionindex = other.cyclepositionindex;
    objecttime = other.objecttime;
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
    selfa = other.selfa;
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
            Uint16 color_, 
            int id_,
            btScalar mass_ )
{
    lastposition = pos;
    mass = mass_;
    id = id_;
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
    selfa = 255;
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
    // add dynamics if the cyclepositions are there
    if ( cyclepositions.size() && abs(dynamics) != 2 )
        dynamics = 2;
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
Ramp::draw( SDL_Surface* screen, Sint32* matrix, spFontPointer font, int alpha_ )
{
    short int alpha = selfa * alpha_ / 255;
    if ( alpha > 0 )
    {
        spMulMatrix( lastpor );
        Sint32 Z = spGetMatrix()[14];
        if ( Z < -5*SP_ONE )
        {
            spSetAlphaPattern4x4(alpha,8);
            draw_ramp( sizex, sizey, sizez, color );
            if ( text.size() ) 
            {
                write_text( screen, font );
            }
        }
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
    text = other.text;
    //std::cout << " calling ramp copy constructor " << std::endl;
    cyclepositions = other.cyclepositions;
    cyclepositionindex = other.cyclepositionindex;
    objecttime = other.objecttime;
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
    selfa = other.selfa;
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
    text = other.text;
    //std::cout << " calling ramp copy assignment " << std::endl;
    cyclepositions = other.cyclepositions;
    cyclepositionindex = other.cyclepositionindex;
    objecttime = other.objecttime;
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
    selfa = other.selfa;
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

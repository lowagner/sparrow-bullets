#ifndef __SHAPES__
#define __SHAPES__

#include "objects.h"


void draw_textured_cube( SDL_Surface* texture, Sint32 halfsize = SP_ONE, Uint16 color = 0xFFFF );
void draw_box( Sint32 halfsize_x = SP_ONE, 
               Sint32 halfsize_y = SP_ONE, 
               Sint32 halfsize_z = SP_ONE, 
               Uint16 color = 0xFFFF );
void draw_ramp( Sint32 sizex = SP_ONE, 
                Sint32 sizey = SP_ONE, 
                Sint32 sizez = SP_ONE, 
                Uint16 color = 0xFFFF );

class Cube : public BaseObject
{ 
    friend class Player;
protected:
    SDL_Surface* texture;

public:
    Cube( btVector3 pos=btVector3(), Uint16 color_=0xFFFF, 
          SDL_Surface* texture_ = NULL, btScalar mass_ = btScalar(1) );

    void update( float dt );

    void add_physics( Physics& new_physics, short int dynamics_=3 );

    // translate/rotate the model-view matrix and then draw:
    void draw( Sint32* matrix, int alpha=255 ); 

    ~Cube();
    Cube( const Cube& other ); // copy constructor
    Cube& operator = ( Cube other ); // Copy Assignment Operator
};


class Box : public BaseObject
{
    friend class Player;
protected:
    Sint32 sizex;
    Sint32 sizey;
    Sint32 sizez;

public:
    Box( btVector3 size_=btVector3(1,1,1), 
         btVector3 pos=btVector3(), Uint16 color_=0xFFFF, btScalar mass_=0 );

    void update( float dt );
    
    void add_physics( Physics& new_physics, short int dynamics_=1 );

    // translate/rotate the model-view matrix and then draw:
    void draw( Sint32* matrix, int alpha=255 ); 

    //void remove();

    ~Box();
    Box( const Box& other ); // copy constructor
    Box& operator = ( Box other ); // Copy Assignment Operator
};

class Ramp : public BaseObject
{
    friend class Player;

protected:
    btVector3 size;
    Sint32 sizex;
    Sint32 sizey;
    Sint32 sizez;

public:
    Ramp( btVector3 size_=btVector3(1,1,1), 
          btVector3 pos=btVector3(), Uint16 color_=0xFFFF, btScalar mass_=0 );

    void update( float dt );
    
    void add_physics( Physics& new_physics, short int dynamics_=1 );

    // translate/rotate the model-view matrix and then draw:
    void draw( Sint32* matrix, int alpha=255 ); 

    //void remove();

    ~Ramp();
    Ramp( const Ramp& other ); // copy constructor
    Ramp& operator = ( Ramp other ); // Copy Assignment Operator
};

#endif

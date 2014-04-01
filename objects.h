#ifndef __OBJECTS__
#define __OBJECTS__

#include "physics.h"
#include "csparrow.h"


void draw_textured_cube( SDL_Surface* texture, Sint32 halfsize = SP_ONE, Uint16 color = 0xFFFF );
void draw_box( Sint32 halfsize_x = SP_ONE, 
               Sint32 halfsize_y = SP_ONE, 
               Sint32 halfsize_z = SP_ONE, 
               Uint16 color = 0xFFFF );
void draw_ramp( Sint32 sizex = SP_ONE, 
                Sint32 sizey = SP_ONE, 
                Sint32 sizez = SP_ONE, 
                Uint16 color = 0xFFFF );

class BaseObject
{
protected:
    int iamdone;
    Uint16 color;

    Sint32 lastpor[16]; // last position and orientation (rotation).  stored as openGL matrix.
    sbVector lastvelocity;

    btDynamicsWorld* m_dworld;
    btRigidBody* m_rb;

    void locate();
    btTransform my_transform();

public:
    BaseObject();

    virtual void update( Uint32 dt );

    sbVector last_position();
    
    sbVector last_velocity();

    virtual void add_physics( Physics& physics );

    void translate( sbVector dist );

    void rotateZ( Sint32 angle );
    void rotate( sbVector axis, Sint32 angle );

    void transform( Sint32* m );
    
    void set_por( Sint32* m );

    void remove_physics();

    // translate/rotate the model-view matrix and then draw:
    virtual void draw_mess(); 

    virtual ~BaseObject();
};


class Cube : public BaseObject
{ 
protected:
    SDL_Surface* texture;

public:
    Cube( sbVector pos=sbVector(), Uint16 color_=0xFFFF, SDL_Surface* texture_ = NULL );

    void update( Uint32 dt );

    void add_physics( Physics& physics );

    // translate/rotate the model-view matrix and then draw:
    void draw_mess(); 

    ~Cube();
};


class Box : public BaseObject
{
protected:
    sbVector size;
public:
    Box( sbVector size_=sbVector(1,1,1), sbVector pos=sbVector(), Uint16 color_=0xFFFF );

    void update( Uint32 dt );
    
    void add_physics( Physics& physics );

    // translate/rotate the model-view matrix and then draw:
    void draw_mess(); 

    ~Box();
};

class Ramp : public BaseObject
{
protected:
    sbVector size;
public:
    Ramp( sbVector size_=sbVector(1,1,1), sbVector pos=sbVector(), Uint16 color_=0xFFFF );

    void update( Uint32 dt );
    
    void add_physics( Physics& physics );

    // translate/rotate the model-view matrix and then draw:
    void draw_mess(); 

    ~Ramp();
};

#endif

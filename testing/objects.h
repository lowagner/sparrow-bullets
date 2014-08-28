#ifndef __OBJECTS__
#define __OBJECTS__

#include "physics.h"
#include "../csparrow.h"


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
    Uint16 color;
    Sint32 lastpor[16]; // last position and orientation (rotation), for display.  stored as openGL matrix.
   
    // INTERFACING TO PHYSICS ENGINE
    bool kinematic; // whether or not the object is moved by fiat.

    btScalar mass;
    btTransform transform; // the position and orientation to interface with Bullet 
    btVector3 lastposition; // position
    btVector3 lastvelocity; // linear velocity
    btVector3 lastomega; //angular velocity

    Physics* m_physics;
    btRigidBody* m_rb;

    void update_por( btScalar dt ); // updates por for non-kinematic/dynamic objects
    void fix_transform(); // essentially sets transform from the lastpor guy.
    void fix_por(); // essentially sets lastpor from the transform guy.

    void update_transform( btScalar dt ); // updates por/transform for kinematic objects

    void locate_and_move( btScalar dt );
    void reset_camera( Sint32* matrix );
    

public:
    int id;
    bool debug;

    BaseObject();

    virtual void update( float dt );

    btVector3 last_position();
    
    btVector3 last_velocity();
    
    btVector3 last_omega();

    virtual void add_physics( Physics& physics );

    bool out_of_bounds( btVector3 outofbounds );

    void translate( btVector3 dist );

    void rotateZ( btScalar angle );
    void rotate( btVector3 axis, btScalar angle );

    //void transform( Sint32* m );

    
    //void set_por( Sint32* m );

    void remove_physics();

    //virtual void remove();

    // translate/rotate the model-view matrix, draws, then resets the model-view to "matrix"
    virtual void draw( Sint32* matrix, int alpha=255 ); 

    // rule of three, define all these or none at all.
    virtual ~BaseObject();
    //BaseObject( const BaseObject& other ); // copy constructor
    //virtual BaseObject& operator = ( BaseObject other ); // Copy Assignment Operator
    // except we don't here, because we don't want our children to inherit
    // anything here.

};


class Cube : public BaseObject
{ 
protected:
    SDL_Surface* texture;
    bool canjump, onground;
    float speed, rotspeed;

public:
    Cube( btVector3 pos=btVector3(), Uint16 color_=0xFFFF, SDL_Surface* texture_ = NULL, btScalar mass_=1 );

    void update( float dt );

    void add_physics( Physics& physics );

    // translate/rotate the model-view matrix and then draw:
    void draw( Sint32* matrix, int alpha=255 ); 

    bool feet_on_something(); // ray cast down in cube-coords
    bool on_ground(); // ray cast downwards in world-coords

    void jump();
    void walk( float dt );
    void turn( float dt, int dir );
    void quick_turn();

    ~Cube();
    Cube( const Cube& other ); // copy constructor
    Cube& operator = ( Cube other ); // Copy Assignment Operator
};


class Box : public BaseObject
{
protected:
    btVector3 size;
    Sint32 sizex;
    Sint32 sizey;
    Sint32 sizez;

public:
    Box( btVector3 size_=btVector3(1,1,1), btVector3 pos=btVector3(), Uint16 color_=0xFFFF, btScalar mass_=0 );

    void update( float dt );
    
    void add_physics( Physics& physics );

    // translate/rotate the model-view matrix and then draw:
    void draw( Sint32* matrix, int alpha=255 ); 

    //void remove();

    ~Box();
    Box( const Box& other ); // copy constructor
    Box& operator = ( Box other ); // Copy Assignment Operator
};

class Ramp : public BaseObject
{
protected:
    btVector3 size;
    Sint32 sizex;
    Sint32 sizey;
    Sint32 sizez;

public:
    Ramp( btVector3 size_=btVector3(1,1,1), btVector3 pos=btVector3(), Uint16 color_=0xFFFF, btScalar mass_=0 );

    void update( float dt );
    
    void add_physics( Physics& physics );

    // translate/rotate the model-view matrix and then draw:
    void draw( Sint32* matrix, int alpha=255 ); 

    //void remove();

    ~Ramp();
    Ramp( const Ramp& other ); // copy constructor
    Ramp& operator = ( Ramp other ); // Copy Assignment Operator
};

#endif

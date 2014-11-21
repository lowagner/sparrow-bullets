#ifndef __OBJECTS__
#define __OBJECTS__

#include "physics.h"
#include "../csparrow.h"
#include <vector>

class BaseObject
{
    friend class Player;
protected:
    Uint16 color; 
    short int selfa;

    Sint32 lastpor[16]; // last position and orientation (rotation), for display.  stored as openGL matrix.
   
    // INTERFACING TO PHYSICS ENGINE
    //bool kinematic; // whether or not the object is moved by fiat.
    short int dynamics;  //  0 is not in the physics world
                         //  1 is in the physics world, but not moving
                         //  2 is in the physics world, moving, but with mass=0 and not affected by physics
                         //  3 is fully in the physics world, dynamics fully described by the world
                         // -1 is a ghost in the physics world, not moving
                         // -2 is a ghost in the physics world, moving
    
    bool onground;

    btTransform transform; // the position and orientation to interface with Bullet 
    btVector3 lastposition; // position
    btVector3 lastvelocity; // linear velocity
    btVector3 lastomega; //angular velocity

    std::vector<btVector4> cyclepositions; 
    btScalar objecttime;
    short int cyclepositionindex;

    btRigidBody* body;

    btScalar mass;
    btVector3 size;

    void update_por( btScalar dt ); // updates por for non-kinematic/dynamic objects
    void fix_transform(); // essentially sets transform from the lastpor guy.
    void fix_por(); // essentially sets lastpor from the transform guy.

    void update_transform( btScalar dt ); // updates por/transform for kinematic objects

    void locate_and_move( btScalar dt );
    void reset_camera( Sint32* matrix );
    void write_text( SDL_Surface* screen, spFontPointer font );
    
    std::vector<std::string> text;

public:
    Physics* physics;

    void add_text( char* text_ );

    int id;
    bool debug;

    BaseObject();

    void set_alpha( short int selfa_ );

    void set_velocity( btVector3 vel );

    void push_cycle( btVector3 position, btScalar dt = 2 );

    btVector3 get_pos();

    virtual void update( float dt );

    virtual void add_physics( Physics& new_physics, short int dynamics_=1 );

    bool out_of_bounds( btVector3 outofbounds );

    void translate( btVector3 dist );
    void activate();

    void rotateZ( btScalar angle );
    void rotate( btVector3 axis, btScalar angle );
    void impulse( btVector3 forcedt );

    //void transform( Sint32* m );

    
    //void set_por( Sint32* m );

    void remove_physics();

    //virtual void remove();

    // translate/rotate the model-view matrix, draws, then resets the model-view to "matrix"
    virtual void draw( SDL_Surface* screen, Sint32* matrix, spFontPointer font=NULL, int alpha_=255 ); 

    // rule of three, define all these or none at all.
    virtual ~BaseObject();
    //BaseObject( const BaseObject& other ); // copy constructor
    //virtual BaseObject& operator = ( BaseObject other ); // Copy Assignment Operator
    // except we don't here, because we don't want our children to inherit
    // anything here.

};


class Player 
{ 
protected:
    
    bool canjump;
    int topsideup, facesideup;
    

    float maxwalkspeed2, walkacceleration, currentacceleration;
    float maxrotspeed2, rotacceleration, flyingrotacceleration, currentrotacceleration;
    float jerk, rotjerk;
    float siderotacceleration;
    float sidewalkacceleration;
    float wrigglerotimpulse, wriggleupimpulse;

    float jumpimpulse, kickimpulse, kickupimpulse, kickairrotimpulse;

    void check_surroundings();

public:
    BaseObject* object;

    Player( btVector3 pos=btVector3(), Uint16 color_=0xFFFF, 
            SDL_Surface* texture_ = NULL, btScalar mass_ = btScalar(1) );
    Player( BaseObject object_ );

    void init();
    
    virtual void update( float dt );

    int jump();
    int kick( int forwarddir );

    int walk( float dt, int dir=1 );
    int turn( float dt, int dir );
    int quick_turn();
    void draw_debug();

    bool on_ground();
    int topside_up();
    int facing_up();

    btVector3 get_position();

    btVector3 get_forward();
    btVector3 get_side();
    btVector3 get_up();
   
    ~Player();
    Player( const Player& other ); // copy constructor
    Player& operator = ( Player other ); // Copy Assignment Operator
};


#endif

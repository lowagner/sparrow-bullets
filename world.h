#ifndef __WORLD__
#define __WORLD__

#include "basic.h"


void draw_textured_cube( SDL_Surface* texture, Sint32 halfsize = SP_ONE, Uint16 color = 0xFFF );
void draw_box( Sint32 halfsize_x = SP_ONE, 
               Sint32 halfsize_y = SP_ONE, 
               Sint32 halfsize_z = SP_ONE, 
               Uint16 color = 0xFFF );
    

class World
{

public:
    // biggest PHYSICS class
    // the general world which calculates everything, does dynamics, etc.
	btDynamicsWorld*		m_dworld;

private:
    // more physics classes

    // this collision shapes holds all the unique collision shapes.
    // so don't add to it unless you have a different shape to collide.
    // (it's probably ok, it's just less efficient.)
	btAlignedObjectArray<btCollisionShape*>	m_colshapes;

    // methods to calculate things...
	btBroadphaseInterface*	m_broadphase;

    // something to dispatch... collision events?
	btCollisionDispatcher*	m_dispatcher;

    // methods to calculate constraints
	btSequentialImpulseConstraintSolver*	m_solver;

    // not sure what this is
	btDefaultCollisionConfiguration* m_colconfig;
   
    // standard box shape
	btBoxShape* cubeshape;
    // standard box inertia -- or moment of inertia off-diagonals, not exactly sure!
    btVector3 cubeinertia;
	
public:

    World();

    void
    update( float dt );

    btRigidBody*
    add_cube( sbVector pos );

    btRigidBody*
    add_box( sbVector size, 
             sbVector pos,
             Uint32 mass_=0 );

    void remove_body( btRigidBody* rb );

    ~World();
};

class Cube
{
private:
    int iamdone;
    Uint16 color;

    sbVector lastpos;
    btQuaternion lastrot;

    btDynamicsWorld* m_dworld;
    btRigidBody* m_rb;

public:
    Cube( sbVector pos=sbVector(), Uint16 color_=0xFFF );

    void remove();
    
    int done(); // am i done and can be removed?
    
    void get_position_orientation( sbVector& pos, btQuaternion& rot );

    void add_to_world( World& world );

    void remove_from_world();

    int crashed(); // did i crash in the last iteration

    // translate/rotate the model-view matrix and then draw:
    void change_matrix_and_draw( SDL_Surface* texture = NULL ); 

    ~Cube();
};


class Box
{
private:
    int iamdone;
    Uint16 color;

    sbVector size;
    sbVector lastpos;
    btQuaternion lastrot;

    btDynamicsWorld* m_dworld;
    btRigidBody* m_rb;

public:
    Box( sbVector size_=sbVector(1,1,1), sbVector pos=sbVector(), Uint16 color_=0xFFF );

    void remove();
    
    int done(); // am i done and can be removed?
    
    void get_position_orientation( sbVector& pos, btQuaternion& rot );

    void add_to_world( World& world );

    void remove_from_world();

    int crashed(); // did i crash in the last iteration

    // translate/rotate the model-view matrix and then draw:
    void change_matrix_and_draw(); 

    ~Box();
};


#endif

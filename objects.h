#ifndef __OBJECTS__
#define __OBJECTS__

#include "physics.h"
#include "csparrow.h"


void draw_textured_cube( SDL_Surface* texture, Sint32 halfsize = SP_ONE, Uint16 color = 0xFFF );
void draw_box( Sint32 halfsize_x = SP_ONE, 
               Sint32 halfsize_y = SP_ONE, 
               Sint32 halfsize_z = SP_ONE, 
               Uint16 color = 0xFFF );

class Cube
{
private:
    int iamdone;
    Uint16 color;

    sbVector lastpos;
    btQuaternion lastrot;
    Sint32 lastorientation[16];

    btDynamicsWorld* m_dworld;
    btRigidBody* m_rb;

public:
    Cube( sbVector pos=sbVector(), Uint16 color_=0xFFFF );

    void remove();
    
    int done(); // am i done and can be removed?
    
    void get_position_orientation( sbVector& pos, btQuaternion& rot );

    void add_physics( Physics& physics );

    void remove_physics();

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
    Sint32 lastorientation[16];

    btDynamicsWorld* m_dworld;
    btRigidBody* m_rb;

public:
    Box( sbVector size_=sbVector(1,1,1), sbVector pos=sbVector(), Uint16 color_=0xFFF );

    void remove();
    
    int done(); // am i done and can be removed?
    
    void get_position_orientation( sbVector& pos, btQuaternion& rot );

    void add_physics( Physics& physics );

    void remove_physics();

    int crashed(); // did i crash in the last iteration

    // translate/rotate the model-view matrix and then draw:
    void change_matrix_and_draw(); 

    ~Box();
};


#endif

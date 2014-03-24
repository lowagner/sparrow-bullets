#ifndef __WORLD__
#define __WORLD__

#include "btBulletDynamicsCommon.h"
#include "meta.h" // for sparrow stuff

class World
{

private:
    SDL_Surface* checkertexture;

    // PHYSICS classes
	btDynamicsWorld*		m_dynamicsWorld;

	btCollisionShape*	m_shootBoxShape;

	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

	btBroadphaseInterface*	m_broadphase;

	btCollisionDispatcher*	m_dispatcher;

	btSequentialImpulseConstraintSolver*	m_solver;

	btDefaultCollisionConfiguration* m_collisionConfiguration;
    
// drawing methods, hidden to the metaworld.

    void draw_textured_box( Sint32 halfsize = SP_ONE, Uint16 color = 0xFFF );
    void draw_box( Sint32 halfsize = SP_ONE, Uint16 color = 0xFFF );
    
public:

    World();

    void
    update( float dt );

    void
    add_box( float x=0, float y=0, float z=0, 
             float size_x=1.0f, float size_y=1.0f, float size_z=1.0f,
             float mass_=1.0f, Uint16 color=0xFFF );
    
    void draw();

    ~World();
};

#endif

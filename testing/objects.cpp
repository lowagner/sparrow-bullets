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

BaseObject::BaseObject()
{
    std::cout << " calling base object constructor " << id << std::endl;
    debug = false;
    m_physics = NULL;
    m_rb = NULL;
}

BaseObject::~BaseObject()
{
    // even in inherited classes, this method is called.
    
    // WARNING WARNING.
    // remove_physics();
    // DO NOT UNCOMMENT THE ABOVE LINE.
    // DO NOT remove_physics in any of the child class destructors, either.
    // YOU HAVE BEEN WARNED.
    // we shouldn't remove physics except explicitly:  in a vector of objects,
    // c++ destroys and recreates all higher elements if you erase an internal element.
    // you have no way to check if c++ is deleting the physics of an object that should
    // still exist.

    m_physics = NULL;
    m_rb = NULL;
    // it's called after the inherited class' own destructor.
    // don't put any virtual methods in it.
    std::cout << " finished calling base object " << id << " destructor " << std::endl;
}

btVector3 
BaseObject::last_position()
{
    btVector3 pos;
    pos.setX( spFixedToFloat( lastpor[12] ) );
    pos.setY( spFixedToFloat( lastpor[13] ) );
    pos.setZ( spFixedToFloat( lastpor[14] ) );
    return pos;
}

btVector3 
BaseObject::last_velocity()
{
    return lastvelocity;
}

void
BaseObject::update_por( btScalar dt )
{ 
    // non kinematic, non dynamic object.  it does what it wants, regardless of physics

    // currently this does not rotate the object via lastomega, it just uses linear velocity of lastvelocity.

    // NON KINEMATIC, NON DYNAMIC OBJECTS.  they just move, they don't interact with physics
    lastposition += dt * lastvelocity;
    lastpor[12] = spFloatToFixed( lastposition.getX() ); 
    lastpor[13] = spFloatToFixed( lastposition.getY() ); 
    lastpor[14] = spFloatToFixed( lastposition.getZ() ); 

    // if desired to get angles moving, check out update_transform
}

void
BaseObject::update_transform( btScalar dt )
{
    // kinematic object updating its transform and the por

    lastposition += dt * lastvelocity;
    transform.setOrigin( lastposition );

    btScalar omegalength = lastomega.length2();
    if ( omegalength != btScalar(0.0) )
    {
        // rotate given the angular velocity...
        omegalength = btSqrt( omegalength ); 
        btScalar rotangle = omegalength*dt;
        btQuaternion drot( lastomega, rotangle );  
        btQuaternion rot = transform.getRotation();
        transform.setRotation( rot * drot );

        // fix the por from the transformation
        fix_por();
    }
    else
    {
        // no spinning, just velocity...
        lastpor[12] = spFloatToFixed( lastposition.getX() ); 
        lastpor[13] = spFloatToFixed( lastposition.getY() ); 
        lastpor[14] = spFloatToFixed( lastposition.getZ() ); 
    }

    // interface with the physics
    m_rb->getMotionState()->setWorldTransform( transform );
    m_rb->setCenterOfMassTransform( transform );
    //m_rb->setLinearVelocity( lastvelocity ); // maybe not necessary, unless things seem jittery
}

void
BaseObject::fix_transform()
{
    // uses lastpor and fixes transform from that.
    // ONLY should be used when sending a non-kinematic, non-dynamic object to the physics world

    btScalar btm[16]; // transform matrix in btScalar form
    for (int i=0; i<16; i++)
    {
        btm[i] = spFixedToFloat( lastpor[i] );
    }
    transform.setFromOpenGLMatrix( btm ); 
}

void
BaseObject::fix_por()
{
    // fixes lastpor using the transform

    // get the openGL matrix from the transform
    btScalar m[16];
    transform.getOpenGLMatrix(m); 
    // we will use it in the draw() method, but need it in sparrow coordinates
    for (int i=0; i<16; i++)
    {
        lastpor[i] = spFloatToFixed( m[i] );
    }
}

void
BaseObject::locate_and_move( btScalar dt )
{
    if (m_physics)
    {
        if ( kinematic )
        {   // KINEMATIC -- this guy is in the physics world but moved by fiat
            // change position by fiat
            update_transform( dt );  // update por/transform for kinematic objects
        }
        else
        {   // NOT kinematic, this guy is controlled by the physics world
            // get our world transform from the physics engine.
            m_rb->getMotionState()->getWorldTransform( transform );
            // get por from the transform
            fix_por();

            // also get all the fun velocities
            lastposition = transform.getOrigin();
            lastvelocity = m_rb->getLinearVelocity();
            lastomega = m_rb->getAngularVelocity();
        }
    }
    else
    {   
        // physics has nothing to do with our poor object.
        // but let it continue off into space with a constant velocity.
        update_por( dt ); 
    }
}

void 
BaseObject::rotate( btVector3 axis, btScalar angle )
{
    if (m_physics)
    {
        if ( kinematic )
        {
            btQuaternion qrot = transform.getRotation();
            btQuaternion dqrot( axis, angle );
            qrot *= dqrot;
            // perform the transformation 
            transform.setRotation( qrot );

            m_rb->getMotionState()->setWorldTransform( transform );
            m_rb->setCenterOfMassTransform( transform );
            
            // fix the visible position and rotation
            fix_por();
        }
        else 
        { // DYNAMIC
            std::cout << " WARNING, cannot rotate a dynamic object yet " << std::endl;
        }
    } 
    else
    { // NON kinematic, NON dynamic
        // make a btTransform from the por
        btScalar btm[16]; // old transform matrix in btScalar form
        for (int i=0; i<16; i++)
        {
            btm[i] = spFixedToFloat( lastpor[i] );
        }
        transform.setFromOpenGLMatrix( btm ); 

        // then get the rotations...
        btQuaternion qrot = transform.getRotation();
        btQuaternion dqrot( axis, angle );
        qrot *= dqrot;
        // perform the transformation 
        transform.setRotation( qrot );

        // set the transformation back into the lastpor open GL orientation matrix
        transform.getOpenGLMatrix( btm );
        for (int i=0; i<16; i++)
        {
            lastpor[i] = spFloatToFixed( btm[i] );
        }
    }
}

void 
BaseObject::rotateZ( btScalar angle )
{
    if (m_physics)
    {
        if ( kinematic )
        {
            btQuaternion qrot = transform.getRotation();
            btQuaternion dqrot( btVector3(0,0,1), angle );
            qrot *= dqrot;
            // perform the transformation 
            transform.setRotation( qrot );

            m_rb->getMotionState()->setWorldTransform( transform );
            m_rb->setCenterOfMassTransform( transform );
            
            // fix the visible position and rotation
            fix_por();
        }
        else 
        { // DYNAMIC
            std::cout << " WARNING, cannot rotate a dynamic object yet " << std::endl;
        }
    } 
    else
    { // non dynamic, non kinematic object
        Sint32 rad = spFloatToFixed( angle );
    //Rotation matrix: (from sparrowRender spRotateZ)
        Sint32 s = spSin( rad );
        Sint32 c = spCos( rad );

        Sint32 rotate[16];
        rotate[ 0] = c;
        rotate[ 4] = -s;
        rotate[ 8] = 0;
        rotate[12] = 0;
        rotate[ 1] = s;
        rotate[ 5] = c;
        rotate[ 9] = 0;
        rotate[13] = 0;
        rotate[ 2] = 0;
        rotate[ 6] = 0;
        rotate[10] = SP_ONE;
        rotate[14] = 0;
        rotate[ 3] = 0;
        rotate[ 7] = 0;
        rotate[11] = 0;
        rotate[15] = SP_ONE;

        Sint32 result[8];
        result[ 0] = spMul( lastpor[0], rotate[0] ) + spMul( lastpor[4], rotate[1] );
        result[ 1] = spMul( lastpor[1], rotate[0] ) + spMul( lastpor[5], rotate[1] );
        result[ 2] = spMul( lastpor[2], rotate[0] ) + spMul( lastpor[6], rotate[1] );
        result[ 3] = spMul( lastpor[3], rotate[0] ) + spMul( lastpor[7], rotate[1] );

        result[ 4] = spMul( lastpor[0], rotate[4] ) + spMul( lastpor[4], rotate[5] );
        result[ 5] = spMul( lastpor[1], rotate[4] ) + spMul( lastpor[5], rotate[5] );
        result[ 6] = spMul( lastpor[2], rotate[4] ) + spMul( lastpor[6], rotate[5] );
        result[ 7] = spMul( lastpor[3], rotate[4] ) + spMul( lastpor[7], rotate[5] );

        memcpy( lastpor, result, sizeof(Sint32)*8 );
    }
}


void
BaseObject::translate( btVector3 dist )
{
    lastpor[12] += spFloatToFixed( dist.getX() );
    lastpor[13] += spFloatToFixed( dist.getY() );
    lastpor[14] += spFloatToFixed( dist.getZ() );

    if (m_physics)
    {
        if ( kinematic ) 
        {
            transform.setOrigin( transform.getOrigin() + dist );
            // get the openGL matrix from the object
            m_rb->getMotionState()->setWorldTransform( transform );
            m_rb->setCenterOfMassTransform( transform );
        }
        else
        {
            std::cout << " WARNING, cannot translate a dynamic object yet " << std::endl;
        }
    }
}

//void
//BaseObject::transform( Sint32* m )
//{
//    btScalar dbtm[16]; // transformation matrix in btScalar form
//    btScalar obtm[16]; // old transform matrix in btScalar form
//    for (int i=0; i<16; i++)
//    {
//        dbtm[i] = spFixedToFloat( m[i] );
//        obtm[i] = spFixedToFloat( lastpor[i] );
//    }
//    btTransform transform;
//    transform.setFromOpenGLMatrix( obtm ); 
//    btTransform dtransform;
//    dtransform.setFromOpenGLMatrix( dbtm ); 
//    // perform the transformation 
//    transform *= dtransform;
//    
//    // set the transformation back into the lastpor open GL orientation matrix
//    transform.getOpenGLMatrix( obtm );
//    for (int i=0; i<16; i++)
//    {
//        lastpor[i] = spFloatToFixed( obtm[i] );
//    }
//
//    if (m_physics)
//    {
//        // get the openGL matrix from the object
//        m_rb->getMotionState()->setWorldTransform( transform );
//        m_rb->setCenterOfMassTransform( transform );
//    }
//}

//void
//BaseObject::set_por( Sint32* m )
//{
//    if (m_physics)
//    {
//        btScalar btm[16]; // transform matrix in btScalar form
//        for (int i=0; i<16; i++)
//        {
//            lastpor[i] = m[i];
//            btm[i] = spFixedToFloat( m[i] );
//        }
//        btTransform transform;
//        transform.setFromOpenGLMatrix( btm ); 
//
//        // get the openGL matrix from the object
//        m_rb->getMotionState()->setWorldTransform( transform );
//        m_rb->setCenterOfMassTransform( transform );
//    }
//    else
//    {
//        for (int i=0; i<16; i++)
//            lastpor[i] = m[i];
//    }
//}

//btTransform
//BaseObject::transform
//{
//    btScalar btm[16]; // transform matrix in btScalar form
//    for (int i=0; i<16; i++)
//    {
//        btm[i] = spFixedToFloat( lastpor[i] );
//    }
//    btTransform transform;
//    // get the openGL matrix from the lastpor, now in btscalar form
//    transform.setFromOpenGLMatrix( btm ); 
//    return transform;
//}


void
BaseObject::add_physics( Physics& physics )
{ }

bool
BaseObject::out_of_bounds( btVector3 outofbounds )
{
    if ( fabs(lastposition.getX()) > outofbounds.getX() )
        return true;
    if ( fabs(lastposition.getY()) > outofbounds.getY() )
        return true;
    if ( fabs(lastposition.getZ()) > outofbounds.getZ() )
        return true;
    return false;
}

void
BaseObject::remove_physics()
{
    if (m_physics)
    {
        m_physics->m_dworld->removeRigidBody(m_rb);
        delete m_rb->getMotionState();
        delete m_rb;
        m_physics = NULL;
    }
}


void
BaseObject::update( float dt )
{}

void
BaseObject::reset_camera( Sint32* matrix )
{
    memcpy(spGetMatrix(),matrix,16*sizeof(Sint32)); //reload camera matrix after every draw
}

void
BaseObject::draw( Sint32* original_camera_matrix, int alpha )
{}


Cube::Cube( btVector3 pos, Uint16 color_, SDL_Surface* texture_,
            btScalar mass_ )
{
    mass = mass_;
    id = 0;
    speed = 2.8;
    rotspeed = 0.5;
    canjump = false;
    onground = false;
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
    m_physics = NULL;
}


void
Cube::add_physics( Physics& physics )
{
    if (m_physics)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        // non-kinematic, non dynamic guy
        // grab the physics pointer
        m_physics = &physics;
        // we haven't been keeping up with the transforms
        // get the transform from the por
        fix_transform();
        // add physics
        m_rb = physics.add_cube( transform, lastvelocity, lastomega, mass );
        //init_physics_por();
        kinematic = (mass);
        std::cerr << " kinematic Cube is " << kinematic << std::endl;
    }
}


bool 
Cube::feet_on_something() // ray cast down in cube-coords
{
    btVector3 rayfrom( lastposition );
    // get the cube-z direction in world coordinates
    //btVector3 cubeup = m_rb->getWorldTransform().getBasis()[2];
    //cubeup.normalize(); // this shouldn't be necessary
    //btVector3 rayto = rayfrom - cubeup;

    btTransform cubeform;
    m_rb->getMotionState()->getWorldTransform( cubeform );
    btVector3 down = -cubeform.getBasis()[2];
    //btVector3 forward = cubeform.getBasis()[0];

    btVector3 rayto = rayfrom + down;


    ClosestNotMe ray(m_rb);

    m_physics->m_dworld->rayTest( rayfrom, rayto, ray ); 

    if ( ray.hasHit() )
        return true;
    else
        return false;
}

bool 
Cube::on_ground() // ray cast downwards in world-coords
{
    btVector3 rayfrom( lastposition );
    btVector3 rayto = rayfrom - btVector3(0,0,1);
    
    ClosestNotMe ray(m_rb);

    m_physics->m_dworld->rayTest( rayfrom, rayto, ray ); 

    if ( ray.hasHit() )
        return true;
    else
        return false;
}

void
Cube::jump()
{
//
    if ( canjump )
    {
        canjump = false;

        // get the cube-z direction in world coordinates
        btVector3 cubeup = m_rb->getWorldTransform().getBasis()[2];
//        cubeup.setX( 1.0* int( cubeup.x()*100 ) / 100 );
//        cubeup.setY( 1.0* int( cubeup.y()*100 ) / 100 );
//        cubeup.setZ( 1.0* int( cubeup.z()*100 ) / 100 );
//        cubeup.normalize(); // this shouldn't be necessary
        // add worldup to cubeup to get a jump vector
        btVector3 jumpvec = (btVector3(0,0,1) + cubeup);
        
        m_rb->applyCentralImpulse( jumpvec );
    }
}


void
Cube::walk( float dt )
{
    if (canjump && onground)
    {
        // get the cube-x direction in world coordinates
        btVector3 forward = m_rb->getWorldTransform().getBasis()[0];
        forward.normalize();
        std::cout << " forward = " << forward.x() << ", "
                                   << forward.y() << ", "
                                   << forward.z() << "\n";

//        btTransform cubeform;
//        m_rb->getMotionState()->getWorldTransform( cubeform );
//        //btVector3 down = -cubeform.getBasis()[2];
//        btVector3 forward = cubeform.getBasis()[0];

        m_rb->applyCentralImpulse( speed*forward*dt );
    }
}

void
Cube::turn( float dt, int dir )
{
//    if (canjump && onground)
    {
        // get the cube-z direction in world coordinates
        btVector3 up = m_rb->getWorldTransform().getBasis()[2];

        //btTransform cubeform;
        //m_rb->getMotionState()->getWorldTransform( cubeform );
        //btVector3 up = cubeform.getBasis()[2];

//        btVector3 up = m_rb->getInvInertiaTensorWorld().inverse()*
//                                ( m_rb->getWorldTransform().getBasis()*btVector3(0,0,1) );

        //m_rb->applyTorque( rotspeed*up*dir ); 
//        m_rb->applyTorqueImpulse( rotspeed*up*dt*dir ); 
        Physics* standin = m_physics;
        remove_physics();
        rotateZ( rotspeed*dt*dir ); 
        add_physics(*standin);

    }
}

void
Cube::quick_turn()
{

}

void
Cube::update( float dt )
{
    // find where cube is now.
    locate_and_move( dt );

    canjump = feet_on_something();
    onground = on_ground();
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
    std::cout << " finishing removing cube " << id << std::endl;
}

Cube::Cube( const Cube& other ) // copy constructor
{
    //std::cout << " calling cube copy constructor from " << other.id << " to " << id << std::endl;
    id = other.id + 100;
    speed = other.speed;
    rotspeed = other.rotspeed;
    debug = other.debug;
    texture = other.texture;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    color = other.color;
    lastvelocity = other.lastvelocity;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
}

Cube& 
Cube::operator = ( Cube other ) // Copy Assignment Operator
{
    kinematic = other.kinematic;
    //std::cout << " calling cube copy assignment from " << other.id << " to " << id << std::endl;
    id = other.id + 100;
    mass = other.mass;
    speed = other.speed;
    rotspeed = other.rotspeed;
    debug = other.debug;
    texture = other.texture;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
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
    std::cerr << " sizex = " << sizex << std::endl;
    sizey = spFloatToFixed(size.y());
    sizez = spFloatToFixed(size.z());

    color = color_;
    m_physics = NULL;
}


void
Box::add_physics( Physics& physics )
{
    if (m_physics)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        // non-kinematic, non dynamic guy
        // grab the physics pointer
        m_physics = &physics;
        // we haven't been keeping up with the transforms
        // get the transform from the por
        fix_transform();
        // add physics
        m_rb = physics.add_box( size, transform, lastvelocity, lastomega, mass );
        //init_physics_por();
        kinematic = (mass);
        std::cerr << " kinematic Box is " << kinematic << std::endl;
    }
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
        draw_ramp( sizex, sizey, sizez, color );
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
    //std::cout << " calling box copy constructor " << std::endl;
    id = other.id;
    debug = other.debug;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    size = other.size;
    color = other.color;
    lastvelocity = other.lastvelocity;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
}

Box& 
Box::operator = ( Box other ) // Copy Assignment Operator
{
    //std::cout << " calling box copy assignment " << std::endl;
    kinematic = other.kinematic;
    m_physics = other.m_physics;
    mass = other.mass;
    m_rb = other.m_rb;
    debug = other.debug;
    id = other.id;
    size = other.size;
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
    m_physics = NULL;
}

void
Ramp::add_physics( Physics& physics )
{
    if (m_physics)
    {
        // do we want to readd it by destroying it first?
        // just ignore for now.
    }
    else
    {
        // non-kinematic, non dynamic guy
        // grab the physics pointer
        m_physics = &physics;
        // we haven't been keeping up with the transforms
        // get the transform from the por
        fix_transform();
        // add physics
        m_rb = physics.add_ramp( size, transform, lastvelocity, lastomega, mass );
        //init_physics_por();
        kinematic = (mass);
        std::cerr << " kinematic Ramp is " << kinematic << std::endl;
    }
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
    id = other.id;
    debug = other.debug;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    size = other.size;
    color = other.color;
    lastvelocity = other.lastvelocity;
    for ( int i=0; i<16; i++ )
    {
        lastpor[i] = other.lastpor[i];
    }
}

Ramp& 
Ramp::operator = ( Ramp other ) // Copy Assignment Operator
{
    //std::cout << " calling ramp copy assignment " << std::endl;
    kinematic = other.kinematic;
    m_physics = other.m_physics;
    m_rb = other.m_rb;
    id = other.id;
    debug = other.debug;
    size = other.size;
    color = other.color;
    mass = other.mass;
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

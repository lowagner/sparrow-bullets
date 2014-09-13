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
    physics = NULL;
    body = NULL;
    mass = 0;
    dynamics = 0;
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

    physics = NULL;
    body = NULL;
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
    body->getMotionState()->setWorldTransform( transform );
    body->setCenterOfMassTransform( transform );
    //body->setLinearVelocity( lastvelocity ); // maybe not necessary, unless things seem jittery
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


//        switch ( dynamics ) 
//        {
//          case -2:
//            // ghost object with kinematics
//            update_transform( dt );  // update por/transform for kinematic objects
//            break; // keep this break, switch-stuff.
//          case -1:
//            // ghost object without movement
//            break; // keep this break, switch-stuff.
//          case 0:
//            // non dynamic object, shouldn't happen
//            std::cerr << " Dynamics should not be zero if physics is on " << std::endl;
//            break; // keep this break, switch-stuff.
//          case 1:
//            // physics object without movement, i.e. static
//            break; // keep this break, switch-stuff.
//          case 2:
//            // physics object with movement, i.e. kinematic
//            update_transform( dt );  // update por/transform for kinematic objects
//            break; // keep this break, switch-stuff.
//          case 3:
//            // physics object determined fully by physics world, i.e. dynamic
//            break; // keep this break, switch-stuff.
//          default:
//            // non dynamic object, shouldn't happen
//            std::cerr << " Dynamics unknown, but physics is on " << std::endl;
//      }


void
BaseObject::locate_and_move( btScalar dt )
{
    if (physics)
    {
        switch ( dynamics ) 
        {
          case 1:
          {
            // physics object without movement, i.e. static
            break; // keep this break, switch-stuff.
          }
          case 2:
          {
            // physics object with movement, i.e. kinematic
            update_transform( dt );  // update por/transform for kinematic objects
            break; // keep this break, switch-stuff.
          }
          case 3:
          {
            // physics object determined fully by physics world, i.e. dynamic
            body->getMotionState()->getWorldTransform( transform );
            // get por from the transform
            fix_por();

            // also get all the fun velocities
            lastposition = transform.getOrigin();
            lastvelocity = body->getLinearVelocity();
            lastomega = body->getAngularVelocity();
//            btRigidBody* rbody = dynamic_cast<btRigidBody*> body;
//            if ( rbody )
//            {
//                rbody->getMotionState()->getWorldTransform( transform );
//                // get por from the transform
//                fix_por();
//
//                // also get all the fun velocities
//                lastposition = transform.getOrigin();
//                lastvelocity = rbody->getLinearVelocity();
//                lastomega = rbody->getAngularVelocity();
//            }
//            else
//                std::cerr << " WARNING!  Dynamics3 does not have rigidbody" << std::endl;
            break; // keep this break, switch-stuff.
          }
          default:
          {
            // non dynamic object, shouldn't happen
            std::cerr << " WARNING!  Dynamics unknown, but physics is on " << std::endl;
          }
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
    if (physics)
    {
        short int dyn = abs( dynamics );
        switch ( dyn ) 
        {
        case 1:
        {
            // physics object without movement, i.e. static
            break; // keep this break, switch-stuff.
        }
        case 2:
        {
            // physics object with movement, i.e. kinematic
            btQuaternion qrot = transform.getRotation();
            btQuaternion dqrot( axis, angle );
            qrot *= dqrot;
            // perform the transformation 
            transform.setRotation( qrot );

            body->getMotionState()->setWorldTransform( transform );
            body->setCenterOfMassTransform( transform );
            
            // fix the visible position and rotation
            fix_por();
            break; // keep this break, switch-stuff.
        }
        case 3:
        {
            // physics object determined fully by physics world, i.e. dynamic
            std::cerr << " WARNING, cannot rotate a dynamic object yet " << std::endl;
            break; // keep this break, switch-stuff.
        }
        }
    } 
    else
    {   // NON kinematic, NON dynamic

        // PROBABLY should just use sparrow3d to rotate. see below

//        // make a btTransform from the por
//        btScalar btm[16]; // old transform matrix in btScalar form
//        for (int i=0; i<16; i++)
//        {
//            btm[i] = spFixedToFloat( lastpor[i] );
//        }
//        transform.setFromOpenGLMatrix( btm ); 
//
//        // then get the rotations...
//        btQuaternion qrot = transform.getRotation();
//        btQuaternion dqrot( axis, angle );
//        qrot *= dqrot;
//        // perform the transformation 
//        transform.setRotation( qrot );
//
//        // set the transformation back into the lastpor open GL orientation matrix
//        transform.getOpenGLMatrix( btm );
//        for (int i=0; i<16; i++)
//        {
//            lastpor[i] = spFloatToFixed( btm[i] );
//        }

    
        //Rotation matrix:
        Sint32 x = spFloatToFixed( axis.x() );
        Sint32 y = spFloatToFixed( axis.y() );
        Sint32 z = spFloatToFixed( axis.z() );
        Sint32 rad = spFloatToFixed( angle );

        // following code copied and modified from sparrowRenderer.c
        Sint32 s = spSin( rad );
        Sint32 c = spCos( rad );
        Sint32 l = spSqrt( spMul( x, x )
                           + spMul( y, y )
                           + spMul( z, z ) );
        if ( l == 0 )
            return;
        x = spDiv( x, l );
        y = spDiv( y, l );
        z = spDiv( z, l );
        Sint32 rotate[16];
        rotate[ 0] = c + spMul( spMul( x, x ), ( SP_ONE ) - c );
        rotate[ 4] =   spMul( spMul( x, y ), ( SP_ONE ) - c ) - spMul( z, s );
        rotate[ 8] =   spMul( spMul( x, z ), ( SP_ONE ) - c ) + spMul( y, s );
        rotate[12] = 0;
        rotate[ 1] =   spMul( spMul( y, x ), ( SP_ONE ) - c ) + spMul( z, s );
        rotate[ 5] = c + spMul( spMul( y, y ), ( SP_ONE ) - c );
        rotate[ 9] =   spMul( spMul( y, z ), ( SP_ONE ) - c ) - spMul( x, s );
        rotate[13] = 0;
        rotate[ 2] =   spMul( spMul( z, x ), ( SP_ONE ) - c ) - spMul( y, s );
        rotate[ 6] =   spMul( spMul( z, y ), ( SP_ONE ) - c ) + spMul( x, s );
        rotate[10] = c + spMul( spMul( z, z ), ( SP_ONE ) - c );
        rotate[14] = 0;
        rotate[ 3] = 0;
        rotate[ 7] = 0;
        rotate[11] = 0;
        rotate[15] = SP_ONE;

        Sint32 result[12];
        // matrix multiplication!
        result[ 0] = ( lastpor[ 0] >> SP_HALF_ACCURACY ) * ( rotate[ 0] >> SP_HALF_ACCURACY ) + ( lastpor[ 4] >> SP_HALF_ACCURACY ) * ( rotate[ 1] >> SP_HALF_ACCURACY ) + ( lastpor[ 8] >> SP_HALF_ACCURACY ) * ( rotate[ 2] >> SP_HALF_ACCURACY );
        result[ 1] = ( lastpor[ 1] >> SP_HALF_ACCURACY ) * ( rotate[ 0] >> SP_HALF_ACCURACY ) + ( lastpor[ 5] >> SP_HALF_ACCURACY ) * ( rotate[ 1] >> SP_HALF_ACCURACY ) + ( lastpor[ 9] >> SP_HALF_ACCURACY ) * ( rotate[ 2] >> SP_HALF_ACCURACY );
        result[ 2] = ( lastpor[ 2] >> SP_HALF_ACCURACY ) * ( rotate[ 0] >> SP_HALF_ACCURACY ) + ( lastpor[ 6] >> SP_HALF_ACCURACY ) * ( rotate[ 1] >> SP_HALF_ACCURACY ) + ( lastpor[10] >> SP_HALF_ACCURACY ) * ( rotate[ 2] >> SP_HALF_ACCURACY );
        result[ 3] = ( lastpor[ 3] >> SP_HALF_ACCURACY ) * ( rotate[ 0] >> SP_HALF_ACCURACY ) + ( lastpor[ 7] >> SP_HALF_ACCURACY ) * ( rotate[ 1] >> SP_HALF_ACCURACY ) + ( lastpor[11] >> SP_HALF_ACCURACY ) * ( rotate[ 2] >> SP_HALF_ACCURACY );

        result[ 4] = ( lastpor[ 0] >> SP_HALF_ACCURACY ) * ( rotate[ 4] >> SP_HALF_ACCURACY ) + ( lastpor[ 4] >> SP_HALF_ACCURACY ) * ( rotate[ 5] >> SP_HALF_ACCURACY ) + ( lastpor[ 8] >> SP_HALF_ACCURACY ) * ( rotate[ 6] >> SP_HALF_ACCURACY );
        result[ 5] = ( lastpor[ 1] >> SP_HALF_ACCURACY ) * ( rotate[ 4] >> SP_HALF_ACCURACY ) + ( lastpor[ 5] >> SP_HALF_ACCURACY ) * ( rotate[ 5] >> SP_HALF_ACCURACY ) + ( lastpor[ 9] >> SP_HALF_ACCURACY ) * ( rotate[ 6] >> SP_HALF_ACCURACY );
        result[ 6] = ( lastpor[ 2] >> SP_HALF_ACCURACY ) * ( rotate[ 4] >> SP_HALF_ACCURACY ) + ( lastpor[ 6] >> SP_HALF_ACCURACY ) * ( rotate[ 5] >> SP_HALF_ACCURACY ) + ( lastpor[10] >> SP_HALF_ACCURACY ) * ( rotate[ 6] >> SP_HALF_ACCURACY );
        result[ 7] = ( lastpor[ 3] >> SP_HALF_ACCURACY ) * ( rotate[ 4] >> SP_HALF_ACCURACY ) + ( lastpor[ 7] >> SP_HALF_ACCURACY ) * ( rotate[ 5] >> SP_HALF_ACCURACY ) + ( lastpor[11] >> SP_HALF_ACCURACY ) * ( rotate[ 6] >> SP_HALF_ACCURACY );

        result[ 8] = ( lastpor[ 0] >> SP_HALF_ACCURACY ) * ( rotate[ 8] >> SP_HALF_ACCURACY ) + ( lastpor[ 4] >> SP_HALF_ACCURACY ) * ( rotate[ 9] >> SP_HALF_ACCURACY ) + ( lastpor[ 8] >> SP_HALF_ACCURACY ) * ( rotate[10] >> SP_HALF_ACCURACY );
        result[ 9] = ( lastpor[ 1] >> SP_HALF_ACCURACY ) * ( rotate[ 8] >> SP_HALF_ACCURACY ) + ( lastpor[ 5] >> SP_HALF_ACCURACY ) * ( rotate[ 9] >> SP_HALF_ACCURACY ) + ( lastpor[ 9] >> SP_HALF_ACCURACY ) * ( rotate[10] >> SP_HALF_ACCURACY );
        result[10] = ( lastpor[ 2] >> SP_HALF_ACCURACY ) * ( rotate[ 8] >> SP_HALF_ACCURACY ) + ( lastpor[ 6] >> SP_HALF_ACCURACY ) * ( rotate[ 9] >> SP_HALF_ACCURACY ) + ( lastpor[10] >> SP_HALF_ACCURACY ) * ( rotate[10] >> SP_HALF_ACCURACY );
        result[11] = ( lastpor[ 3] >> SP_HALF_ACCURACY ) * ( rotate[ 8] >> SP_HALF_ACCURACY ) + ( lastpor[ 7] >> SP_HALF_ACCURACY ) * ( rotate[ 9] >> SP_HALF_ACCURACY ) + ( lastpor[11] >> SP_HALF_ACCURACY ) * ( rotate[10] >> SP_HALF_ACCURACY );

        memcpy( lastpor, result, sizeof(Sint32)*12 );
    }
}

void 
BaseObject::rotateZ( btScalar angle )
{
    if (physics)
    {
        short unsigned int dyn = abs( dynamics );
        switch ( dyn ) 
        {
          case 1:
          {
            // physics object without movement, i.e. static
            break; // keep this break, switch-stuff.
          }
          case 2:
          {
            // physics object with movement, i.e. kinematic
            btQuaternion qrot = transform.getRotation();
            btQuaternion dqrot( btVector3(0,0,1), angle );
            qrot *= dqrot;
            // perform the transformation 
            transform.setRotation( qrot );

            body->getMotionState()->setWorldTransform( transform );
            body->setCenterOfMassTransform( transform );
            
            // fix the visible position and rotation
            fix_por();
            break; // keep this break, switch-stuff.
          }
          case 3:
          {
            // physics object determined fully by physics world, i.e. dynamic
            std::cerr << " WARNING, cannot rotate a dynamic object yet " << std::endl;
            break; // keep this break, switch-stuff.
          }
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

    if (physics)
    {
        short unsigned int dyn = abs( dynamics );
        switch ( dyn ) 
        {
          case 1:
          {
            // physics object without movement, i.e. static
            break; // keep this break, switch-stuff.
          }
          case 2:
          {
            // physics object with movement, i.e. kinematic
            transform.setOrigin( transform.getOrigin() + dist );
            // get the openGL matrix from the object
            body->getMotionState()->setWorldTransform( transform );
            body->setCenterOfMassTransform( transform );
            break; // keep this break, switch-stuff.
          }
          case 3:
          {
            // physics object determined fully by physics world, i.e. dynamic
            std::cerr << " WARNING, cannot translate a dynamic object yet " << std::endl;
            break; // keep this break, switch-stuff.
          }
        }
    }
}

void
BaseObject::add_physics( Physics& new_physics, short int dynamics_ )
{ 
std::cerr << " base object add_physics " << std::endl;
}

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
    if (physics)
    {
        physics->dworld->removeRigidBody(body);
        delete body->getMotionState();
        delete body;
        physics = NULL;
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
    std::cout << " Cube mass = " << mass << std::endl;
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
    std::cerr << " dynamics Cube is " << dynamics << std::endl;
}


bool 
Cube::feet_on_something() // ray cast down in cube-coords
{
    btVector3 rayfrom( lastposition );
    // get the cube-z direction in world coordinates
    //btVector3 cubeup = body->getWorldTransform().getBasis()[2];
    //cubeup.normalize(); // this shouldn't be necessary
    //btVector3 rayto = rayfrom - cubeup;

    btTransform cubeform;
    body->getMotionState()->getWorldTransform( cubeform );
    btVector3 down = -cubeform.getBasis()[2];
    //btVector3 forward = cubeform.getBasis()[0];

    btVector3 rayto = rayfrom + down;


    ClosestNotMe ray(body);

    physics->dworld->rayTest( rayfrom, rayto, ray ); 

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
    
    ClosestNotMe ray(body);

    physics->dworld->rayTest( rayfrom, rayto, ray ); 

    if ( ray.hasHit() )
        return true;
    else
        return false;
}

void
Cube::jump()
{
//
    if ( canjump && (dynamics == 3) )
    {
        canjump = false;

        // get the cube-z direction in world coordinates
        btVector3 cubeup = body->getWorldTransform().getBasis()[2];
        // add worldup to cubeup to get a jump vector
        btVector3 jumpvec = (btVector3(0,0,1) + cubeup);
        
        body->applyCentralImpulse( jumpvec );
//        btRigidBody* rbody = dynamic_cast<btRigidBody*> body;
//        if ( rbody )
//        {
//            canjump = false;
//
//            // get the cube-z direction in world coordinates
//            btVector3 cubeup = rbody->getWorldTransform().getBasis()[2];
//            // add worldup to cubeup to get a jump vector
//            btVector3 jumpvec = (btVector3(0,0,1) + cubeup);
//            
//            rbody->applyCentralImpulse( jumpvec );
//        }
//        else
//        {
//            std::cerr << " dynamics3 object is not a rigid body, WARNING! " << std::endl;
//        }
    }
}


void
Cube::walk( float dt )
{
    if ( canjump && onground && (dynamics == 3) )
    {
        // get the cube-x direction in world coordinates
        btVector3 forward = body->getWorldTransform().getBasis()[0];
        forward.normalize();
//        std::cout << " forward = " << forward.x() << ", "
//                                   << forward.y() << ", "
//                                   << forward.z() << "\n";

        body->applyCentralImpulse( speed*forward*dt );
//        btRigidBody* rbody = dynamic_cast<btRigidBody*>( body );
//        if ( rbody )
//        {
//            // get the cube-x direction in world coordinates
//            btVector3 forward = rbody->getWorldTransform().getBasis()[0];
//            forward.normalize();
//            std::cout << " forward = " << forward.x() << ", "
//                                       << forward.y() << ", "
//                                       << forward.z() << "\n";
//
//    //        btTransform cubeform;
//    //        body->getMotionState()->getWorldTransform( cubeform );
//    //        //btVector3 down = -cubeform.getBasis()[2];
//    //        btVector3 forward = cubeform.getBasis()[0];
//
//            rbody->applyCentralImpulse( speed*forward*dt );
//        }
//        else
//        {
//            std::cerr << " dynamics3 object is not a rigid body, WARNING! " << std::endl;
//        }
    }
}

void
Cube::turn( float dt, int dir )
{
//    if (canjump && onground)
    {
        // get the cube-z direction in world coordinates
        btVector3 up = body->getWorldTransform().getBasis()[2];

        //btTransform cubeform;
        //body->getMotionState()->getWorldTransform( cubeform );
        //btVector3 up = cubeform.getBasis()[2];

//        btVector3 up = body->getInvInertiaTensorWorld().inverse()*
//                                ( body->getWorldTransform().getBasis()*btVector3(0,0,1) );

        //body->applyTorque( rotspeed*up*dir ); 
//        body->applyTorqueImpulse( rotspeed*up*dt*dir ); 
        Physics* standin = physics;
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
    dynamics = other.dynamics;
    mass = other.mass;
    id = other.id + 100;
    speed = other.speed;
    rotspeed = other.rotspeed;
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
    speed = other.speed;
    rotspeed = other.rotspeed;
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
    std::cerr << " dynamics Box is " << dynamics << std::endl;
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
    std::cerr << " dynamics Ramp is " << dynamics << std::endl;
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

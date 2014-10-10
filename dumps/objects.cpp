/*
Check the LICENSE file included for copyright information.  
*/
#include "objects.h"
#include "shapes.h" // necessary to do separately, for Player class.


BaseObject::BaseObject()
{
//    std::cout << " calling base object constructor " << id << std::endl;
    selfa = 256;
    text.clear();
    debug = false;
    physics = NULL;
    body = NULL;
    mass = 0;
    dynamics = 0;
    lastposition = btVector3(); // position
    lastvelocity = btVector3(); // linear velocity
    lastomega = btVector3(); //angular velocity
    size = btVector3(1,1,1); // size
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
//    std::cout << " finished calling base object " << id << " destructor " << std::endl;
    text.clear();
}

void 
BaseObject::set_alpha( short int selfa_ )
{
    if ( selfa_ > 255 )
        selfa = 255;
    else if ( selfa_ < 0 )
        selfa = 0;
    else
        selfa = selfa_;
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
BaseObject::impulse( btVector3 forcedt )
{
  if (mass != 0.f)
  {
    btVector3 dv = forcedt / mass;
    lastvelocity += dv;
    if ( physics )
    {
        if ( dynamics == 3 )
        {
            body->forceActivationState(1);
            body->applyCentralImpulse( forcedt );
        }
    }
  }
}

void
BaseObject::activate()
{
    body->forceActivationState(1);
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
BaseObject::draw( SDL_Surface* screen, Sint32* original_camera_matrix, spFontPointer font, int alpha_ )
{}


Player::Player( btVector3 pos, Uint16 color_, SDL_Surface* texture_,
                btScalar mass_ )
{
    object = new Cube( pos,  // 
                       color_,  // hero's color
                       13374340,  // hero's id:  elite hero.
                       texture_, true,  // draw texture flipped by default.  can change that.
                       mass_ );
    init();
}

Player::Player( BaseObject object_ )
{
    object = &object_;
    init();
}

void
Player::init()
{
    maxwalkspeed2 = 50;
    walkacceleration = 2.1;
    maxrotspeed2 = 15;
    rotacceleration = 7;
    flyingrotacceleration = 5;
    siderotacceleration = 0.1;
    sidewalkacceleration = 1.8;
    kickrotimpulse = 20;
    kickupimpulse = 0.5;
    canjump = false;
    onground = false;
    jumpimpulse = 15;
}

void
Player::update( float dt )
{
    object->update( dt );

    check_surroundings();
}

btVector3
Player::get_position()
{
    return object->lastposition;
}

btVector3
Player::get_forward()
{
    btVector3 forward = object->body->getWorldTransform().getBasis() * btVector3(1,0,0);
    forward.normalize();
    return forward;
}

btVector3
Player::get_side()
{
    btVector3 side = ( object->body->getWorldTransform().getBasis()* btVector3(0,1,0) );
    side.normalize();
    return side;
}

btVector3
Player::get_up()
{
    btVector3 up = object->body->getWorldTransform().getBasis() * btVector3(0,0,1);
    up.normalize();
    return up;
}

void
Player::draw_debug()
{
    btVector3 forward = get_forward();
    btVector3 side = get_side();
    btVector3 up = get_up();
    forward *= 2;
    side *= 2;
    up *= 2;
    
    Sint32 x = spFloatToFixed( object->lastposition.x() );
    Sint32 y = spFloatToFixed( object->lastposition.y() );
    Sint32 z = spFloatToFixed( object->lastposition.z() );
    std::cerr << "x,y,z = " << x << ", " << y << ", " << z << "; " << std::endl;
    std::cerr << " forward = " << forward.x() << ", " << forward.y() << ", " << forward.z() << "; " << std::endl;
    std::cerr << " side = " << side.x() << ", " << side.y() << ", " << side.z() << "; " << std::endl;
    std::cerr << " up = " << up.x() << ", " << up.y() << ", " << up.z() << "; " << std::endl;


    spLine3D(x,y,z, 
             x+spFloatToFixed(forward.x()),
             y+spFloatToFixed(forward.y()),
             z+spFloatToFixed(forward.z()), 0xF00F);
    spLine3D(x,y,z, 
             x+spFloatToFixed(side.x()),
             y+spFloatToFixed(side.y()),
             z+spFloatToFixed(side.z()), 0x0F0F);
    spLine3D(x,y,z, 
             x+spFloatToFixed(up.x()),
             y+spFloatToFixed(up.y()),
             z+spFloatToFixed(up.z()), 0x00FF);
}

int
Player::walk( float dt, int dir )
{
    if ( onground )
    {  
        if (canjump && (topsideup==1))
        {
            if (object->dynamics == 3)
            {
                // get the cube-x direction in world coordinates
                btVector3 forward = get_forward();

                float veldiff2 = maxwalkspeed2- (object->lastvelocity.length2());
                if ( veldiff2 > 0 )
                {
                    object->body->forceActivationState(1);
                    object->body->applyCentralImpulse( -walkacceleration*veldiff2*forward*dt*dir );
                }
        //        else
        //            std::cerr << "moving too fast, can't walk properly" << std::endl;
            }
        }
        else 
        {
        // on ground, but in a bad spot
            //std::cerr << " player landed badly, can't walk " << std::endl;
            if ( facesideup==0 && topsideup == 0 )
            {
                //  trouble maker.  one of your sides is pointing up
                btVector3 forward = get_forward();
                
                // do the "woobwoobwoobwoob" move, move forward ...
                float veldiff2 = 0.5*maxwalkspeed2- (object->lastvelocity.length2());
                if ( veldiff2 > 0 )
                {
                    object->body->forceActivationState(1);
                    object->body->applyCentralImpulse( -sidewalkacceleration*veldiff2*forward*dt*dir );
                }

                // do the "woobwoobwoobwoob" move, ... and spin around
                float omegadiff2 = 0.5*maxrotspeed2- (object->lastomega.length2()); 
                if ( omegadiff2 > 0 )
                {
                    float multiplier = get_side().z();
                    if ( multiplier > 0 )
                        multiplier = 1;
                    else
                        multiplier = -1;

                    object->body->applyTorqueImpulse( multiplier*siderotacceleration*omegadiff2*btVector3(0,0,1)*dir ); 
                }
            }
            else
            {
                // either you're on your head or you're on your face or you're on your back
                btVector3 axis = get_side();
                float omegadiff2 = maxrotspeed2- (object->lastomega.length2());
                if ( omegadiff2 > 0 )
                {
                    object->body->forceActivationState(1);
                    object->body->applyTorqueImpulse( -kickrotimpulse*axis*dir ); 
                    object->body->applyCentralImpulse( btVector3(0,0,kickupimpulse) );
                    onground = false;
                    return 0; // return if we should stop input from the button
                }

            }
        }
    }
    else
    {
    // in the air
        //std::cerr << " player in the air, can't walk " << std::endl;
        btVector3 sideaxis = get_side();
        
        float omegadiff2 = maxrotspeed2- (object->lastomega.length2());
        if ( omegadiff2 > 0 )
        {
            object->body->forceActivationState(1);
            object->body->applyTorqueImpulse( -flyingrotacceleration*sideaxis*dt*dir ); 
        }

    }
    return dir; // +-1 if continuous motion is ok to continue.  0 if not.
}

int
Player::turn( float dt, int dir )
{
    if ( onground && ( (topsideup == 0 ) || ( facesideup ) ) )
    {   // very bad landing.  player's head isn't up (or down), or he can't jump
        //std::cerr << " player landed badly, can't turn " << std::endl;
        if ( facesideup == 0 )
        {   // we are on our side somewhere
            //std::cerr << " player landed on side, can't turn " << std::endl; 
            btVector3 axis = get_forward();
            float omegadiff2 = maxrotspeed2- (object->lastomega.length2());
            if ( omegadiff2 > 0 )
            {
                object->body->forceActivationState(1);
                object->body->applyTorqueImpulse( kickrotimpulse*axis*dir ); 
                object->body->applyCentralImpulse( btVector3(0,0,kickupimpulse) );
                onground = false;
                return 0; // return if we should stop input from the button
            }
        }
        else
        {   // either facing the sky or the ground
            //std::cerr << " player on face/back, " << facesideup << "; can't turn " << std::endl;
            // turn cube in face direction
            btVector3 axis = get_forward();

            float omegadiff2 = maxrotspeed2- (object->lastomega.length2());
            if ( omegadiff2 > 0 )
            {
                object->body->forceActivationState(1);
                object->body->applyTorqueImpulse( -rotacceleration*(omegadiff2)*axis*dt*dir ); 
            }

        }
    }
    else
    {   // player is fine, on the ground or in the air, but able to turn freely.
        // get the cube-z direction in world coordinates
        btVector3 up = get_up();

        float omegadiff2 = maxrotspeed2- (object->lastomega.length2());
        if ( omegadiff2 > 0 )
        {
            object->body->forceActivationState(1);
            object->body->applyTorqueImpulse( -rotacceleration*(omegadiff2)*up*dt*dir ); 
        }
        // else
        // std::cerr << " player is spinning out of control " << std::endl;
    }
    return dir; // +-1 if continuous motion is ok to continue.  0 if not.
}

int
Player::quick_turn()
{
//        btVector3 up = object->body->getWorldTransform().getBasis()[2];
//        Physics* standin = object->physics;
//        object->remove_physics();
//        object->rotateZ( rotspeed*dt*dir ); 
//        object->fix_transform(); // need to fix transform since there was no dynamics
//        object->add_physics(*standin);
    return 1; // 1 if continuous motion is ok to continue.  0 if not.
}


void 
Player::check_surroundings() 
{
    // FIRST
    // check if our feet are on something...    
    // ray cast down in cube-coords

    btVector3 rayfrom( object->lastposition );
    // get the cube-z direction in world coordinates
    //btVector3 cubeup = body->getWorldTransform().getBasis()[2];
    //cubeup.normalize(); // this shouldn't be necessary
    //btVector3 rayto = rayfrom - cubeup;

    btTransform cubeform;
    btVector3 down = -get_up();
    down *= object->size.z()*1.1;
   
    // in the meantime, check if our head is rightside up
    float result = -down.z(); //dot( btVector3(0,0,-1) );
    if ( result > 0.1f )
        topsideup = 1; // standing on our feet
    else if ( result < -0.1f )
        topsideup = -1; // standing on our head
    else
        topsideup = 0;

    if (topsideup == 0)
    {
        btVector3 forward = get_forward();
        
        result = forward.z(); //dot( btVector3(0,0,1) );
        //std::cerr << " face dotted into up is " << result << std::endl;
        if ( result > 0.1f )
            facesideup = 1;
        else if ( result < -0.1f )
            facesideup = -1;
        else
            facesideup = 0;
    }
    else
    {
        // topside is either up or down, in either case the face can't be.
        facesideup = 0;
    }

    btVector3 rayto = rayfrom + down;
    ClosestNotMe ray(object->body);

    object->physics->dworld->rayTest( rayfrom, rayto, ray ); 

    canjump = ( ray.hasHit() );
    
    

    // SECOND
    // check if we're on the ground.
    // ray cast downwards in world-coords

    rayto = rayfrom - 1.2*btVector3(0,0,object->size.z());

    object->physics->dworld->rayTest( rayfrom, rayto, ray ); 

    onground = ( ray.hasHit() );
}

bool
Player::on_ground()
{
    return onground;
}

int
Player::topside_up()
{
    return topsideup;
}

int
Player::facing_up()
{
    return facesideup;
}

int
Player::jump()
{
//
    if ( canjump && (object->dynamics == 3) )
    {
        canjump = false;
        onground = false;

        object->body->forceActivationState(1);

        // get the cube-z direction in world coordinates
        btVector3 cubeup = get_up();
        // add worldup to cubeup to get a jump vector
        btVector3 jumpvec = (btVector3(0,0,1) /* it'd be nice to multiply by friction of object at feet here */+ cubeup);
        
        object->body->applyCentralImpulse( jumpvec*jumpimpulse );
        return 0; // stop holding down jump button
    }
    return 1; // if it's ok to keep holding down jump button
}

Player::~Player()
{
    object = NULL;
}

Player::Player( const Player& other ) // copy constructor
{
    //std::cout << " calling cube copy constructor from " << other.id << " to " << id << std::endl;
    object = other.object;
    maxwalkspeed2 = other.maxwalkspeed2;
    walkacceleration = other.walkacceleration;
    maxrotspeed2 = other.maxrotspeed2;
    rotacceleration = other.rotacceleration;
    jumpimpulse = other.jumpimpulse;
    flyingrotacceleration = other.flyingrotacceleration;
    kickupimpulse = other.kickupimpulse;
    kickrotimpulse = other.kickrotimpulse;
    siderotacceleration = other.siderotacceleration;
    sidewalkacceleration = other.sidewalkacceleration;
}

Player& 
Player::operator = ( Player other ) // Copy Assignment Operator
{
    object = other.object;
    maxwalkspeed2 = other.maxwalkspeed2;
    walkacceleration = other.walkacceleration;
    maxrotspeed2 = other.maxrotspeed2;
    rotacceleration = other.rotacceleration;
    jumpimpulse = other.jumpimpulse;
    flyingrotacceleration = other.flyingrotacceleration;
    kickupimpulse = other.kickupimpulse;
    kickrotimpulse = other.kickrotimpulse;
    siderotacceleration = other.siderotacceleration;
    sidewalkacceleration = other.sidewalkacceleration;
    return *this;
}

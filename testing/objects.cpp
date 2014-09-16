#include "objects.h"
#include "shapes.h" // necessary to do separately, for Player class.


BaseObject::BaseObject()
{
//    std::cout << " calling base object constructor " << id << std::endl;
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
//std::cerr << " base object add_physics " << std::endl;
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


Player::Player( btVector3 pos, Uint16 color_, SDL_Surface* texture_,
                btScalar mass_ )
{
    object = new Cube( pos, color_, texture_, mass_ );
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
    maxwalkspeed2 = 43;
    walkacceleration = 2.5;
    maxrotspeed2 = 12;
    rotacceleration = 10;
    canjump = false;
    onground = false;
    jumpimpulse = 15;
}

void
Player::update( float dt )
{
    object->update( dt );

    canjump = feet_on_something();
    onground = on_ground();
}

void
Player::walk( float dt, int dir )
{
    if ( canjump && onground)
    if (object->dynamics == 3)
    {
        // get the cube-x direction in world coordinates
        btVector3 forward = object->body->getWorldTransform().getBasis()[0];
        forward.normalize();
        forward.setY( -forward.y() ); // for some reason the world transform has an inverted y...
//        std::cout << " forward = " << forward.x() << ", "
//                                   << forward.y() << ", "
//                                   << forward.z() << "\n";

        float veldiff2 = maxwalkspeed2- (object->lastvelocity.length2());
//        std::cerr << " last velocity = " << (object->lastvelocity.length2()) << std::endl;
//        std::cerr << " max walk speed = " << (maxwalkspeed2) << std::endl;
        if ( veldiff2 > 0 )
        {
            object->body->forceActivationState(1);
            object->body->applyCentralImpulse( walkacceleration*veldiff2*forward*dt*dir );
        }
//        else
//            std::cerr << "moving too fast, can't walk properly" << std::endl;
    }
}

void
Player::turn( float dt, int dir )
{
//    if (canjump && onground)
    {
        // get the cube-z direction in world coordinates
        btVector3 up = object->body->getWorldTransform().getBasis()[2];
        up.normalize();

//        btVector3 up = body->getInvInertiaTensorWorld().inverse()*
//                                ( body->getWorldTransform().getBasis()*btVector3(0,0,1) );

        //body->applyTorque( rotspeed*up*dir ); 
        float omegadiff2 = maxrotspeed2- (object->lastomega.length2());
        if ( omegadiff2 > 0 )
        {
            object->body->forceActivationState(1);
            object->body->applyTorqueImpulse( rotacceleration*(omegadiff2)*up*dt*dir ); 
        }
        // else
        // std::cerr << " player is spinning out of control " << std::endl;
    }
}

void
Player::quick_turn()
{
//        btVector3 up = object->body->getWorldTransform().getBasis()[2];
//        Physics* standin = object->physics;
//        object->remove_physics();
//        object->rotateZ( rotspeed*dt*dir ); 
//        object->fix_transform(); // need to fix transform since there was no dynamics
//        object->add_physics(*standin);
}


bool 
Player::feet_on_something() // ray cast down in cube-coords
{
    btVector3 rayfrom( object->lastposition );
    // get the cube-z direction in world coordinates
    //btVector3 cubeup = body->getWorldTransform().getBasis()[2];
    //cubeup.normalize(); // this shouldn't be necessary
    //btVector3 rayto = rayfrom - cubeup;

    btTransform cubeform;
    btVector3 down = -(object->body->getWorldTransform().getBasis()[2]);
    down.normalize();
    down *= object->size.z()*1.1;
    //btVector3 forward = cubeform.getBasis()[0];

    btVector3 rayto = rayfrom + down;


    ClosestNotMe ray(object->body);

    object->physics->dworld->rayTest( rayfrom, rayto, ray ); 

    if ( ray.hasHit() )
        return true;
    else
        return false;
}

bool 
Player::on_ground() // ray cast downwards in world-coords
{
    btVector3 rayfrom( object->lastposition );
    btVector3 rayto = rayfrom - 1.2*btVector3(0,0,object->size.z());
    
    ClosestNotMe ray(object->body);

    object->physics->dworld->rayTest( rayfrom, rayto, ray ); 

    if ( ray.hasHit() )
        return true;
    else
        return false;
}

void
Player::jump()
{
//
    if ( canjump && (object->dynamics == 3) )
    {
        canjump = false;
        object->body->forceActivationState(1);

        // get the cube-z direction in world coordinates
        btVector3 cubeup = object->body->getWorldTransform().getBasis()[2];
        // add worldup to cubeup to get a jump vector
        btVector3 jumpvec = (btVector3(0,0,1) /* it'd be nice to multiply by friction of object at feet here */+ cubeup);
        
        object->body->applyCentralImpulse( jumpvec*jumpimpulse );
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
    return *this;
}

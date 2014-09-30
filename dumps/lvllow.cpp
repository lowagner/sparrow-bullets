/*
Check the LICENSE file included for copyright information.  
*/
#include "play.h"
#include <string>
#include <iostream>

int
LowLevels::reset()
{
    totalclock += clock;

    deinit(); // kill everything first
//    previous_t = time(0);
//    current_t = time(0);

    killboxfromblockid = false;


    // then rebirth it all...
    winlevel = 0.f; // you haven't won yet!
    pause = 0; // default to no pause
    menu = 0;
    clock = 0;
    checkertexture = spLoadSurface("../data/check.png");

    if ( level == 1 )
    {
        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(10,10,1), btVector3(0,0,-5), 0x05FF ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(7,7,-3), 0x055F ) ); // half-sizes, pos, color 
        boxes.push_back( Box( btVector3(6,1,2), btVector3(3,-1,-2), 0xF50F ) ); // half-sizes, pos, color

        ramps.push_back( Ramp( btVector3(10,2,4), btVector3(-8,0,-4), 0xF0FF ) ); // sizes, pos, color
        ramps.push_back( Ramp( btVector3(10,2.5,1), btVector3(0,-10,-4), 0xFFFF ) ); // sizes, pos, color
        ramps[0].rotateZ( M_PI/2 );
        ramps[1].rotateZ( 2*M_PI );

        hero = Player( btVector3(0,8,5), 0xF00F, checkertexture );
        hero.object->debug = true;
       
        // add some blocks pieces
        for ( int i=0; i<5; i++ )
        {
            blocks.push_back(  Cube( btVector3(2*i-3,0,10+2*i), 0x0F0F )  );
        }
        blocks[4].impulse( btVector3(0,10,0) );
        blocks[3].impulse( btVector3(10,15,0) );
        blocks[2].impulse( btVector3(-10,-10,0) );
        blocks[1].impulse( btVector3(-5,-10,0) );
    }
    else if ( level == 2 )
    {
        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(7,7,1), btVector3(0,0,-5), 0x05FF ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(7,7,-3), 0x055F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(-7,7,-3), 0x055F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(7,-7,-3), 0x055F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(-7,-7,-3), 0x055F ) ); // half-sizes, pos, color

        ramps.push_back( Ramp( btVector3(4,2,2.8), btVector3(-4,-5.1,2.8), 0xFFFF ) ); // half-sizes, pos, color
        ramps.push_back( Ramp( btVector3(4,2,2.8), btVector3(-7.1,-4,2.8), 0xFFFF ) ); // half-sizes, pos, color
        ramps[0].rotate( btVector3(1,0,0), M_PI );
        ramps[1].rotate( btVector3(1,0,0), M_PI );
        ramps[1].rotateZ( -M_PI/ 2);

        hero = Player( btVector3(0,4,5), 0xF00F, checkertexture );
        hero.object->rotateZ( M_PI/2 );
        hero.object->debug = true;
       
        // add some blocks pieces
        for ( int i=0; i<6; i++ )
        {
            blocks.push_back(  Cube( btVector3(2*i-4,2*i-4,10+2*i), 0xFF0F )  );
        }
        blocks[4].impulse( btVector3(0,10,0) );
    }
    else if ( level == 3 )
    {
        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(7,7,1), btVector3(0,0,-5), 0xF00F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(7,5,-3), 0xF50F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(3,7,-1), 0x0F0F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(1,3,1), 0x03FF ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(7,-1,3), 0x00FF ) ); // half-sizes, pos, color

        hero = Player( btVector3(0,4,5), 0xF00F, checkertexture );
        hero.object->rotateZ( -M_PI/2 );
        hero.object->debug = true;
       
        // add some blocks pieces
        for ( int i=0; i<6; i++ )
        {
            blocks.push_back(  Cube( btVector3(2*i-4,2*i-6,10+2*i), 0xFFFF )  );
        }
        //blocks[4].impulse( btVector3(0,10,0) );
    }
    else if ( level == 4 )
    {
        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(6,6,1), btVector3(-14,0,-5), 0x05FF ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(-4,10,-5), 0x055F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(4,2,-5), 0x055F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(3,3,1), btVector3(13,5,-4), 0x055F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(2,2,1), btVector3(6,-5,-5), 0x055F ) ); // half-sizes, pos, color

        hero = Player( btVector3(-14,4,5), 0xF00F, checkertexture );
        hero.object->rotateZ( -M_PI/2 );
        hero.object->debug = true;
      
        blocks.push_back( Cube( btVector3(-15,-3,19), 0xF00F ) );
        blocks.push_back( Cube( btVector3(-4,7,3), 0xF00F ) );
        blocks.push_back( Cube( btVector3(13,7,3), 0xF00F ) );
        blocks.push_back( Cube( btVector3(6,-6,3), 0xF00F ) );
        blocks.push_back( Cube( btVector3(-14,2,0), 0xF00F ) );

        // add some blocks pieces
        blocks[4].impulse( btVector3(23,0,40) );
    }
    else if ( level == 5 )
    {
        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(4,5,1), btVector3(2,5,-5), 0x05FF ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(2,2,1), btVector3(-5,0,-5), 0x05FF ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(10,1,1), btVector3(0,-5,-5), 0x05FF ) ); // half-sizes, pos, color

        boxes.push_back( Box( btVector3(4,4,1), btVector3(7,7,-3), 0x055F ) ); // half-sizes, pos, color 
        boxes.push_back( Box( btVector3(6,1,2), btVector3(3,-1,-2), 0xF50F ) ); // half-sizes, pos, color

        ramps.push_back( Ramp( btVector3(10,2,4), btVector3(-8,0,-4), 0xF0FF ) ); // sizes, pos, color
        ramps.push_back( Ramp( btVector3(10,2.5,1), btVector3(0,-10,-4), 0xFFFF ) ); // sizes, pos, color
        ramps[0].rotateZ( M_PI/2 );
        ramps[1].rotateZ( 2*M_PI );

        hero = Player( btVector3(0,8,5), 0xF00F, checkertexture );
        hero.object->debug = true;
       
        // add some blocks pieces
        for ( int i=0; i<5; i++ )
        {
            blocks.push_back(  Cube( btVector3(2*i-3.5,-1,10+2*i), 0x0F0F )  );
        }
        blocks[4].impulse( btVector3(0,10,0) );
        blocks[3].impulse( btVector3(10,15,0) );
        blocks[2].impulse( btVector3(10,-7,0) );
        blocks[0].impulse( btVector3(-10,10,0) );
    }
    else if ( level == 6 )
    {
        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(1,5,1), btVector3(5,2,4), 0x1100 ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(7,1,1), btVector3(10,-2,0), 0x1100 ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(5,1,1), btVector3(5,2,-3), 0x1100 ) ); // half-sizes, pos, color
        
        boxes.push_back( Box( btVector3(5,0.15,1), btVector3(-5,0.9,-3.5), 0xA030 ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(5,0.15,1), btVector3(-5,-0.9,-3.5), 0xA030 ) ); // half-sizes, pos, color

        hero = Player( btVector3(5,6,7), 0xF00F, checkertexture );
        hero.object->rotateZ( -M_PI/2 );
        hero.object->debug = true;
      
        blocks.push_back( Cube( btVector3(-9,0,5), 0xF000 ) );
        blocks[0].rotate( btVector3(1,0,0), M_PI/4 );
        
        blocks.push_back( Cube( btVector3(15,-2,2), 0xF000 ) );
    }
    else if ( level == 7 )
    {
        // fun level, where boxes will die if cubes do
        killboxfromblockid = true;

        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(4,4,1), btVector3(4,4,-3), 0x500F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(-4,4,-3), 0x505F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(4,-4,-3), 0x055F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(4,4,1), btVector3(-4,-4,-3), 0x0F5F ) ); // half-sizes, pos, color

        blocks.push_back(  Cube( btVector3(4,4,1), 0x500F )  );
        blocks.push_back(  Cube( btVector3(-4,4,1), 0x505F )  );
        blocks.push_back(  Cube( btVector3(4,-4,1), 0x055F )  );
        blocks.push_back(  Cube( btVector3(-4,-4,1), 0x0F5F )  );
        
        boxes.push_back( Box( btVector3(1,4,1), btVector3(9,4,-1), 0xFFFF ) ); // half-sizes, pos, color


        hero = Player( btVector3(4,4,5), 0xF00F, checkertexture );
        hero.object->rotateZ( M_PI/2 );
        hero.object->debug = true;
       
        // add some blocks pieces
        for ( int i=0; i<4; i++ )
        {
            blocks[i].id = i;
            boxes[i].id = i;
        }
        boxes[4].id = 1234;
    }
    else if ( level == 8 )
    {
        // fun level, where boxes will die if cubes do
        killboxfromblockid = true;

        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(1,1,1), btVector3(-6,0,7), 0x109F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(1,1,1), btVector3(-3,0,5), 0x307F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(1,1,1), btVector3(0,0,3), 0x505F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(1,1,1), btVector3(3,0,1), 0x703F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(1,1,1), btVector3(6,0,-1), 0x901F ) ); // half-sizes, pos, color
        
        boxes.push_back( Box( btVector3(10,5,5), btVector3(0,-7,-7), 0x000F ) ); // half-sizes, pos, color
        boxes.push_back( Box( btVector3(10,5,5), btVector3(0,7,5), 0x000F ) ); // half-sizes, pos, color

        blocks.push_back(  Cube( btVector3(4,4,12), 0xFFFF )  );
        blocks.push_back(  Cube( btVector3(-4,4,12), 0xFFFF )  );
        blocks.push_back(  Cube( btVector3(4,-4,12), 0xFFFF )  );
        blocks.push_back(  Cube( btVector3(-4,-4,12), 0xFFFF )  );

        hero = Player( btVector3(-9,-6,12), 0xF00F, checkertexture );
        //hero.object->rotateZ( M_PI/2 );
        hero.object->debug = true;
       
        // add some blocks pieces
        for ( int i=0; i<4; i++ )
        {
            blocks[i].id = 0;
            boxes[i].id = 0;
        }
        boxes[4].id = 0;
        for ( int i=5; i<7; i++ )
        {
            boxes[i].id = 1234;
        }
    }
    else
    {
        std::cout << " congratulations, you beated all levels! " << std::endl;
        std::cout << " total accumulated time: " << totalclock << std::endl;
        std::cout << " remaining lives: " << lives << std::endl;
        std::cout << " hopefully they will get around to creating level " << level << std::endl;
        return GAMESTATEquit;
    }
    
    // now add physics to everybody 
    physics.init();
    hero.object->add_physics( physics );
    
    for ( int i=0; i<blocks.size(); i++ )
        blocks[i].add_physics( physics );
    
    for ( int i=0; i<boxes.size(); i++ )
        boxes[i].add_physics( physics );

    for ( int i=0; i<ramps.size(); i++ )
        ramps[i].add_physics( physics );
    
    spDrawInExtraThread(0);
    //spDrawInExtraThread(1);
    return GAMESTATEplay;
}


/*
Check the LICENSE file included for copyright information.  
*/
#include "play.h"
#include <string>
#include <iostream>

MainMenu::MainMenu( int level_ ) 
{
    gamestate = GAMESTATEmenu;

    level = level_;
    levelset = 1; // levelset = 0 is the main menu.
                  // levelset = 1 is the low levels
    reset();
}


int
MainMenu::reset()
{
    std::cout << " levelset = " << levelset << std::endl;
    std::cout << " level = " << level << std::endl;
    switch ( levelset )
    {
        case 1:
            sprintf( lvltext, "main menu" );
            break;
        case 2:
            sprintf( lvltext, "low levels" );
            break;

    }
    std::cout << " leveltext = " << lvltext << std::endl;

    if ( lives == 999998 )
    {
        // if you lose a life on the Main menu, that is tantamount to quitting
        std::cout << " You fell off the map and ended the game. " << std::endl;
        return GAMESTATEquit;
    }

    totalclock = 0;
    clock = 1000; // hack to make the clock not appear
    lives = 999999; // hack to make lives not appear

    deinit(); // kill everything first
//    previous_t = time(0);
//    current_t = time(0);

    killboxfromblockid = false;


    // then rebirth it all...
    winlevel = 0.f; // you haven't won yet!
    pause = 0; // default to no pause
    menu = 0;
    checkertexture = spLoadSurface("../data/check.png");

    if ( level == 1 )
    {
        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(7,7,1), btVector3(0,0,-3), 0x05FF ) ); // half-sizes, pos, color

        blocks.push_back( Cube( btVector3(0,0,5), 0x05FF ) ); // half-sizes, pos, color
        blocks[0].id = 2;

        hero = Player( btVector3(0,4,5), 0xF00F, checkertexture );
        hero.object->rotateZ( M_PI/2 );
        hero.object->debug = true;
    }
    else if ( level == 2 )
    {
        // low levels
        std::cout << " switching to low levels" << std::endl;

        return GAMESTATElovels;
    }
    else
    {
        std::cout << "There are no levels past this point.  But please contribute!" << std::endl; 
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
    return gamestate;
}



int MainMenu::update_level( btScalar fdt )
{ 
    if ( winlevel == 0.f )
    {
        // if we have not won the level yet...
        // update level time
        clock += fdt;

        // update blocks
        int i=0;
        int blocksize = blocks.size();
        bool activate = false;
        while ( i < blocksize )
        {
            //std::cout << i << "; id " << blocks[i].id << " ";
            blocks[i].update( fdt );

            if (  blocks[i].out_of_bounds( outofbounds )   )
            {
                level = blocks[i].id;  // eventually this will be levelset = blocks[i].id
                return 1;
            }
            else
            {
                i++;
            }
        }
//        if (blocksize == 0)
//        {
//            winlevel = 3;
//        }
//        else if ( activate )
//        {
//            // boxes disappeared or something, we should reactivate everything
//            for ( int i=0; i<blocks.size(); i++ )
//            {
//                blocks[i].activate();
//            }
//            hero.object->activate();
//        }
    } 
    else
    { // we have won and are waiting to start next level
        winlevel -= fdt;
        if ( winlevel <= 0.f )
        {
            level += 1;
            return 1;
        }

    }
    return 0;
}



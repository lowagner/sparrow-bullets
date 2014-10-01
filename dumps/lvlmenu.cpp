/*
Check the LICENSE file included for copyright information.  
*/
#include "play.h"
#include <string>
#include <iostream>

MainMenu::MainMenu( int level_, int levelset_ ) 
{
    gamestate = GAMESTATEmenu;

    level = level_;
    levelset = levelset_; // levelset = 1 is the main menu.
                          // levelset = 2 is the low levels, etc.
    reset();
}


int
MainMenu::reset()
{
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
    numbertexture.push_back( spLoadSurface("../data/number_M.png") );

    for ( int i=1; i<2; i++)
    {
        char buffer[60];
        sprintf( buffer, "../data/number_%i.png", i );
        numbertexture.push_back( spLoadSurface(buffer) );
    }

    if ( levelset == GAMESTATEmenu )
    {
        if ( level == 1 )
        {
            sprintf( lvltext, "main menu" );
            set_alert( "- choose your stage -" );

            outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

            // this guy includes the floor.  all static rectangular prisms.
            boxes.push_back( Box( btVector3(7,7,1), btVector3(0,0,-3), 0x05FF ) ); // half-sizes, pos, color

            blocks.push_back( Cube( btVector3(0,0,5), 0x05FF, 2 ) ); // half-sizes, pos, color

            hero = Player( btVector3(0,4,5), 0xF00F, checkertexture );
            hero.object->rotateZ( 3*M_PI/2 );
            hero.object->debug = true;
        }
        else 
        {
            levelset = level;
            level = 0;
        }
    }


    if ( levelset == GAMESTATEmenu )
    {
    // run through once more just in case we switched...
    }
    else if ( levelset == GAMESTATElovels )
    {
        if ( level == 0 )
        { // choose your level of the low levels
            sprintf( lvltext, "low levels" );
            set_alert( "- choose level for low -" );

            outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

            // this guy includes the floor.  all static rectangular prisms.
            boxes.push_back( Box( btVector3(7,7,1), btVector3(0,0,-3), 0x05FF ) ); // half-sizes, pos, color

            blocks.push_back( Cube( btVector3(5,-3,5), 0x05FF, 1, numbertexture[1] ) ); // half-sizes, pos, color
            blocks.push_back( Cube( btVector3(3,-3,5), 0x05FF, 2 ) ); // half-sizes, pos, color
            blocks.push_back( Cube( btVector3(1,-3,5), 0x05FF, 3 ) ); // half-sizes, pos, color
            blocks.push_back( Cube( btVector3(-1,-3,5), 0x05FF, 4 ) ); // half-sizes, pos, color
            blocks.push_back( Cube( btVector3(-3,-3,5), 0x05FF, 5 ) ); // half-sizes, pos, color
            blocks.push_back( Cube( btVector3(-5,-3,5), 0x05FF, 6 ) ); // half-sizes, pos, color
            blocks.push_back( Cube( btVector3(-5,-1,5), 0x05FF, 7 ) ); // half-sizes, pos, color
            blocks.push_back( Cube( btVector3(-5,1,5), 0x05FF, 8 ) ); // half-sizes, pos, color
            blocks.push_back( Cube( btVector3(-5,3,5), 0x05FF, 9 ) ); // half-sizes, pos, color

            hero = Player( btVector3(0,4,5), 0xF00F, checkertexture );
            hero.object->rotateZ( 3*M_PI/2 );
            hero.object->debug = true;
        }
        else
        {
            return GAMESTATElovels;
        }
    }
    else
    {
        std::cout << "There are no level sets past this point.  But please contribute!" << std::endl; 
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
        if ( alerttime > 0.f )
        {
            alerttime -= fdt;
            if ( alerttime <= 0.f )
            {
                alerttime = 0.f;
            }
        }
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



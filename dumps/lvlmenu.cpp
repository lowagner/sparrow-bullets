/*
Check the LICENSE file included for copyright information.  
*/
#include "play.h"
#include <string>
#include <iostream>
#include "util.h"

MainMenu::MainMenu( int level_, int levelset_ ) 
{
    won = false;

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
        if ( alive )
        { // reset the level
        }
        else if ( levelset == 1 )
        {
            // if you lose a life on the Main menu, that is tantamount to quitting
            std::cout << " You fell off the map and ended the game. " << std::endl;
            return GAMESTATEquit;
        }
        else
        {
            std::cout << " Going back one level in main menu. " << std::endl;
            levelset = 1;
            level = 1;
        }
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

    for ( int i=1; i<13; i++)
    {
        char buffer[60];
        sprintf( buffer, "../data/number_%i.png", i );
        numbertexture.push_back( spLoadSurface(buffer) );
    }

    if ( levelset == GAMESTATEmenu )
    { 
        won = false;
        if ( level == 1 )
        {
            sprintf( lvltext, "main menu" );
            set_alert( "- choose your stage -" );

            outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

            // this guy includes the floor.  all static rectangular prisms.
            boxes.push_back( Box( btVector3(5,5,1), btVector3(0,0,-3), 0x05FF ) ); // half-sizes, pos, color
            boxes.push_back( Box( btVector3(1,1,1), btVector3(0,-7,1), 0x05FF ) ); // half-sizes, pos, color
            boxes[1].add_text( "push a lvl cube off" );
            boxes[1].add_text( "to play that level" );

            blocks.push_back( Cube( btVector3(0,0,5), 0x05FF, 2 ) ); // half-sizes, pos, color
            blocks[0].add_text( "low" );
            blocks[0].add_text( "lvls" );

            hero = Player( btVector3(0,4,5), 0xF00F, checkertexture );
            hero.object->rotateZ( 3*M_PI/2 );
            hero.object->debug = true;
            hero.object->add_text("hero");
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
        if ( won && level == 0 )
        {
            // play marathon play
            std::cout << " playing marathon of level set " << levelset << "\n";
            level = 1;
            return GAMESTATElovels;
        }
        else if ( level == 0 )
        { // choose your level of the low levels
            sprintf( lvltext, "low levels" );
            set_alert( "- choose low level -" );

            outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

            // this guy includes the floor.  all static rectangular prisms.
            boxes.push_back( Box( btVector3(4,4,1), btVector3(7,0,-3), 0x35B7 ) ); 
            boxes.push_back( Box( btVector3(3,3,1), btVector3(0,-5,-4), 0x1131 ) ); 
            boxes.push_back( Box( btVector3(5,5,1), btVector3(-6,4,-1), 0x0007 ) ); 
            boxes.push_back( Box( btVector3(2,2,1), btVector3(2,4,3), 0x0000 ) ); 

            blocks.push_back( Cube( btVector3(7,0,5), 0x05FF, 0, numbertexture[0] ) ); 
            blocks[0].add_text("MARATHON");

            blocks.push_back( Cube( btVector3(2,-7,5), 0x35FF, 1, numbertexture[1] ) );
            blocks.push_back( Cube( btVector3(0,-7,5), 0x343A, 2, numbertexture[2] ) );
            blocks.push_back( Cube( btVector3(-2,-7,5), 0x350F, 3, numbertexture[3] ) ); 
            blocks.push_back( Cube( btVector3(-2,-3,5), 0xF10F, 4, numbertexture[4] ) ); 

            blocks.push_back( Cube( btVector3(-8,0,5), 0x35FF, 5, numbertexture[5] ) ); 
            blocks.push_back( Cube( btVector3(-10,0,5), 0x3300, 6, numbertexture[6] ) ); 
            blocks.push_back( Cube( btVector3(-10,2,5), 0x700F, 7, numbertexture[7] ) ); 
            blocks.push_back( Cube( btVector3(-8,8,5), 0xFFFF, 8, numbertexture[8] ) );
            blocks.push_back( Cube( btVector3(-4,8,5), 0x05FF, 9, numbertexture[9] ) );
            blocks.push_back( Cube( btVector3(-2,8,5), 0x05FF, 10, numbertexture[10] ) );
            blocks.push_back( Cube( btVector3(-2,6,5), 0x05FF, 11, numbertexture[11] ) );

            blocks.push_back( Cube( btVector3(3,4,8), 0x05FF, 12, numbertexture[12] ) );
          
            int i = 1;
            std::cout << "LEVELSET " << levelset << " TIMES:\n";
            while ( i <= 12 )
            {
                char buffer[64];
                sprintf( buffer, "%i-%i.dat", levelset, i ); 
                float oldbest = loadtime( buffer );
                if ( oldbest < 999.0f )
                {
                    sprintf( buffer, "%.3f", oldbest );
                    std::cout << " level " << i << ": " << oldbest << "\n";
                    blocks[i].add_text( buffer );
                    i += 1;
                }
                else
                {
                    break;
                }
            }
            if ( i <= 12 )
            {
                // delete any blocks which we haven't beaten yet
                int times = 13 - i;
                while ( times > 0 )
                {
                    // probably a more efficient way than to do this, but i don't have internet right now...
                    blocks.erase(blocks.begin() + i); 
                    times-=1;
                }

                boxes.erase( boxes.begin() + 3 );
                if ( i <= 5 )
                    boxes.erase( boxes.begin() + 2 );
                if ( i <= 1 )
                    boxes.erase( boxes.begin() + 1 );
            }
   
            hero = Player( btVector3(10,0,5), 0xF00F, checkertexture );
            hero.object->rotateZ( M_PI );
            hero.object->debug = true;
            
            hero.object->add_text("hero");

        }
        else
        {   
            // single level play
            std::cout << " playing single level " << level << " in level set " << levelset << "\n";
            levelset = 0; 
            return GAMESTATElovels;
        }
    }
    else
    {
        std::cout << "There are no level sets past this point.  Please contribute some!" << std::endl; 
        return GAMESTATEquit;
    }

    won = false; 
    alive = true;

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
                won = true;
                level = blocks[i].id;  // this becomes levelset = blocks[i].id in reset()
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



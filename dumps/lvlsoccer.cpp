/*
Check the LICENSE file included for copyright information.  
*/
#include "play.h"
#include <string>
#include <iostream>

#include <ctime>
#include <cstdlib>


Soccer::Soccer( int levelset_, int level_, char* message_ )
{
    cameramovecooldown = 0.0f;
    sprintf( message, "%s", message_ );
    totalclock = 0;

    level = level_;
    levelset = levelset_;

    gamestate = GAMESTATEsoccer;

    won = false;
    reset();
    srand(time(0));
}



int
Soccer::reset()
{   
    points = 0;
    if ( message != "" )
    {
        set_alert( message );
        sprintf( message, "" );
    }
    lives = 9999;

    sprintf( lvltext, "soccer stage %i", level );

    deinit(); // kill everything first
//    previous_t = time(0);
//    current_t = time(0);

    killboxfromblockid = false;
    addboxfromblockid = false;

    // then rebirth it all...
    winlevel = 0.f; // you haven't won yet!
    pause = 0; // default to no pause
    menu = 0;
    clock = 60;
    checkertexture = spLoadSurface("../data/check.png");
    physics.init();

    if ( level == 1 )
    {
        outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

        halfdistancetogoal = 10;
        halfgoalwidth = 4;

        // this guy includes the floor.  all static rectangular prisms.
        boxes.push_back( Box( btVector3(halfdistancetogoal,8,1), btVector3(0,0,-5), 0x0F00 ) ); 
        boxes.push_back( Box( btVector3(1,halfgoalwidth+1,3), 
                              btVector3(halfdistancetogoal+4,0,-3), 0xAFFF ) ); 
        boxes.push_back( Box( btVector3(1.5,1,3), btVector3(halfdistancetogoal+1.5,-halfgoalwidth,-3), 0xAFFF ) ); 
        boxes.push_back( Box( btVector3(1.5,1,3), btVector3(halfdistancetogoal+1.5,halfgoalwidth,-3), 0xAFFF ) );  
        boxes.push_back( Box( btVector3(2.5,5,1), btVector3(halfdistancetogoal+2.5,0,1), 0xAFFF ) );   
        boxes.push_back( Box( btVector3(1.5,1,3), btVector3(-halfdistancetogoal-1.5,-halfgoalwidth,-3), 0xFFFA ) ); 
        boxes.push_back( Box( btVector3(1.5,1,3), btVector3(-halfdistancetogoal-1.5,halfgoalwidth,-3), 0xFFFA ) );  
        boxes.push_back( Box( btVector3(2.5,5,1), btVector3(-halfdistancetogoal-2.5,0,1), 0xFFFA ) );   
        boxes.push_back( Box( btVector3(1,5,3), btVector3(-halfdistancetogoal-4,0,-3), 0xFFFA ) ); 

        hero = Player( btVector3(-8,0,5), 0xF000 );
        hero.object->debug = true;
      
        blocks.push_back( Cube( btVector3(0,0,0), 0xFFFF, 0, checkertexture ) ); 
        
        standard_physics_init();
    }
    else
    {
        std::cout << " unknown soccer stage " << level << "\n";
        return GAMESTATEquit;
    }
    
    won = false;
    alive = true;
    // now add physics to everybody 
    hero.object->add_physics( physics ); 
    
    spDrawInExtraThread(0);
    //spDrawInExtraThread(1);
    return gamestate;
}


int Soccer::update_level( btScalar fdt )
{
    if ( not alive )
    {
        alive = true;
        hero.object->remove_physics();
        hero.object->set_position( btVector3(-5,0,5) );
        hero.object->set_velocity( btVector3() );
        hero.object->set_omega( btVector3() );
        hero.object->add_physics(physics);
    }

    // update boxes
    for ( int i=0; i<boxes.size(); i++ )
        boxes[i].update( fdt );
    // update ramps
    for ( int i=0; i<ramps.size(); i++ )
        ramps[i].update( fdt );

    // update blocks (soccer balls)
    int i=0;
    int blocksize = blocks.size();
    bool activate = false;
    while ( i < blocksize )
    {
        //std::cout << i << "; id " << blocks[i].id << " ";
        blocks[i].update( fdt );

        if (  blocks[i].out_of_bounds( outofbounds )   )
        {
            spSoundPlay( gotsound, -1,0,0,0 ); // play getting the box sound

            if ( winlevel == 0.f )
            {   // only can score without the finally count down
                btVector3 pos = blocks[i].get_position();
                if ( pos.getX() > halfdistancetogoal-1 )
                {
                    if ( pos.getX() < halfdistancetogoal+2.1 )
                    if ( fabs(pos.getY()) < halfgoalwidth )
                    {
                        points += 1;
                        std::cout << " player scores!  game " << points << " / " << antipoints << "\n";
                    }
                }
                else if ( pos.getX() < -halfdistancetogoal+1 )
                {
                    if ( pos.getX() > -halfdistancetogoal-2.1 )
                    if ( fabs(pos.getY()) < halfgoalwidth )
                    {
                        antipoints += 1;
                        std::cout << " opponent scores!  game " << points << " / " << antipoints << "\n";
                    }
                }
            }

            //std::cout << "\n WARNING!  block " << i << " out of bounds!\n";
            blocks[i].remove_physics();  // remove this guy from physics world
            if ( killboxfromblockid || addboxfromblockid )
            { // here we kill any boxes which have the same id as block j

                int blockid = blocks[i].id;
                int j=0;
                int boxsize = boxes.size();
                //std::cout << "\n deleting  block " << blockid << "; ";
                while ( j < boxsize )
                {
                    //std::cout << "checking  box " << boxes[j].id << "; "; 
                    if ( boxes[j].id == blockid )
                    {
                        //std::cout << "deleting  box " << boxes[j].id << "; "; 
                        if ( boxes[j].physics )
                        {
                            if ( killboxfromblockid )
                            {
                                boxes[j].remove_physics();  // remove this guy from physics world
                                if ( addboxfromblockid )
                                    boxes[j].set_alpha( 128 );
                                else
                                    boxes[j].set_alpha( 0 );
                            }
                        }
                        else
                        {
                            if ( addboxfromblockid )
                                boxes[j].add_physics( physics );  // add this guy from physics world
                        }
                        activate = true;
                    }
                    j++;
                }
            }
            // finally, we would destroy the block.
            // but now we re-add it.
            blocks[i].set_velocity( btVector3() );
            blocks[i].set_omega( btVector3() );
            blocks[i].set_position( btVector3(0,0,5) );
            blocks[i].add_physics( physics ) ;
        }
        i++;
    }
    
    if ( winlevel == 0.f )
    {
        // if we have not won the level yet...
        // update level time.  in soccer we remove time
        clock -= fdt;
        // check win conditions...
        if (clock <= 0.f)
        {
            clock = 0.;
            winlevel = 3; // amount of time you must survive before transitioning to next level
        }
        if ( activate )
        {
            // boxes disappeared or something, we should reactivate everything
            for ( int i=0; i<blocks.size(); i++ )
            {
                blocks[i].activate();
            }
            hero.object->activate();
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
            won = ( points > antipoints );
            return 1;
        }
    }
    return 0; // no reset if update_level doesn't do nothing
}


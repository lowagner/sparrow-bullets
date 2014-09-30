/*
Check the LICENSE file included for copyright information.  
*/
#include "play.h"
#include <string>
#include <iostream>

int
MainMenu::reset()
{
    totalclock = 0;
    clock = 1000; // hack to make the clock not appear
    lives = 999100; // hack to make lives not appear

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
        boxes.push_back( Box( btVector3(7,7,1), btVector3(0,0,-5), 0x05FF ) ); // half-sizes, pos, color

        ramps.push_back( Ramp( btVector3(4,2,2.8), btVector3(-4,-5.1,2.8), 0xFFFF ) ); // half-sizes, pos, color
        ramps.push_back( Ramp( btVector3(4,2,2.8), btVector3(-7.1,-4,2.8), 0xFFFF ) ); // half-sizes, pos, color

        hero = Player( btVector3(0,4,5), 0xF00F, checkertexture );
        hero.object->rotateZ( M_PI/2 );
        hero.object->debug = true;
    }
    else if ( level == 2 )
    {
        // low levels

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



int MainMenu::update( Uint32 dt )
{
    if (menu)
    {
        // if we are showing the menu entries
        // make sure to have it paused...
        pause = 1;
        // inputy type things
        if ( spGetInput()->axis[1]==1 )
        {
            menu += 1;
            if ( menu >= menuitems.size() )
                menu = 1;
            spGetInput()->axis[1] = 0;
        }
        else if ( spGetInput()->axis[1]==-1 )
        {
            menu -= 1;
            if ( menu <= 0 )
                menu = menuitems.size() - 1;
            spGetInput()->axis[1] = 0;
        }
        else if ( spGetInput()->axis[0]==1 )
        {
            if ( menuitemvalues[menu].size() > 0 )
            {
                if ( (menuitemvalueindices[menu] += 1) > menuitemvalues[menu].size() )
                    menuitemvalueindices[menu] = 0;
                set_value( menuitems[menu], menuitemvalues[menu][menuitemvalueindices[menu]] );
                write_settings();
            }
            spGetInput()->axis[0] = 0;
        }
        else if ( spGetInput()->axis[0]==-1 )
        {
            if ( menuitemvalues[menu].size() > 0 )
            {
                if ( (menuitemvalueindices[menu] -= 1) < 0  )
                    menuitemvalueindices[menu] = menuitemvalues[menu].size() - 1;
                set_value( menuitems[menu], menuitemvalues[menu][menuitemvalueindices[menu]] );
                write_settings();
            }
            spGetInput()->axis[0] = 0;
        }
        else if ( spGetInput()->button[SP_BUTTON_START] || spGetInput()->button[SP_BUTTON_SELECT] )
        {
            // if we hit start or select
            spGetInput()->button[SP_BUTTON_START] = 0;
            spGetInput()->button[SP_BUTTON_SELECT] = 0;
            // make sure the buttons are off.

            switch (menu) 
            {
                case 1:  // Return to play
                    menu=0;
                    pause=0;
                    return GAMESTATEplay;
                    break;
                case 2: // Next level
                    menu=0; // unmenu
                    pause=0; // unpause
                    lives -= 10; // penalize the player
                    clock=999; // penalize the player
                    level += 1; // increase level
                    return reset();
                    break;
                case 3: // camerafollowspeed
                case 4: // cameraalignspeed
                    menu=0;
                    pause=0;
                    return GAMESTATEplay;
                    break;
                case 5: // Exit
                    return GAMESTATEquit;
                    break; 
            }
        }

        // DON'T allow any other things to go on when you are navigating
        // the menu.  so prematurely disrupt update function.
        return GAMESTATEplay;
    }

    // otherwise, if we hit start we want to get to menu
    if ( spGetInput()->button[SP_BUTTON_START] )
    {
        menu = 1;
        // unset the start button so it won't grab the first menu item.
        spGetInput()->button[SP_BUTTON_START] = 0;
        return GAMESTATEplay;
    }

    if ( spGetInput()->button[SP_BUTTON_SELECT] )
    {
        spGetInput()->button[SP_BUTTON_SELECT] = 0;
        pause = 1-pause;
    }
    if ( spGetInput()->button[SP_BUTTON_A] )
    {
        // key A on a standard QWERTY keyboard
        cameraaxis += std::max(int(80*dt),1);
        cameramovecooldown = 2;
    }
    if ( spGetInput()->button[SP_BUTTON_B] )
    {
        // key D on a standard QWERTY keyboard
        cameraaxis -= std::max(int(80*dt),1);
        cameramovecooldown = 2;
    }
    if ( spGetInput()->button[SP_BUTTON_X] )
    {
        // key S on a QWERTY
        //spGetInput()->button[SP_BUTTON_x] = 0; // can only allow input once if desired
        cameradistance -= 1000*dt;
        cameraincline += 50*dt;
    }
    else if ( spGetInput()->button[SP_BUTTON_Y] )
    {
        // key W on a QWERTY
        //spGetInput()->button[SP_BUTTON_Y] = 0;
        cameradistance += 1000*dt;
        cameraincline -= 50*dt;
    }

    if ( spGetInput()->axis[1] == 1 )
    {
        //rotation += 100*dt;
    }
    else if ( spGetInput()->axis[1] == -1 )
    {
        //rotation -= 100*dt;
    }

    if ( spGetInput()->button[SP_BUTTON_R] )
    {
        // randomize
        //spGetInput()->button[SP_BUTTON_R] = 0;
    }
    if ( spGetInput()->button[SP_BUTTON_L] )
    {
        spGetInput()->button[SP_BUTTON_L] = 0;
        lives -= 1;
        clock = 0;
        reset();
    }

    if (!(pause))
    {
        btScalar fdt = dt*1.0/1000;
        
//        current_t = time(0);
//        double time = difftime(current_t, previous_t);
//        sleep(3);
//        std::cerr << " sparrow dt = " << fdt << ", c++ sec = " << time << ";  ";
//        previous_t = current_t;
        

        physics.update( fdt );
        // update hero stuff
        hero.update( fdt );
        if ( hero.object->out_of_bounds( outofbounds ) )
        {
            lives -= 1;
            clock = 0;
            reset();
        }

        if ( camerafollowspeed > 0.f )
            cameracenter = ( cameracenter + camerafollowspeed * fdt * hero.get_position() ) / ( 1 + camerafollowspeed * fdt );
        if ( cameraalignspeed > 0.f )
        { 
            if ( cameramovecooldown == 0.f )
            {
                if ( hero.on_ground() )
                {   
                    btVector3 cameraforward = hero.get_forward();
                    if ( fabs( cameraforward.z() ) > 0.91f )
                    {   // if we are looking up or down...
                        if ( cameraforward.z() < 0 )
                            cameraforward = hero.get_up();
                        else
                            cameraforward = -hero.get_up();
                    }
                    else if ( hero.topside_up() == -1 )
                    {   // if we're on our head...
                        cameraforward *= -1;
                    }
                    Sint32 theta = spFloatToFixed( atan2( -cameraforward.x(), -cameraforward.y() ) + M_PI ) ;
//                    std::cerr << " theta / (2 Pi ) = " << ( ( theta )*1.0/(2*SP_PI) ) << std::endl;

                    if ( cameraaxis > 2*SP_PI )
                        cameraaxis -= 2*SP_PI;
                    else if (cameraaxis < 0 )
                        cameraaxis += 2*SP_PI;
                    //std::cerr << " camera / (2 Pi ) = " << ( ( cameraaxis )*1.0/(2*SP_PI) ) << std::endl;

                    if ( abs( cameraaxis - theta ) > SP_PI )
                    {
                        // check if the distance between the two is very large...
                        // because we want to go the quickest way around...
                        if ( abs( cameraaxis - ( theta + 2*SP_PI ) ) < abs( cameraaxis - (theta - 2*SP_PI ) ) )
                        {
                            // here, it's best to add 2pi to theta...
                            cameraaxis = ( cameraaxis + fdt * (theta+2*SP_PI) ) / ( 1 + fdt );
                        }
                        else
                        {
                            // here it's better to subtract 2pi from theta
                            cameraaxis = ( cameraaxis + cameraalignspeed * fdt * (theta-2*SP_PI) ) / ( 1 +cameraalignspeed * fdt );
                        }
                    }
                    else
                        cameraaxis = ( cameraaxis + cameraalignspeed * fdt * theta ) / ( 1 + cameraalignspeed * fdt );
                }
                else
                { // currently don't spin camera around, if hero is in the air

                }
            }
            else
            { // camera was moved recently, give a cool down
                cameramovecooldown -= fdt;
                if ( cameramovecooldown < 0.f )
                    cameramovecooldown = 0.f;
            }
        }
        
        if ( spGetInput()->axis[0] )
        {
            spGetInput()->axis[0] = hero.turn( fdt, spGetInput()->axis[0] );
            //std::cout << " turning hero " << (spGetInput()->axis[0]) << std::endl;
        }
        // this function will check to see if the hero can actually walk.
        if ( spGetInput()->axis[1] )
            spGetInput()->axis[1] = hero.walk( fdt, spGetInput()->axis[1] );

        if ( spGetInput()->button[SP_BUTTON_R] )
        {
            spGetInput()->button[SP_BUTTON_R] = hero.jump();
        }

        
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
                    //std::cout << "\n WARNING!  block " << i << " out of bounds!\n";
                    blocks[i].remove_physics();  // remove this guy from physics world
                    if ( killboxfromblockid )
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
                                boxes[j].remove_physics();  // remove this guy from physics world
                                boxes.erase(boxes.begin() + j);
                                boxsize --;
                                activate = true;
                            }
                            else
                            {
                                //std::cout << "skipping  box " << boxes[j].id << "; "; 
                                j++;
                            }
                        }
                    }
                    blocks.erase(blocks.begin() + i);
                    blocksize --;
                    //std::cout << "\n deleted  block " << i << " since it was out of bounds.\n";
                }
                else
                {
                    i++;
                }
            }
            if (blocksize == 0)
            {
                winlevel = 3;
            }
            else if ( activate )
            {
                // boxes disappeared or something, we should reactivate everything
                for ( int i=0; i<blocks.size(); i++ )
                {
                    blocks[i].activate();
                }
                hero.object->activate();
            }
        } 
        else
        { // we have won and are waiting to start next level
            winlevel -= fdt;
            if ( winlevel <= 0.f )
            {
                level += 1;
                return reset();
            }

        }
        //std::cout << std::endl;
    }

    // return a value
    return GAMESTATEplay; // always return your own value if you want to continue updating
}


void MainMenu::handle( SDL_Event* event )
{
    // get all SDL events and do something with them...  if you want...
    if (event->type == SDL_KEYDOWN)
    {
        int lastKey = event->key.keysym.unicode;
//        char buffer[5];
//        printf("keydown event 0x%x = \"%s\" keysym=%i\n",lastKey,spFontGetUTF8FromUnicode(lastKey,buffer,5),event->key.keysym.sym);
    }
}

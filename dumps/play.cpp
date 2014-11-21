/*
Check the LICENSE file included for copyright information.  
*/
//#include <cmath>
#include "play.h"
#include <string>
#include <iostream>
#include <fstream>
#include "util.h"

Play::Play( int levelset_, int level_, char* message_ ) // init play class
{
    cameramovecooldown = 0.0f;
    sprintf( message, "%s", message_ );

    won = false;
    alive = true;

    level = level_;
    levelset = levelset_;
    sprintf( wintext, "win in" ); // default text to use when winning...
    spFontShadeButtons(1);
    lives = 12;
    totalclock = 0;
    clock = 0;
  
    // GAMEPLAY menu
    // menu is the index of the currently selected menu item.  0 if menu is not shown,
    // and conveniently 0 is not a selectable menu item, it is the title of the menu.
    menuitems.push_back( "Game Menu" ); // not allowed to select this guy, menu > 0
    menuitems.push_back( "Return to play" );
    menuitems.push_back( "Reset level" );
    menuitems.push_back( "Next level" );
    menuitems.push_back( "Camera follow speed" ); 
    menuitems.push_back( "Camera align speed" ); 
    menuitems.push_back( "Camera timeout" ); 
    menuitems.push_back( "Return to main menu" );
    menuitems.push_back( "Exit" );
    
    std::vector<float> emptylist;
    menuitemvalues.push_back( emptylist ); // game menu.  a never-used placeholder
        menuitemvalueindices.push_back( 0 );
    menuitemvalues.push_back( emptylist ); // return to play
        menuitemvalueindices.push_back( 0 );
    menuitemvalues.push_back( emptylist ); // reset level
        menuitemvalueindices.push_back( 0 );
    menuitemvalues.push_back( emptylist ); // next level
        menuitemvalueindices.push_back( 0 );
    {
        std::vector<float> camerafollowspeedvalues; // camera follow speed
        camerafollowspeedvalues.push_back(0.f);
        camerafollowspeedvalues.push_back(0.4f);
        camerafollowspeedvalues.push_back(0.8f);
        camerafollowspeedvalues.push_back(1.6f);
        camerafollowspeedvalues.push_back(3.1f);
        camerafollowspeedvalues.push_back(4.1f);
        camerafollowspeedvalues.push_back(5.9f);
        menuitemvalues.push_back( camerafollowspeedvalues );
            menuitemvalueindices.push_back( 2 );
    }
    {
        std::vector<float> cameraalignspeedvalues; // camera align speed
        cameraalignspeedvalues.push_back(0.f);
        cameraalignspeedvalues.push_back(0.4f);
        cameraalignspeedvalues.push_back(0.8f);
        cameraalignspeedvalues.push_back(1.6f);
        cameraalignspeedvalues.push_back(3.1f);
        cameraalignspeedvalues.push_back(4.1f);
        cameraalignspeedvalues.push_back(5.9f);
        menuitemvalues.push_back( cameraalignspeedvalues );
            menuitemvalueindices.push_back( 2 );
    }
    {
        std::vector<float> cameratimeoutvalues;
        cameratimeoutvalues.push_back(0.5f);
        cameratimeoutvalues.push_back(1.0f);
        cameratimeoutvalues.push_back(2.0f);
        cameratimeoutvalues.push_back(3.0f);
        cameratimeoutvalues.push_back(6.0f);
        menuitemvalues.push_back( cameratimeoutvalues );
            menuitemvalueindices.push_back( 2 );
    }
    //menuitemvalues.push_back( emptylist );
    menuitemvalues.push_back( emptylist );
        menuitemvalueindices.push_back( 0 );
    menuitemvalues.push_back( emptylist );
        menuitemvalueindices.push_back( 0 );

    // default camera behavior
    cameratimeout = 2;
    camerafollowspeed = 0.4;
    cameraalignspeed = 0.4;

    char filedirectory[256];
    spCreateDirectoryChain( get_path(filedirectory,"") );
    get_path(filedirectory,"settings.txt");

    if ( file_exists( filedirectory ) )
    {
        std::ifstream fin( filedirectory );

        std::string name;
        float value;

        while (fin >> name >> value)
        {
            /* do something with name and value */
            std::cout << "Got setting "<< name << " = " << value << "\n";
            int index = -1; // the index of the menu item, if it is allowed to be changed
            if ( name == "camerafollowspeed" )
            {
                camerafollowspeed = value;
                index = 4; // index of the menu item of camera follow hero...
            }
            else if (name == "cameraalignspeed" )
            {
                cameraalignspeed = value; // index of the menu item of camera align with hero...
                index = 5;
            }
            else if (name == "cameratimeout" )
            {
                cameratimeout = value; // index of the menu item of camera timeout
                index = 6;
            }

            if ( index > 0 )
            {
                // find value in the menu which is closest to this guy.
                int itemvalueindex = 0;
                int closestitemvalueindex = 0;
                float closestvaluediff = 1234;
                while ( itemvalueindex < menuitemvalues[index].size() )
                {
                    float valuediff = fabs( menuitemvalues[index][itemvalueindex] - value );
                    if ( valuediff < closestvaluediff )
                    {
                        closestvaluediff = valuediff;
                        closestitemvalueindex = itemvalueindex;
                    }
                    itemvalueindex += 1;
                }
                if ( menuitemvalues[index][closestitemvalueindex] > value )
                {
                    menuitemvalues[index].insert( menuitemvalues[index].begin()+closestitemvalueindex, value );
                    menuitemvalueindices[index] = closestitemvalueindex;
                }
                else if ( menuitemvalues[index][closestitemvalueindex] < value )
                {
                    menuitemvalues[index].insert( menuitemvalues[index].begin()+closestitemvalueindex+1, value );
                    menuitemvalueindices[index] = closestitemvalueindex+1;
                }
                else
                    menuitemvalueindices[index] = closestitemvalueindex;
            }
        }


    }
    else
    {
        std::cout << " No settings file, using defaults" << std::endl;
    }

    cameracenter = btVector3();
    cameradistance = spFloatToFixed( -25.0f );
    cameraaxis = SP_PI * 0.1; // for rotation around the z axis
    set_camera_incline();
    //cameraincline = spFloatToFixed( -0.8f ); // closer to zero - more from top down.

    font=NULL;
    checkertexture=NULL;

    //reset();
}

void
Play::set_alert( const char* alert, btScalar deltat )
{
    alerttime = deltat;
    sprintf( alerttext, "%s", alert );
}

void
Play::standard_physics_init()
{
    for ( int i=0; i<blocks.size(); i++ )
        blocks[i].add_physics( physics );
    
    for ( int i=0; i<boxes.size(); i++ )
        boxes[i].add_physics( physics );

    for ( int i=0; i<ramps.size(); i++ )
        ramps[i].add_physics( physics );
}

int
Play::set_value( const char* name, float value )
{
    if ( name == "camerafollowspeed" || name == "Camera follow speed" )
        camerafollowspeed = value;
    else if (name == "cameraalignspeed" || name == "Camera align speed" )
        cameraalignspeed = value;
    else if (name == "cameratimeout" || name == "Camera timeout" )
        cameratimeout = value;
    else
        return 1;

    return 0;
}

void
Play::write_settings()
{
    char filedirectory[256];
    spCreateDirectoryChain( get_path(filedirectory,"") );
    get_path(filedirectory,"settings.txt");

    std::ofstream file;
    file.open( filedirectory );
    file << "camerafollowspeed " << camerafollowspeed << "\n";
    file << "cameraalignspeed " << cameraalignspeed << "\n";
    file << "cameratimeout " << cameratimeout << "\n";
    file.close();
}

void
Play::deinit()
{
    blocks.clear();
    boxes.clear();
    ramps.clear();
   
    if ( font )
        spFontDelete( font );
    font=NULL;

    if ( checkertexture )
        spDeleteSurface(checkertexture);
    checkertexture=NULL;

    for ( int i=0; i<numbertexture.size(); i++ )
        if ( numbertexture[i] )
        {
            spDeleteSurface( numbertexture[i] );
            numbertexture[i] = NULL;
        }
    numbertexture.clear();
    
    physics.deinit();
}

int
Play::reset()
{
    return gamestate;
//    std::cout << " play reset " << std::endl;
//    sprintf( lvltext, "lvl. %i", level );
//    std::cout << " lvltext = " << lvltext << std::endl;
//    //sprintf( buffer, "lvl. %i", level );
//
//    totalclock += clock;
//
//    deinit(); // kill everything first
////    previous_t = time(0);
////    current_t = time(0);
//
//    killboxfromblockid = false;
//
//
//    // then rebirth it all...
//    winlevel = 0.f; // you haven't won yet!
//    pause = 0; // default to no pause
//    menu = 0;
//    clock = 0;
//    checkertexture = spLoadSurface("../data/check.png");

//    if ( level == 1 )
//    {
//        // load level obstacles and such
//    }
//    else
//    {
//        std::cout << " congratulations, you beated all levels! " << std::endl;
//        std::cout << " total accumulated time: " << totalclock << std::endl;
//        std::cout << " remaining lives: " << lives << std::endl;
//        std::cout << " hopefully they will get around to creating level " << level << std::endl;
//        return GAMESTATEquit;
//    }
//    
//    // now add physics to everybody 
//    physics.init();
//    hero.object->add_physics( physics );
//    
//    for ( int i=0; i<blocks.size(); i++ )
//        blocks[i].add_physics( physics );
//    
//    for ( int i=0; i<boxes.size(); i++ )
//        boxes[i].add_physics( physics );
//
//    for ( int i=0; i<ramps.size(); i++ )
//        ramps[i].add_physics( physics );
    
//    spDrawInExtraThread(0);
    //spDrawInExtraThread(1);
//    std::cout << "end play reset " << std::endl;
//    return GAMESTATEplay;
}


void Play::draw( SDL_Surface* screen )
{
    if (!(font))
        load_font();

    // the first part of this stuff is drawing buttons and what not.
    spSetZSet( 0 );
    spSetZTest( 0 );
    spSetAlphaTest( 1 );

    char buffer[64]; 
    if (menu)
    {
        spFontDrawRight( screen->w - 2 , font-> maxheight+2, 0, "[S] Select", font );
        spFontDraw( 2, font-> maxheight+4, 0, "[E] Select", font ); 
        int y = 3*screen->h/7 - ((menuitems.size()-1)/2)*(font->maxheight);

        sprintf( buffer, "= %s =", menuitems[0] );
        spFontDrawMiddle( screen->w / 2, y, 0, buffer, font );
        y += font->maxheight*3/2;

        for ( int i=1; i<menuitems.size(); i++)
        {
            if ( i == menu )
            {
                if ( menuitemvalues[i].size() > 0 )
                {
                    sprintf( buffer, "+ %s < %.1f > +", menuitems[i], menuitemvalues[i][menuitemvalueindices[i]] );
                }
                else    
                    sprintf( buffer, "+ %s +", menuitems[i] );
            }
            else
                sprintf( buffer, "%s", menuitems[i] );
            spFontDrawMiddle( screen->w / 2, y, 0, buffer, font );
            y += font->maxheight + 4;
            if ( i == 3 || i == 6 )
                y += (font->maxheight*2/5);
        } 
        
        spFontDraw( 2, screen->h - 1*font->maxheight,0, "D-pad Navigate Menu", font); 
    }
    else
    {   // no menu
        spFontDraw( 2, 2, 0, "[L] Kick", font ); 
        spFontDrawRight( screen->w - 2 , 2, 0, "[R] Jump", font );

        spFontDrawRight( screen->w - 2 , font-> maxheight+2, 0, "[S] Menu", font );
        switch (pause)
        {
            case 0:  spFontDraw( 2, font-> maxheight+4, 0, "[E] Pause", font ); break;
            case 1:  spFontDraw( 2, font-> maxheight+4, 0, "[E] Unpause", font ); break;
        }

        spFontDraw( 2, screen->h - 1*font->maxheight,0, "D-pad Move Player", font); 

        spFontDrawRight( screen->w - 2, screen->h - 2*font-> maxheight, 0, "[Y] Zoom in", font ); 
        spFontDrawRight( screen->w - 2, screen->h - 1*font-> maxheight, 0, "[X] Zoom out", font );

        
        if ( winlevel > 0.f )
        {
            sprintf( buffer, "%s %d", wintext, int(ceil(winlevel)) );
            spFontDrawMiddle( screen->w / 2, screen->h / 2, 0, buffer, font );

            if ( totalclock+clock < 999.0f )
            {
                sprintf( buffer, "accumulated time: %.2f", (totalclock+clock) );
                spFontDrawMiddle( screen->w / 2, screen->h / 2 + font->maxheight + 2, 0, buffer, font );
            }
        }
        else if ( alerttime > 0.f )
        {
            spFontDrawMiddle( screen->w / 2, screen->h / 2, 0, alerttext, font ); 
        }
    }
    //spFontDrawMiddle( screen->w /2, screen->h - 2*font-> maxheight, 0, input, font );


    sprintf( buffer, "cube dump: %s", lvltext );
    spFontDrawMiddle( screen->w / 2, font->maxheight + 2, 0, buffer, font );
    
    sprintf( buffer, "fps: %i", spGetFPS() );
    spFontDrawMiddle( screen->w/2, 1, 0, buffer, font ); 

    if ( clock < 1000.0f )
    {
        sprintf( buffer, "time: %.2f", clock );
        spFontDrawMiddle( screen->w / 2, 2*(font->maxheight) + 4 , 0, buffer, font );
    }
  
    if ( lives < 1000 && levelset )
    {
        if ( lives == 1 )
            sprintf( buffer, "life: %d", lives );
        else
            sprintf( buffer, "lives: %d", lives );
        spFontDrawMiddle( screen->w / 2, screen->h  - (font->maxheight), 0, buffer, font );
    }

    // this displays everything. (??)
    spFlip(); 

    // we predraw things for next time.  (??)
    spClearTarget( spGetRGB(64,64,64) ); // set background color
    spResetZBuffer(); // not sure what this is
    spSetZSet( 1 ); // one of these guys helps with depth perception, i.e.
    spSetZTest( 1 ); // not drawing background objects in the foreground.

    spSetPerspectiveTextureMapping(1); // textures aren't so weird
    spSetAlphaTest( 0 );  // this makes purple not invisible

    spSetLightPosition(0,spFloatToFixed( 0.875f ),spFloatToFixed( 1.875f ),spFloatToFixed( 4.875f ));
    spSetLightColor(0,SP_ONE,SP_ONE,SP_ONE);
    spSetLight( 1 ); // 0 for no lights, 1 for lights

    // set the camera matrix
    spIdentity();
    // draw debuggy type things
//    if (pause)
//        hero.draw_debug();

    // continue with camera matrix
    spTranslate( 0, 0, cameradistance );
    spRotateX( cameraincline );
    spRotateZ( cameraaxis );
    //spTranslate( 0, 0, spFloatToFixed( -1.0f ) ); // go a bit up from the player

    // also get to center of camera
    if ( camerafollowspeed > 0.f )
        spTranslate( spFloatToFixed( -cameracenter.x() ), 
                     spFloatToFixed( -cameracenter.y() ), 
                     spFloatToFixed( -cameracenter.z() ) + cameradistance/5  ); // previous -SP_ONE
   
    // grab the camera matrix for later usage.
    Sint32 matrix[16]; //pointer to array of 16 Sint32's.
    memcpy(matrix,spGetMatrix(),16*sizeof(Sint32)); //need to reload this after every draw.

    if ( (alerttime > 0.f) || menu )
    {
        // don't draw text if alert is showing
        for (int i=0; i<boxes.size(); i++)
        {
            boxes[i].draw( screen, matrix, NULL );  // this draws and resets the model view to matrix
        }
        
        for (int i=0; i<ramps.size(); i++)
        {
            ramps[i].draw( screen, matrix, NULL ); 
        }

        for (int i=0; i<blocks.size(); i++)
        {
            blocks[i].draw( screen, matrix, NULL, 200 ); 
            // draw blocks at partial transparency.  max alpha = 255 (fully opaque), 0 = fully transparent
        }
        
        if ( alive )
            hero.object->draw( screen, matrix, NULL );
    }
    else
    {
        for (int i=0; i<boxes.size(); i++)
        {
            boxes[i].draw( screen, matrix, font );  // this draws and resets the model view to matrix
        }
        
        for (int i=0; i<ramps.size(); i++)
        {
            ramps[i].draw( screen, matrix, font ); 
        }

        for (int i=0; i<blocks.size(); i++)
        {
            blocks[i].draw( screen, matrix, font, 200 ); 
            // draw blocks at partial transparency.  max alpha = 255 (fully opaque), 0 = fully transparent
        }

        if ( alive )
            hero.object->draw( screen, matrix, font );

    }


    //spDeactivatePattern();
    spSetPerspectiveTextureMapping(0);
}


int Play::update( Uint32 dt )
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
                    return gamestate;
                    break;
                case 2: // reset level
                    lives -= 1; // penalize the player
                    clock = 0.0;
                    return reset();
                case 3: // Next level
                    menu=0; // unmenu
                    pause=0; // unpause
                    if ( lives >= 6 )
                    {
                        if ( lives < 100000 )
                            lives -= 6; // penalize the player
                        clock=1000.0f; // penalize the player
                        won = true; // the player "won"...
                        return reset();
                    }
                    else
                        set_alert( "Not enough lives to skip a level" );
                    break;
                case 4: // camerafollowspeed
                case 5: // cameraalignspeed
                case 6: // cameratimeout
                    menu=0;
                    pause=0;
                    return gamestate;
                    break;
                case 7: // return to menu
                    menu=0;
                    pause=0;
                    if ( gamestate == GAMESTATEmenu )
                    {
                        if ( levelset > 1 )
                        {
                            levelset = 1;
                            level = 1;
                            reset();
                        }
                    }
                    else
                    {
                        level=0;
                        levelset=gamestate;
                    }
                    return GAMESTATEmenu;
                    break; 
                case 8: // Exit
                    return GAMESTATEquit;
                    break; 
            }
        }

        // DON'T allow any other things to go on when you are navigating
        // the menu.  so prematurely disrupt update function.
        return gamestate;
    }

    // otherwise, if we hit start we want to get to menu
    if ( spGetInput()->button[SP_BUTTON_START] )
    {
        menu = 1;
        // unset the start button so it won't grab the first menu item.
        spGetInput()->button[SP_BUTTON_START] = 0;
        return gamestate;
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
        cameramovecooldown = cameratimeout;
    }
    if ( spGetInput()->button[SP_BUTTON_B] )
    {
        // key D on a standard QWERTY keyboard
        cameraaxis -= std::max(int(80*dt),1);
        cameramovecooldown = cameratimeout;
    }
    if ( spGetInput()->button[SP_BUTTON_X] )
    {
        // key S on a QWERTY
        //spGetInput()->button[SP_BUTTON_x] = 0; // can only allow input once if desired
        if ( cameradistance < -15*SP_ONE )
            cameradistance -= 1000*dt ;
        else
            cameradistance -= -cameradistance*dt/(1000) +dt;
        //cameraincline = spFloatToFixed(3.5)/( 1 + (abs(cameradistance)/(SP_ONE/5)))-10*SP_PI / ( 1 + (abs(cameradistance)/(SP_ONE/3)) ) ;
        set_camera_incline();
        //cameraincline +=dt * abs(cameraincline)/1000 * abs(SP_PI+cameraincline)/50000;
        //std::cout << " cameradistance/SP_ONE =  " << (cameradistance/SP_ONE) << "\n";
    }
    else if ( spGetInput()->button[SP_BUTTON_Y] )
    {
        // key W on a QWERTY
        //spGetInput()->button[SP_BUTTON_Y] = 0;
        if ( cameradistance < -15*SP_ONE )
            cameradistance += 1000*dt ;
        else
            cameradistance += -cameradistance*dt/(1000) +dt;
        set_camera_incline();
        //cameraincline = spFloatToFixed(3.5)/( 1 + (abs(cameradistance)/(SP_ONE/5))) -10*SP_PI / ( 1 + (abs(cameradistance)/(SP_ONE/3)) ) ;
        //cameraincline -= dt * abs(cameraincline)/1000* abs(SP_PI+cameraincline)/50000;
        //std::cout << " cameradistance/SP_ONE =  " << (cameradistance/SP_ONE) << "\n";
    }

    if ( spGetInput()->button[SP_BUTTON_L] )
    {   // attempt to kick.  this is key Q on a keyboard
        // check if player is pressing forward or back
        spGetInput()->axis[1] = hero.kick( spGetInput()->axis[1] );
        spGetInput()->button[SP_BUTTON_L] = 0;
    }

    if (!(pause))
    {
        btScalar fdt = dt*1.0/1000;
        
        // update the physics world
        physics.update( fdt );
        // update hero stuff
        if ( update_hero( fdt ) )
            return reset();
        // update blocks and what not 
        if ( update_level( fdt ) )
            return reset();
        //std::cout << std::endl;
    }

    // return a value
    return gamestate; // always return your own value if you want to continue updating
}

void Play::set_camera_incline()
{
    if ( cameradistance < -15*SP_ONE )
        // -10*SP_PI / ( 1 + 25*SP_ONE / (SP_ONE/5) ) = -10*SP_PI / (1 + 25*5)
        // = -10*SP_PI / 126  = -approx 0.249333 * SP_ONE
        // = -SP_ONE / 4.01
        //cameraincline = -SP_ONE / 4.01; //cameradistance / 6; 
        //cameraincline = cameradistance/100.32 - (cameradistance+25*SP_ONE)*0.13;
        cameraincline = -0.87*SP_ONE  - 0.014*(cameradistance+15*SP_ONE);//spSqrt(-(cameradistance+20*SP_ONE)*0.05) - 0.87*SP_ONE;
    else if ( cameradistance < -5*SP_ONE )
        cameraincline = -(cameradistance+15*SP_ONE)*0.07 - 0.87*SP_ONE;
    else
        cameraincline = SP_ONE * (-10*0.07-0.87);

    
    //cameraincline = spFloatToFixed(-0.2) -10*SP_PI / ( 1 + (abs(cameradistance)/(SP_ONE/3)) ) ;
}

int Play::update_hero( btScalar fdt )
{
    hero.update( fdt );
    if ( hero.object->out_of_bounds( outofbounds ) )
    {
        lives -= 1;
        clock = 0;
        alive = false;
        return 1;
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

    return 0;
}


int Play::update_level( btScalar fdt )
{

    // update boxes
    for ( int i=0; i<boxes.size(); i++ )
        boxes[i].update( fdt );
    // update ramps
    for ( int i=0; i<ramps.size(); i++ )
        ramps[i].update( fdt );

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
                            //boxes.erase(boxes.begin() + j);
                            //boxsize --;
                            activate = true;
                        }
                        j++;
//                        else
//                        {
//                            //std::cout << "skipping  box " << boxes[j].id << "; "; 
//                            j++;
//                        }
                    }
                }
//                else if ( addboxfromblockid )
//                {
//                    int blockid = blocks[i].id;
//                    int j=0;
//                    int boxsize = boxes.size();
//                    while ( j < boxsize )
//                    {
//                        if ( boxes[j].id == blockid )
//                        {
//                            boxes[j].add_physics( physics );  // add this guy to physics world
//                        }
//                        j++;
//                    }
//                    
//                }
                // finally, destroy this block from existence
                blocks.erase(blocks.begin() + i);
                blocksize --;
                //std::cout << "\n deleted  block " << i << " since it was out of bounds.\n";
            }
            else
            {
                i++;
            }
        }
        // check win conditions...
        if (blocksize == 0)
        {
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
            won = true;
            return 1;
        }

    }
    return 0; // no reset if update_level doesn't do nothing

}

void Play::resize( Uint16 w, Uint16 h )
{
    spSelectRenderTarget(spGetWindowSurface());
    //Setup of the new/resized window
    spSetPerspective( 30.0,  // kinda like the zoom factor
                      ( float )spGetWindowSurface()->w / ( float )spGetWindowSurface()->h, 
                      0.5f, 100.0f ); // z near and z far
    //spSetPerspective( 45.0, ( float )spGetWindowSurface()->w / ( float )spGetWindowSurface()->h, 
    //                  1.0f, 100.0f );

    //Font Loading
    if ( font )
        spFontDelete( font );

    load_font();
}

void Play::load_font()
{
    font = spFontLoad( "../font/Play-Bold.ttf", spFixedToInt(10 * spGetSizeFactor()));
    spFontSetShadeColor(0);
    spFontAdd( font, SP_FONT_GROUP_ASCII, 65535 ); //whole ASCII
    spFontAdd( font, "äüöÄÜÖßẞ", 65535 ); //German stuff (same like spFontAdd( font, SP_FONT_GROUP_GERMAN, 0 ); )
    spFontAddBorder( font, 0 );
    spFontSetButtonStrategy(SP_FONT_BUTTON);
    spFontAddButton( font, 'A', SP_BUTTON_A_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'B', SP_BUTTON_B_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'X', SP_BUTTON_X_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'Y', SP_BUTTON_Y_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'L', SP_BUTTON_L_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'R', SP_BUTTON_R_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontSetButtonStrategy(SP_FONT_INTELLIGENT);
    spFontAddButton( font, 'S', SP_BUTTON_START_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'E', SP_BUTTON_SELECT_NAME, 65535, spGetRGB( 64, 64, 64 ) );
}


void Play::handle( SDL_Event* event )
{
    // get all SDL events and do something with them...  if you want...
//    if (event->type == SDL_KEYDOWN)
//    {
//        int lastKey = event->key.keysym.unicode;
//        char buffer[5];
//        printf("keydown event 0x%x = \"%s\" keysym=%i\n",lastKey,spFontGetUTF8FromUnicode(lastKey,buffer,5),event->key.keysym.sym);
//    }
}


int Play::save_time_if_best()
{
    char filedir[64];
    sprintf( filedir, "%i-%i.dat", gamestate, level );
    float oldbest = loadtime( filedir );
    if ( clock < oldbest )
    {
        savetime( filedir, (float) clock );
        std::cout << " NEW BEST TIME! for level " << filedir << ": " << clock << "\n";
        return 1;
    }
    else
        return 0;
}

Play::~Play()
{
    std::cout << " exiting play mode " << std::endl;
    deinit();
}

/*
Check the LICENSE file included for copyright information.  
*/
//#include <cmath>
#include "play.h"
//#include <strings.h>
#include <string>
//using std::string;
#include <iostream>

Play::Play( int level_ ) // init play class
{
    spFontShadeButtons(1);
    lives = 12;
    totalclock = 0;
    clock = 0;

    distance = spFloatToFixed( -25.0f );
    axis = SP_PI * 0.1;
    rotation = spFloatToFixed( -1.5f ); // closer to zero - more from top down.

    font=NULL;
    checkertexture=NULL;

    level = level_;

    // also, menu is the index of the currently selected menu item. 
    menuitems.push_back( "Game Menu" ); // not allowed to select this guy, menu > 0
    menuitems.push_back( "Return to play" );
    menuitems.push_back( "Next level (deduct 10 lives)" );
    //menuitems.push_back( "Exit to main menu" );
    menuitems.push_back( "Exit" );

    reset();
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
    
    physics.deinit();
}

int
Play::reset()
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


void Play::draw( SDL_Surface* screen )
{
    if (!(font))
        load_font();

    // the first part of this stuff is drawing buttons and what not.
    spSetZSet( 0 );
    spSetZTest( 0 );
    spSetAlphaTest( 1 );
    spFontDraw( 2, 2, 0, "[L] Reset", font ); 
    spFontDrawRight( screen->w - 2 , 2, 0, "[R] Jump", font );

    if (menu)
    {
        spFontDrawRight( screen->w - 2 , font-> maxheight+2, 0, "[S] Select", font );
        spFontDraw( 2, font-> maxheight+4, 0, "[E] Select", font ); 
        int y = 2*screen->h/5 - ((menuitems.size()+1)/2)*(font->maxheight);
        for ( int i=0; i<menuitems.size(); i++)
        {
            char buffer[64]; 
            if ( i == menu )
                sprintf( buffer, "+%s+", menuitems[i] );
            else
                sprintf( buffer, "%s", menuitems[i] );
            spFontDrawMiddle( screen->w / 2, y, 0, buffer, font );
            y += font->maxheight + 4;
            if ( i == 0 )
            {
                y += 4;
            }
        } 
        
        spFontDraw( 2, screen->h - 1*font->maxheight,0, "D-pad Navigate Menu", font); 
    }
    else
    {
        spFontDrawRight( screen->w - 2 , font-> maxheight+2, 0, "[S] Menu", font );
        switch (pause)
        {
            case 0:  spFontDraw( 2, font-> maxheight+4, 0, "[E] Pause", font ); break;
            case 1:  spFontDraw( 2, font-> maxheight+4, 0, "[E] Unpause", font ); break;
        }

        spFontDraw( 2, screen->h - 1*font->maxheight,0, "D-pad Move Player", font); 
    }
    //spFontDrawMiddle( screen->w /2, screen->h - 2*font-> maxheight, 0, input, font );
    spFontDrawRight( screen->w - 2, screen->h - 2*font-> maxheight, 0, "[Y] Zoom in", font ); 
    spFontDrawRight( screen->w - 2, screen->h - 1*font-> maxheight, 0, "[X] Zoom out", font );



    char buffer[64]; 
    sprintf( buffer, "cube dump lvl. %i", level );
    spFontDrawMiddle( screen->w / 2, font->maxheight + 2, 0, buffer, font );

    sprintf( buffer, "fps: %i", spGetFPS() );
    spFontDrawMiddle( screen->w/2, 1, 0, buffer, font );
    
    sprintf( buffer, "time: %.2f", clock );
    spFontDrawMiddle( screen->w / 2, 2*(font->maxheight) + 4 , 0, buffer, font );

    if ( winlevel > 0.f )
    {
        sprintf( buffer, "win in %d", int(ceil(winlevel)) );
        spFontDrawMiddle( screen->w / 2, screen->h / 2, 0, buffer, font );

        sprintf( buffer, "accumulated time: %.2f", (totalclock+clock) );
        spFontDrawMiddle( screen->w / 2, screen->h / 2 + font->maxheight + 2, 0, buffer, font );
    }
   
    if ( lives == 1 )
        sprintf( buffer, "life: %d", lives );
    else
        sprintf( buffer, "lives: %d", lives );
    spFontDrawMiddle( screen->w / 2, screen->h  - (font->maxheight), 0, buffer, font );

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
    spTranslate( 0, 0, distance );
    spRotateX( rotation );
    spRotateZ( axis );
    spTranslate( 0, 0, spFloatToFixed( -1.0f ) ); // go a bit up from the player
   
    // grab the camera matrix for later usage.
    Sint32 matrix[16]; //pointer to array of 16 Sint32's.
    memcpy(matrix,spGetMatrix(),16*sizeof(Sint32)); //need to reload this after every draw.

    for (int i=0; i<boxes.size(); i++)
    {
        boxes[i].draw( matrix );  // this draws and resets the model view to matrix
    }
    
    for (int i=0; i<ramps.size(); i++)
    {
        ramps[i].draw( matrix ); 
    }

    for (int i=0; i<blocks.size(); i++)
    {
        blocks[i].draw( matrix, 200 ); 
        // draw blocks at partial transparency.  max alpha = 255 (fully opaque), 0 = fully transparent
    }

    hero.object->draw( matrix );

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
                case 3: // Exit
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
        axis += 150*dt;
    }
    if ( spGetInput()->button[SP_BUTTON_B] )
    {
        // key D on a standard QWERTY keyboard
        axis -= 150*dt;
    }
    if ( spGetInput()->button[SP_BUTTON_X] )
    {
        // key S on a QWERTY
        //spGetInput()->button[SP_BUTTON_x] = 0; // can only allow input once if desired
        distance -= 1000*dt;
        rotation += 50*dt;
    }
    else if ( spGetInput()->button[SP_BUTTON_Y] )
    {
        // key W on a QWERTY
        //spGetInput()->button[SP_BUTTON_Y] = 0;
        distance += 1000*dt;
        rotation -= 50*dt;
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
    if (event->type == SDL_KEYDOWN)
    {
        int lastKey = event->key.keysym.unicode;
//        char buffer[5];
//        printf("keydown event 0x%x = \"%s\" keysym=%i\n",lastKey,spFontGetUTF8FromUnicode(lastKey,buffer,5),event->key.keysym.sym);
    }
}

Play::~Play()
{
    std::cout << " exiting play mode " << std::endl;
    deinit();
}

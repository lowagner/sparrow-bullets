/*
 The contents of this file are subject to the "do whatever you like"-license.
 That means: Do, whatver you want, this file is under public domain. It is an
 example for sparrow3d (with bullet3d). Copy it and learn from it for your 
 project and release it under every license you want. ;-)
 For feedback and questions about my Files and Projects please mail me,
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com
 added to by Lucas Wagner (lowagner), low agner _at_ gmail.com
 mostly for the bullets.
*/
//#include <cmath>
#include "play.h"
//#include <strings.h>
#include <string>
//using std::string;
#include <iostream>

Play::Play() // init play class
{
    spFontShadeButtons(1);

    distance = spFloatToFixed( -25.0f );
    axis = SP_PI * 0.1;
    rotation = spFloatToFixed( -1.5f ); // closer to zero - more from top down.

    //input.push_back("");
    iamdone = 0;

    font=NULL;
    checkertexture=NULL;

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

void
Play::reset()
{
    deinit(); // kill everything first
//    previous_t = time(0);
//    current_t = time(0);

    // then rebirth it all...
    pause = 1;
    clock = 0;
    checkertexture = spLoadSurface("../data/check.png");

    outofbounds = btVector3(20,20,20); //anything outside of these half-lengths is considered OB!

    // this guy includes the floor.  all static rectangular prisms.
    boxes.push_back( Box( btVector3(10,10,1), btVector3(0,0,-5), 0x05FF ) ); // half-sizes, pos, color
    boxes.push_back( Box( btVector3(4,4,1), btVector3(7,7,-3), 0x055F ) ); // half-sizes, pos, color

    ramps.push_back( Ramp( btVector3(10,2,4), btVector3(-4,0,-3), 0xF0FF ) ); // sizes, pos, color
    ramps.push_back( Ramp( btVector3(10,2.5,1), btVector3(-1,0,-4), 0xFFFF ) ); // sizes, pos, color
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
    
    // now add physics to everybody 
    physics.init();
    hero.object->add_physics( physics );
    
    for ( int i=0; i<blocks.size(); i++ )
    {
        blocks[i].add_physics( physics );
        blocks[i].id = i;
    }
    
    for ( int i=0; i<boxes.size(); i++ )
        boxes[i].add_physics( physics );

    for ( int i=0; i<ramps.size(); i++ )
        ramps[i].add_physics( physics );
    
    spDrawInExtraThread(0);
    //spDrawInExtraThread(1);
}


void Play::draw( SDL_Surface* screen )
{
    if (!(font))
        load_font();

    // the first part of this stuff is drawing buttons and what not.
    spSetZSet( 0 );
    spSetZTest( 0 );
    spSetAlphaTest( 1 );
    spFontDraw( 2, font-> maxheight+2, 0, "[L] Reset", font );
    spFontDrawRight( screen->w - 2 , font-> maxheight+2, 0, "[R] Jump", font );
    spFontDrawRight( screen->w - 2 , 2, 0, "[S] Exit", font );
    spFontDrawRight( screen->w - 2, screen->h - 2*font-> maxheight, 0, "[Y] Zoom in", font ); 
    spFontDrawRight( screen->w - 2, screen->h - 1*font-> maxheight, 0, "[X] Zoom out", font );

    switch (pause)
    {
        case 0:  spFontDraw( 2, 2, 0, "[E] Pause", font ); break;
        case 1:  spFontDraw( 2, 2, 0, "[E] Unpause", font ); break;
    }
    //spFontDrawMiddle( screen->w /2, screen->h - 2*font-> maxheight, 0, input, font );

    spFontDrawMiddle( screen->w / 2, font->maxheight + 2, 0, "Cube Drop", font );	
    spFontDraw( 2, screen->h - 1*font->maxheight,0, "D-pad Move Player", font); 
    //spFontDraw( 2, screen->h - 2*font->maxheight,0, "[B] rotate right", font);

    char buffer[64];
    sprintf( buffer, "fps: %i", spGetFPS() );
    spFontDrawMiddle( screen->w/2, 1, 0, buffer, font );
    // this displays everything. (??)
    spFlip(); 

    // we predraw things for next time.  (??)
    spClearTarget( spGetRGB(64,64,64) ); // set background color
    spResetZBuffer(); // not sure what this is
    spSetZSet( 1 ); // one of these guys helps with depth perception, i.e.
    spSetZTest( 1 ); // not drawing background objects in the foreground.

    spSetPerspectiveTextureMapping(1); // textures aren't so weird
    spSetAlphaTest( 0 );  // this makes purple not invisible

    spSetLightPosition(0,spFloatToFixed( 0.875f ),spFloatToFixed( 0.875f ),spFloatToFixed( 0.875f ));
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
        // draw at partial transparency.  max alpha = 255 (fully opaque), 0 = fully transparent
    }

    hero.object->draw( matrix );

    //spDeactivatePattern();
    spSetPerspectiveTextureMapping(0);
}


int Play::update( Uint32 dt )
{
    // inputy type things
    if ( spGetInput()->axis[0]==-1 )
    {
//        axis -= 150*dt;
        //std::cout << " axis = " << axis << std::endl;
    }
    else if ( spGetInput()->axis[0]==1 )
    {
//        axis += 150*dt;
        //std::cout << " axis = " << axis << std::endl;
    }

    if ( spGetInput()->button[SP_BUTTON_START] )
        iamdone = 1;

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
        reset();
    }

    if (!(pause))
    {
        btScalar fdt = dt*1.0/1000;
        clock += fdt;
        
//        current_t = time(0);
//        double time = difftime(current_t, previous_t);
//        sleep(3);
//        std::cerr << " sparrow dt = " << fdt << ", c++ sec = " << time << ";  ";
//        previous_t = current_t;


        physics.update( fdt );
        // update hero stuff
        hero.update( fdt );
        
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


        // update blocks
        int i=0;
        int blocksize = blocks.size();
        while ( i < blocksize )
        {
            //std::cout << i << "; id " << blocks[i].id << " ";
            blocks[i].update( fdt );

            if (  blocks[i].out_of_bounds( outofbounds )   )
            {
                //std::cout << "\n WARNING!  block " << i << " out of bounds!\n";
                blocks[i].remove_physics();  // remove this guy from physics world
                blocks.erase(blocks.begin() + i);
                blocksize --;
                //std::cout << "\n deleted  block " << i << " since it was out of bounds.\n";
            }
            else
            {
                i++;
            }
        }
        //std::cout << std::endl;
    }

    // return a value
    if (iamdone)
        return GAMESTATEquit; // return the quit gamestate to quit
    else
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
        char buffer[5];
        printf("keydown event 0x%x = \"%s\" keysym=%i\n",lastKey,spFontGetUTF8FromUnicode(lastKey,buffer,5),event->key.keysym.sym);
    }
}

Play::~Play()
{
    std::cout << " exiting play mode " << std::endl;
    deinit();
}

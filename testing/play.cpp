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
    //delete hero;

//    for ( int i=0; i<blocks.size(); i++ )
//        blocks[i]
    blocks.clear();
    
//    for ( int i=0; i<boxes.size(); i++ )
//        boxes[i].remove();
    boxes.clear();
    
//    for ( int i=0; i<ramps.size(); i++ )
//        ramps[i].remove();
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

    // then rebirth it all...
    pause = 1;
    checkertexture = spLoadSurface("../data/check.png");

    outofbounds = sbVector(20,20,20); //anything outside of these half-lengths is considered OB!

    // this guy includes the floor.  all static rectangular prisms.
    boxes.push_back( Box( sbVector(10,10,1), sbVector(0,0,-5), 0x05FF ) ); // half-sizes, pos, color

    ramps.push_back( Ramp( sbVector(10,2,4), sbVector(-3,0,-3), 0xF0FF ) ); // sizes, pos, color
    //ramps[0].rotate( sbVector(0,0,1), SP_PI*0.5 );
    ramps.push_back( Ramp( sbVector(5,2.5,3), sbVector(-1,0,-4), 0xFFFF ) ); // sizes, pos, color
    ramps[0].rotateZ( SP_PI*0.5 );
    ramps[1].rotateZ( SP_PI*0.1 );

    //world.add_box( 10,10,2, sbVector(0,0,-1) ); // add the floor
    hero = Cube( sbVector(0,0,10), 0xF00F, checkertexture );
   
    std::cout << std::endl;
    std::cout << " creating new blocks " << std::endl;
    std::cout << std::endl;
    // add some blocks pieces
    for ( int i=0; i<5; i++ )
    {
        blocks.push_back(  Cube( sbVector(2*i-3,0,10+2*i), 0x0F0F )  );
    }
    std::cout << std::endl;
    std::cout << " end creating new blocks " << std::endl;
    std::cout << std::endl;

    // now add physics to everybody 
    physics.init();
    hero.add_physics( physics );

    for ( int i=0; i<boxes.size(); i++ )
        boxes[i].add_physics( physics );
    
    for ( int i=0; i<blocks.size(); i++ )
    {
        blocks[i].add_physics( physics );
        std::cout << " naming new block " << i << std::endl;
        std::cout << std::endl;
        blocks[i].id = i;
    }
    
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
	spFontDraw( 2, font-> maxheight+2, 0, "[L] reset", font );
	spFontDrawRight( screen->w - 2 , font-> maxheight+2, 0, "[R] N/A", font );
	spFontDrawRight( screen->w - 2 , 2, 0, "[S] Exit", font );
	spFontDrawRight( screen->w - 2, screen->h - 2*font-> maxheight, 0, "[Y] Zoom in", font ); 
	spFontDrawRight( screen->w - 2, screen->h - 1*font-> maxheight, 0, "[X] Zoom out", font );
	
    switch (pause)
	{
		case 0:  spFontDraw( 2, 2, 0, "[E] Pause", font ); break;
		case 1:  spFontDraw( 2, 2, 0, "[E] Unpause", font ); break;
	}
	//spFontDrawMiddle( screen->w /2, screen->h - 2*font-> maxheight, 0, input, font );

    spFontDrawMiddle( screen->w / 2, font->maxheight + 2, 0, "Falling Cube", font );	
    spFontDraw( 2, screen->h - 1*font->maxheight,0, "D-pad rotate view", font); 
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
    spTranslate( 0, 0, distance );
	spRotateX( rotation );
	spRotateZ( axis );
    spTranslate( 0, 0, spFloatToFixed( -1.0f ) ); // go a bit up from the player
   
    // grab the camera matrix for later usage.
    Sint32 matrix[16]; //pointer to array of 16 Sint32's.
    memcpy(matrix,spGetMatrix(),16*sizeof(Sint32)); //need to reload this after every draw.

    for (int i=0; i<boxes.size(); i++)
    {
        boxes[i].draw_mess(); // remember to reload camera matrix after this.  
        memcpy(spGetMatrix(),matrix,16*sizeof(Sint32)); //reload camera matrix after every draw
    }
    
    for (int i=0; i<ramps.size(); i++)
    {
        ramps[i].draw_mess(); // remember to reload camera matrix after this.
        memcpy(spGetMatrix(),matrix,16*sizeof(Sint32)); //reload camera matrix after every draw
    }

    for (int i=0; i<blocks.size(); i++)
    {
        blocks[i].draw_mess( 200 ); // remember to reload camera matrix after this.
        // draw at partial transparency.  max alpha = 255 (fully opaque), 0 = fully transparent
        memcpy(spGetMatrix(),matrix,16*sizeof(Sint32)); //reload camera matrix after every draw
    }

    hero.draw_mess(); // remember to reload camera matrix after this.
    memcpy(spGetMatrix(),matrix,16*sizeof(Sint32)); //reload camera matrix after every draw 
    // need to reload camera matrix at the end or lights will go crazy.

	//spDeactivatePattern();
	spSetPerspectiveTextureMapping(0);
}


int Play::update( Uint32 dt )
{
    // inputy type things
	//if ( spGetInput()->button[SP_BUTTON_A] )
    if ( spGetInput()->axis[0]==-1 )
	{
		spGetInput()->button[SP_BUTTON_A] = 0;
        axis -= 150*dt;
        //std::cout << " axis = " << axis << std::endl;
	}
	//if ( spGetInput()->button[SP_BUTTON_B] )
    else if ( spGetInput()->axis[0]==1 )
	{
		spGetInput()->button[SP_BUTTON_B] = 0;
        axis += 150*dt;
        //std::cout << " axis = " << axis << std::endl;
	}

	if ( spGetInput()->button[SP_BUTTON_START] )
        iamdone = 1;
	
    if ( spGetInput()->button[SP_BUTTON_SELECT] )
    {
        spGetInput()->button[SP_BUTTON_SELECT] = 0;
        pause = 1-pause;
    }

	if ( spGetInput()->button[SP_BUTTON_X] )
	{
		//spGetInput()->button[SP_BUTTON_X] = 0;
        distance -= 1000*dt;
	}
	else if ( spGetInput()->button[SP_BUTTON_Y] )
	{
		//spGetInput()->button[SP_BUTTON_Y] = 0;
        distance += 1000*dt;
	}

    if ( spGetInput()->axis[1] == 1 )
	{
        rotation += 100*dt;
	}
    else if ( spGetInput()->axis[1] == -1 )
	{
        rotation -= 100*dt;
	}

	if ( spGetInput()->button[SP_BUTTON_R] )
    {
        // randomize
		spGetInput()->button[SP_BUTTON_R] = 0;
    }
	if ( spGetInput()->button[SP_BUTTON_L] )
    {
		spGetInput()->button[SP_BUTTON_L] = 0;
        reset();
    }


    if (!(pause))
    {
        physics.update( dt*1.0/100 );

        hero.update( dt );
        int i=0;
        int blocksize = blocks.size();
        while ( i < blocksize )
        {
            //std::cout << i << "; id " << blocks[i].id << " ";
            blocks[i].update( dt );

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

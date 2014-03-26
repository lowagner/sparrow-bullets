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

Play::Play()
{
    font=NULL;

    //input.push_back("");
    pause = 0;
    iamdone = 0;
    threading = 0;
    play_light = 1;
    play_perspective = 0;

    // initialize the physics land and world drawing class
    // THIS IS UNNECESSARY, since World already created itself in Play variables.
    //world = World();
    world.add_box(10,10,1, 0,0,-1); // add the floor
    hero = Cube(0,0,10);
    hero.add_to_world( world );
}

char* 
Play::caption_play(char* caption)
{
	sprintf(caption,"non-Rotating Cube");
	return caption;
}

char* 
Play::settings_play(char* caption,int button)
{
	switch (button)
	{
		case SP_BUTTON_A:
			if (play_perspective)
				sprintf(caption,"[A] Perspective fix");
			else
				sprintf(caption,"[A] Wobbly texture");
			break;
		case SP_BUTTON_B:
			if (play_light)
				sprintf(caption,"[B] Light on");
			else
				sprintf(caption,"[B] Light off");
			break;
	}
	return caption;
}

void Play::draw( SDL_Surface* screen )
{
    // the first part of this stuff is drawing buttons and what not.
	spSetZSet( 0 );
	spSetZTest( 0 );
	spSetAlphaTest( 1 );
	spFontDraw( 2, font-> maxheight+2, 0, "[L] Previous", font );
	spFontDrawRight( screen->w - 2 , font-> maxheight+2, 0, "[R] next", font );
	spFontDrawRight( screen->w - 2 , 2, 0, "[S] Exit", font );
	switch (spIsKeyboardPolled())
	{
		case 0: spFontDraw( 2, 2, 0, "[E] Enter Text", font ); break;
		case 1: spFontDraw( 2, 2, 0, "[E] Finish Text", font ); break;
	}
	switch (threading)
	{
		case 0: spFontDrawRight( screen->w - 2, screen->h - 1*font-> maxheight, 0, "[X] No draw thread", font ); break;
		case 1: spFontDrawRight( screen->w - 2, screen->h - 1*font-> maxheight, 0, "[X] Extra draw thread", font ); break;
	}
	switch (pause)
	{
		case 0: spFontDrawRight( screen->w - 2, screen->h - 2*font-> maxheight, 0, "[Y] Pause", font ); break;
		case 1: spFontDrawRight( screen->w - 2, screen->h - 2*font-> maxheight, 0, "[Y] Unpause", font ); break;
	}
	//spFontDrawMiddle( screen->w /2, screen->h - 2*font-> maxheight, 0, input, font );

	char buffer[256];
    spFontDrawMiddle( screen->w / 2, font->maxheight + 2, 0, caption_play(buffer), font );
    spFontDraw( 2, screen->h - 1*font->maxheight,0, settings_play(buffer,SP_BUTTON_A), font);
    spFontDraw( 2, screen->h - 2*font->maxheight,0, settings_play(buffer,SP_BUTTON_B), font);
	
    sprintf( buffer, "fps: %i", spGetFPS() );
	spFontDrawMiddle( screen->w/2, 1, 0, buffer, font );
	if (spIsKeyboardPolled() && spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS)
		spBlitSurface(screen->w/2,screen->h-spGetVirtualKeyboard()->h/2,0,spGetVirtualKeyboard());
	spFlip();
	spResetZBuffer();
	spClearTarget( spGetRGB(64,64,64) );
	spIdentity();

	spSetLightPosition(0,spFloatToFixed( 0.875f ),spFloatToFixed( 0.875f ),spFloatToFixed( 0.875f ));
	spSetLightColor(0,SP_ONE,SP_ONE,SP_ONE);
	spSetZSet( 1 );
	spSetZTest( 1 );


    // now we actually draw the scene...
	spSetPerspectiveTextureMapping( play_perspective );
    world.draw();
}


int Play::update( Uint32 dt )
{
    // inputy type things
	if ( spGetInput()->button[SP_BUTTON_A] )
	{
		spGetInput()->button[SP_BUTTON_A] = 0;
		play_perspective = 1-play_perspective;
	}
	if ( spGetInput()->button[SP_BUTTON_B] )
	{
		spGetInput()->button[SP_BUTTON_B] = 0;
		play_light = 1-play_light;
	}

	if ( spGetInput()->button[SP_BUTTON_START] )
        iamdone = 1;
	
	if ( spIsKeyboardPolled())
	{
		if ( spGetInput()->button[SP_BUTTON_SELECT] )
		{
			spGetInput()->button[SP_BUTTON_SELECT] = 0;
			spStopKeyboardInput();
		}
	}
	if ( spGetInput()->button[SP_BUTTON_X] )
	{
		spGetInput()->button[SP_BUTTON_X] = 0;
		threading = (threading+1) % 2;
		spDrawInExtraThread(threading);
	}
	if ( spGetInput()->button[SP_BUTTON_Y] )
	{
		spGetInput()->button[SP_BUTTON_Y] = 0;
		pause = 1-pause;
	}
	if ( spGetInput()->button[SP_BUTTON_R] )
	{
		spGetInput()->button[SP_BUTTON_R] = 0;
	}
	if ( spGetInput()->button[SP_BUTTON_L] )
	{
		spGetInput()->button[SP_BUTTON_L] = 0;
	}

	if ( spGetInput()->button[SP_BUTTON_SELECT] )
	{
		spGetInput()->button[SP_BUTTON_SELECT] = 0;
		//spPollKeyboardInput(input,32,SP_BUTTON_A_MASK | SP_BUTTON_B_MASK);
	}

    // updatey type things
    //std::cout << " dt = " << dt << std::endl;
    if (!(pause))
        world.update( 1.0*dt/100 );

    btVector3 heropos;
    btQuaternion herorot;
    hero.get_position_orientation( heropos, herorot );
    std::cout << " hero z = " << heropos.z() << std::endl;

    // return a value
    if (iamdone)
        return GAMESTATEquit; // return the quit gamestate to quit
    else
        return GAMESTATEplay; // always return your own value if you want to continue updating
}

void Play::resize( Uint16 w, Uint16 h )
{
	spSelectRenderTarget(spGetWindowSurface());
	//Settings up the onboard keyboard:
	if (spGetSizeFactor() <= SP_ONE)
		spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_IF_NEEDED,
                0,h-w*48/320,w,w*48/320,
                spLoadSurface("./data/keyboard320.png"),
                spLoadSurface("./data/keyboardShift320.png"));
	else
	if (spGetSizeFactor() <= 2*SP_ONE)
		spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_IF_NEEDED,
                0,h-w*48/320,w,w*48/320,
                spLoadSurface("./data/keyboard640.png"),
                spLoadSurface("./data/keyboardShift640.png"));
	else
		spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_IF_NEEDED,
                0,h-w*48/320,w,w*48/320,
                spLoadSurface("./data/keyboard1280.png"),
                spLoadSurface("./data/keyboardShift1280.png"));
		
	//Setup of the new/resized window
	spSetPerspective( 45.0, ( float )spGetWindowSurface()->w / ( float )spGetWindowSurface()->h, 1.0f, 100.0f );

	//Font Loading
	spFontShadeButtons(1);
	if ( font )
		spFontDelete( font );
	font = spFontLoad( "./font/Play-Bold.ttf", spFixedToInt(10 * spGetSizeFactor()));
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
    delete font;
	//spDeleteSurface(play_texture);
	//spDeleteSurface(play_check);
}

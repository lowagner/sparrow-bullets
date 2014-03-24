/*
 The contents of this file are subject to the "do whatever you like"-license.
 That means: Do, whatver you want, this file is under public domain. It is an
 example for sparrow3d. Copy it and learn from it for your project and release
 it under every license you want. ;-)
 For feedback and questions about my Files and Projects please mail me,
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com
*/
#include "play.h"
#include <SDL_image.h>
#include <math.h>
#include <string.h>
//#include "btBulletDynamicsCommon.h"
//#include "LinearMath/btAabbUtil2.h"


// various gamestates to be in
int GAMESTATEquit = 0; // quit asap
int GAMESTATEplay = 1; // play

// NOT IMPLEMENTED YET
int GAMESTATEsplash = 2; // quit asap
int GAMESTATEmenu = 3; // play

GameChunk* gamechunk;  // pointer to the currently active game chunk
SDL_Surface* screen;   // pointer to the screen.
int gamestate;   // tells you to change gamechunks

void resize( Uint16 w, Uint16 h )
{
    gamechunk->resize( w, h );
}
void draw( void )
{
    gamechunk->draw( screen );
}
void handle( SDL_Event* event )
{
    gamechunk->handle( event );
}

void init()
{
	//sparrow3D Init
	//spSetDefaultWindowSize( 800, 480 ); //Creates a 800x480 window at PC instead of 320x240
	spInitCore();

    gamechunk = new Play();

	//Setup
	screen = spCreateDefaultWindow();
	spUsePrecalculatedNormals(0);
	resize( screen->w, screen->h );
	
	spSelectRenderTarget(screen);

	spSetAffineTextureHack(0);
	//spSetLight(1);
}


int update( Uint32 dt )
{
    gamestate = gamechunk->update( dt );

    if (gamestate == GAMESTATEquit)
        return 1;
    else
        return 0;
}




void exit() // destructor
{
    // delete the gamechunk
    delete gamechunk;
    // this is where the gamechunk destructor method ~GameChunk is called.

	//Winter Wrap up, Winter Wrap up 
	spQuitCore();
}


int main( int argc, char **argv )
{
    init();
    spLoop( draw, update, 10, resize, handle );
    exit();
	return 0;
}








//int main( int argc, char **argv )
//{
//    
//    game = Game();
//
//
//	//Main loop
//    // calls draw methods, update methods given a time lapse,
//    // attempts to get a certain fps, has a method for resize, 
//    // and allows handling all SDL events with another method...
//	spLoop( game.draw, game.update, 10, game.resize, game.handle ); // 10 means attempt to get 100 fps.
//	
//	//Winter Wrap up, Winter Wrap up 
//	spFontDelete( font );
//	quit_play();
//	spQuitCore();
//
//
//
//
//
//
//	return 0;
//}

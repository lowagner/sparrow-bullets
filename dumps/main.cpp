/*
Check the LICENSE file included for copyright information.  
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
int GAMESTATEsplash = 2; // splash screen (first to see)
int GAMESTATEmenu = 3; // main menu

GameChunk* gamechunk;  // pointer to the currently active game chunk
SDL_Surface* screen;   // pointer to the screen.
int gamestate;   // which gamestate you are in; can tell you to change gamechunks

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
    //Setup
    screen = spCreateDefaultWindow();

    // set gamestate and gamechunk to the right variables...
    gamestate = GAMESTATEplay;
    gamechunk = new LowLevels();

    spUsePrecalculatedNormals(0);
    resize( screen->w, screen->h );
    spSelectRenderTarget(screen);

    spSetAffineTextureHack(0);
}


int update( Uint32 dt )
{
    int newgamestate = gamechunk->update( dt );

    if (newgamestate == GAMESTATEquit)
    {
        return 1;
    }
    else
    if (newgamestate != gamestate)
    {
        // this shouldn't actually return 1 here.
        return 1;
        // instead, it should actually kill the previous gamechunk:
        // delete gamechunk;
        // and move to the new gamechunk
        // gamechunk = new GameChunk
        // based on the newgamestate.
        gamestate = newgamestate;
    }
    return 0;
}




void exit() // destructor
{
    // delete the gamechunk
    delete gamechunk;
    // this is where the gamechunk destructor method ~GameChunk is called.

    spQuitCore();
}


int main( int argc, char **argv )
{
    init();
    spLoop( draw, update, 10, resize, handle ); 
                        // 20 means max FPS is 50
                        // 10 means max FPS is 100
    exit();
    return 0;
}

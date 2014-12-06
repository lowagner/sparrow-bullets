/*
Check the LICENSE file included for copyright information.  
*/
#include "splash.h"
#include "play.h"
#include "sounds.h"
#include <SDL_image.h>
#include <math.h>
#include <string.h>
//#include "btBulletDynamicsCommon.h"
//#include "LinearMath/btAabbUtil2.h"


// various gamestates to be in
int GAMESTATEquit = 0; // quit asap
int GAMESTATEmenu = 1; // main menu
int GAMESTATElovels = 2; // low levels

spSound* misssound = NULL; 
spSound* jumpsound = NULL; 
spSound* kicksound = NULL; 
spSound* gotsound = NULL;  
spSound* deathsound = NULL;



// NOT IMPLEMENTED YET
int GAMESTATEsplash = -1; // splash screen (first to see), also win/lose screen

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
    //
    if ( spSoundInit() == 0 )
    {
        std::cerr << " error starting sound " << std::endl;
    }
    spSoundSetChannels(32);
    spSoundSetVolume(SP_VOLUME_MAX/2);
    spSoundSetMusicVolume(SP_VOLUME_MAX/2);

    misssound = spSoundLoad("../sounds/misssound.wav");
    jumpsound = spSoundLoad("../sounds/jumpsound.wav");    
    kicksound = spSoundLoad("../sounds/kicksound.wav");  
    gotsound = spSoundLoad("../sounds/gotsound.wav");    
    deathsound = spSoundLoad("../sounds/deathsound.wav");    

    screen = spCreateDefaultWindow();

    // set gamestate and gamechunk to the right variables...
    gamestate = GAMESTATEsplash;
    gamechunk = //new LowLevels(2,12); 
                new Splash(2,0); // will default to level 0 of levelset 2
                                 // NOTE:  change when there are more levelsets available.

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
    else if (newgamestate != gamestate)
    {
        // we can choose a level and a levelset...
        int level=gamechunk->level;
        int levelset=gamechunk->levelset;
        char message[64]; // is there a message from the previous gamestate?
        if ( gamestate == GAMESTATEsplash )
            sprintf( message, "" ); // don't let splash continue to harrass you
        else
            sprintf( message, "%s", gamechunk->message );

        if ( newgamestate == GAMESTATEsplash )
        {
            std::cout << " initiating splash"  << std::endl;
            int wonvalue = -1;
            if ( gamechunk->won )
                wonvalue = 1;
            delete gamechunk;
            gamechunk = new Splash( levelset, level, wonvalue, message ); 
        }
        else if ( newgamestate == GAMESTATEmenu )
        {
            std::cout << " initiating main menu" << std::endl;
            delete gamechunk;
            gamechunk = new MainMenu( levelset, level, message ); 
        }
        else if ( newgamestate == GAMESTATElovels )
        {
            std::cout << "initiating low levels at level " << level << std::endl;
            delete gamechunk;
            gamechunk = new LowLevels( levelset, level, message ); 
        }
        else
        {
            std::cout << "unknown gamestate / level set " << newgamestate << std::endl;
            return 1;
        }

        gamestate = newgamestate;
    }
    return 0;
}




void exit() // destructor
{
    // delete the gamechunk
    delete gamechunk;
    // this is where the gamechunk destructor method ~GameChunk is called.

    //spSoundStopMusic(0);
    spSoundDelete(jumpsound);
    spSoundDelete(misssound);
    spSoundDelete(kicksound);

    spSoundQuit();
    spQuitCore();
}


int main( int argc, char **argv )
{
    init();
    spLoop( draw, update, 20, resize, handle ); 
                        // 20 means max FPS is 50
                        // 10 means max FPS is 100
    exit();
    return 0;
}

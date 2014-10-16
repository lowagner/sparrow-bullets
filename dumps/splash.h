/*
Check the LICENSE file included for copyright information.  
*/
#ifndef __SPLASH__
#define __SPLASH__
#include "meta.h"
#include "../csparrow.h"


class Splash : public GameChunk
{
protected:
    int gamestate;
    int winintrolose;

    float clock;

    spFontPointer font;
    
    void load_font();
    
public:

    Splash( int levelset_ = 1, int level_ = 1, int winintrolose_=0, char* message_ = "" );

    void handle( SDL_Event* event );
    int update( Uint32 dt );
    void draw( SDL_Surface* screen );
    void resize( Uint16 w, Uint16 h );

    ~Splash();
};


#endif

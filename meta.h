#ifndef __META__
#define __META__

#include "csparrow.h"

// gamestates, defined in main.c
extern int GAMESTATEquit; // quit asap
extern int GAMESTATEplay; // playing mode

// not implemented yet
extern int GAMESTATEsplash; // loading
extern int GAMESTATEmenu; // menu

// base class of gamechunks which are indexed by the above
class GameChunk
{
public:
    GameChunk() {};
    virtual int update( Uint32 dt ) {}; // dt measured in milliseconds
    virtual void resize( Uint16 w, Uint16 h ) {};
    virtual void handle( SDL_Event* event ) {};
    virtual void draw( SDL_Surface* screen ) {};    

    // code currently segfaults with this, but only at the end.
    // i think it's important, but only time will tell.
    virtual ~GameChunk() {};
};

#endif

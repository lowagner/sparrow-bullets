#ifndef __META__
#define __META__

#include "csparrow.h"

class GameChunk
{
public:
    GameChunk() {};
    virtual int update( Uint32 dt ) {}; // dt measured in milliseconds
    virtual void resize( Uint16 w, Uint16 h ) {};
    virtual void handle( SDL_Event* event ) {};
    virtual void draw( SDL_Surface* screen ) {};    
};

#endif

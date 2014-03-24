#ifndef __META__
#define __META__

#ifdef __cplusplus
extern "C"
{
#endif
#include <sparrow3d.h>
#ifdef __cplusplus
}
#endif


class GameChunk
{
public:
    GameChunk() {};
    virtual int update( Uint32 dt ) {}; // dt measured in milliseconds
    virtual void resize( Uint16 w, Uint16 h ) {};
    virtual void handle( SDL_Event* event ) {};
    virtual void draw( SDL_Surface* screen ) {};    
};



//
//class Game
//{
//public:
//// various variables...
//
//// various methods.
//    Game(); // constructor
//
//    int update( Uint32 dt ); // dt measured in milliseconds
//    // define update in main.cpp
//
//
//    ~Game(); // destructor
//};

#endif

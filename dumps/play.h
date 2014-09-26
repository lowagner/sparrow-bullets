/*
Check the LICENSE file included for copyright information.  
*/
#ifndef __PLAY__
#define __PLAY__

#include "physics.h"
#include "objects.h"
#include "shapes.h"
#include "meta.h"
#include <vector>
//#include <time.h>  // for timer functionality
//#include <unistd.h> // for sleep function
//#include <string>
//using std::string;


class Play : public GameChunk
{
protected:
    SDL_Surface* checkertexture;

    Sint32 cameraincline;
    Sint32 cameraaxis;
    Sint32 cameradistance;
    btVector3 cameracenter;
    bool camerafollowhero, cameraalignhero;
    float cameracooldown, cameraaligndelta, camerafollowdelta;

    btVector3 outofbounds;
    btScalar clock;
    btScalar totalclock;

    spFontPointer font;

    Player hero;
    float winlevel; // 0 until we won the level, then jumps to some preset time
    int lives; // number of lives left

    std::vector<Cube> blocks; // dynamic blocks
    std::vector<Box> boxes; // static boxes
    bool killboxfromblockid;
    std::vector<Ramp> ramps; // static ramps

    int pause;
    int menu; // whether to show the menu, and which item it is displaying
    std::vector<char*> menuitems;

    int no_movement;
    //std::string input;
    char input[32];

    int iamdone;
    int level;
    Physics physics;
    
//    time_t previous_t;
//    time_t current_t;
    
    
public:

    Play( int level_ = 1 );
    int reset();
    void draw( SDL_Surface* screen );
    void load_font();
    int update( Uint32 dt );
    void resize( Uint16 w, Uint16 h );
    void handle( SDL_Event* event );
    void deinit();
    ~Play();
};




#endif

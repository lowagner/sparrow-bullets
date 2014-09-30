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
    int gamestate;

    SDL_Surface* checkertexture;

    Sint32 cameraincline;
    Sint32 cameraaxis;
    Sint32 cameradistance;
    btVector3 cameracenter;
    float cameramovecooldown, cameraalignspeed, camerafollowspeed;

    btVector3 outofbounds;
    btScalar clock;
    btScalar totalclock;

    spFontPointer font;

    Player hero;
    btScalar winlevel; // 0 until we won the level, then jumps to some preset time
    char wintext[60]; // 
    char lvltext[60];
    int lives; // number of lives left

    std::vector<Cube> blocks; // dynamic blocks
    std::vector<Box> boxes; // static boxes
    bool killboxfromblockid;
    std::vector<Ramp> ramps; // static ramps

    int pause;
    int menu; // whether to show the menu, and which item it is displaying
    std::vector<char*> menuitems;
    std::vector< std::vector<float> > menuitemvalues;
    std::vector<int> menuitemvalueindices;

    int no_movement;
    //std::string input;
    char input[32];

    int iamdone;
    int level;
    Physics physics;
    
//    time_t previous_t;
//    time_t current_t;
    
    void load_font();
    
public:

    Play( int level_ = 1 );

    virtual int reset();
    virtual int update( Uint32 dt );
    virtual int update_hero( btScalar fdt );
    virtual int update_level( btScalar fdt );
    virtual void handle( SDL_Event* event );

    void draw( SDL_Surface* screen );
    void resize( Uint16 w, Uint16 h );
    int set_value( const char* name, float value);
    void write_settings();
    void deinit();

    ~Play();
};


class MainMenu : public Play
{
protected:
    int levelset;
public:
    MainMenu( int level_ = 1 );
    int reset();
    int update_level( btScalar fdt );
};


class LowLevels : public Play
{
public:
    LowLevels( int level_ = 1 );
    int reset();
};


#endif

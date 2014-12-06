/*
Check the LICENSE file included for copyright information.  
*/
#ifndef __PLAY__
#define __PLAY__

#include "physics.h"
#include "objects.h"
#include "shapes.h"
#include "meta.h"
#include "sounds.h"
#include <vector>
//#include <time.h>  // for timer functionality
//#include <unistd.h> // for sleep function
//#include <string>
//using std::string;


class Play : public GameChunk
{
protected:
    int gamestate;
    bool alive;

    SDL_Surface* checkertexture;
    std::vector<SDL_Surface*> numbertexture;

    Sint32 cameraincline;
    Sint32 cameraaxis;
    Sint32 cameradistance;
    btVector3 cameracenter;
    float cameraalignspeed, camerafollowspeed, cameratimeout;
    float cameramovecooldown;

    btVector3 outofbounds;
    btScalar clock;
    btScalar totalclock;

    spFontPointer font;

    char lvltext[64]; // level name
    char alerttext[64]; // if there are any alerts to be broadcast
    btScalar alerttime; // how long left to broadcast

    Player hero;
    int lives; // number of lives left

    btScalar winlevel; // 0 until we won the level, then jumps to some preset time
    char wintext[60]; // 


    std::vector<Cube> blocks; // dynamic blocks
    std::vector<Box> boxes; // static boxes

    bool killboxfromblockid, addboxfromblockid;
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
    Physics physics;
    
//    time_t previous_t;
//    time_t current_t;
    
    void load_font();

    int save_time_if_best();
    
public:

    Play( int levelset_ = 1, int level_ = 1, char* message_ = "" );

    virtual int reset();
    virtual int update( Uint32 dt );
    virtual int update_hero( btScalar fdt );
    virtual int update_level( btScalar fdt );
    virtual void handle( SDL_Event* event );

    void draw( SDL_Surface* screen );
    void resize( Uint16 w, Uint16 h );
    void standard_physics_init();
    int set_value( const char* name, float value);
    void write_settings();
    void deinit();
    virtual void set_camera_incline();

    void set_alert( const char* alert, btScalar deltat = 3 );

    ~Play();
};


class MainMenu : public Play
{
public:
    MainMenu( int levelset_ = 1, int level_ = 1, char* message_ = "" );
    int reset();
    int update_level( btScalar fdt );
};


class LowLevels : public Play
{
public:
    LowLevels( int levelset_ = 2, int level_ = 1, char* message_ = "" );
    int reset();
};

class Soccer : public Play
{
protected:
    btScalar halfdistancetogoal, halfgoalwidth;
public:
    int points, antipoints;
    Soccer( int levelset_ = 999, int level_ = 1, char* message_ = "" );
    int reset();
    int update_level( btScalar fdt );
};


#endif

/*
 The contents of this file are subject to the "do whatever you like"-license.
 That means: Do, whatver you want, this file is under public domain. It is an
 example for sparrow3d. Copy it and learn from it for your project and release
 it under every license you want. ;-)
 For feedback and questions about my Files and Projects please mail me,
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com
*/
#ifndef __PLAY__
#define __PLAY__

#include "physics.h"
#include "objects.h"
#include "meta.h"
#include <vector>
//#include <string>
//using std::string;


class Play : public GameChunk
{
private:
    SDL_Surface* checkertexture;

    Sint32 axis;
    Sint32 rotation;
    Sint32 distance;

    spFontPointer font;

    Cube hero;
    std::vector<Cube> blocks; // dynamic blocks
    std::vector<Box> boxes; // static boxes
    std::vector<Ramp> ramps; // static ramps

    int pause;
    int no_movement;
    //std::string input;
    char input[32];

    char* settings_play(char* caption,int button);
    char* caption_play(char* caption);
    
    int iamdone;
    Physics physics;
    
    
public:

    Play();
    void reset();
    void draw( SDL_Surface* screen );
    int update( Uint32 dt );
    void resize( Uint16 w, Uint16 h );
    void handle( SDL_Event* event );

    ~Play();
};




#endif

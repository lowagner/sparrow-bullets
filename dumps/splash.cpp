#include "splash.h"
#include <iostream>

Splash::Splash( int level_, int levelset_, int winintrolose_ )
{
    level = level_;
    levelset = levelset_;
    winintrolose = winintrolose_;
    clock = 3.f;
    font = NULL;
    won = false;
}

void Splash::draw( SDL_Surface* screen )
{
    spClearTarget( spGetRGB(64,64,64) ); // set background color
    if (!(font))
        load_font();

    // the first part of this stuff is drawing buttons and what not.
    spSetZSet( 0 );
    spSetZTest( 0 );
    spSetAlphaTest( 1 );

    if ( winintrolose == 1)
    {
        // 
        spFontDrawMiddle( screen->w /2, screen->h/2, 0, "you have done it!", font );
    }
    else if ( winintrolose == -1 )
    { 
        // losing screen
        spFontDrawMiddle( screen->w /2, screen->h/2, 0, "you lost!", font );
    }
    else 
    {
        // intro splash screen
        spFontDrawMiddle( screen->w /2, screen->h/2, 0, "the intro screen!", font );
    }

    spFontDrawMiddle( screen->w / 2, font->maxheight + 2, 0, "cube dump", font );
    
//    char buffer[64];
//    sprintf( buffer, "fps: %i", spGetFPS() );
//    spFontDrawMiddle( screen->w/2, 1, 0, buffer, font ); 

    // this displays everything. (??)
    spFlip(); 
}


int Splash::update( Uint32 dt )
{
    if ( spGetInput()->button[SP_BUTTON_START] || spGetInput()->button[SP_BUTTON_SELECT] )
    {
        spGetInput()->button[SP_BUTTON_START] = 0;
        spGetInput()->button[SP_BUTTON_SELECT] = 0;
        return GAMESTATEmenu;
    }
    clock -= dt*1.0/1000;
    
    // return a value
    if ( clock > 0.f )
        return GAMESTATEsplash; // always return your own value if you want to continue updating
    else
        return GAMESTATEmenu;  
}



void Splash::resize( Uint16 w, Uint16 h )
{
    spSelectRenderTarget(spGetWindowSurface());
    //Setup of the new/resized window
    spSetPerspective( 30.0,  // kinda like the zoom factor
                      ( float )spGetWindowSurface()->w / ( float )spGetWindowSurface()->h, 
                      0.5f, 100.0f ); // z near and z far
    //spSetPerspective( 45.0, ( float )spGetWindowSurface()->w / ( float )spGetWindowSurface()->h, 
    //                  1.0f, 100.0f );

    //Font Loading
    if ( font )
        spFontDelete( font );

    load_font();
}

void Splash::load_font()
{
    font = spFontLoad( "../font/Play-Bold.ttf", spFixedToInt(20 * spGetSizeFactor()));
    spFontSetShadeColor(0);
    spFontAdd( font, SP_FONT_GROUP_ASCII, 65535 ); //whole ASCII
    spFontAdd( font, "äüöÄÜÖßẞ", 65535 ); //German stuff (same like spFontAdd( font, SP_FONT_GROUP_GERMAN, 0 ); )
    spFontAddBorder( font, 0 );
    spFontSetButtonStrategy(SP_FONT_BUTTON);
    spFontAddButton( font, 'A', SP_BUTTON_A_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'B', SP_BUTTON_B_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'X', SP_BUTTON_X_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'Y', SP_BUTTON_Y_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'L', SP_BUTTON_L_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'R', SP_BUTTON_R_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontSetButtonStrategy(SP_FONT_INTELLIGENT);
    spFontAddButton( font, 'S', SP_BUTTON_START_NAME, 65535, spGetRGB( 64, 64, 64 ) );
    spFontAddButton( font, 'E', SP_BUTTON_SELECT_NAME, 65535, spGetRGB( 64, 64, 64 ) );
}


void Splash::handle( SDL_Event* event )
{
    // get all SDL events and do something with them...  if you want...
//    if (event->type == SDL_KEYDOWN)
//    {
//        int lastKey = event->key.keysym.unicode;
//        char buffer[5];
//        printf("keydown event 0x%x = \"%s\" keysym=%i\n",lastKey,spFontGetUTF8FromUnicode(lastKey,buffer,5),event->key.keysym.sym);
//    }
}



Splash::~Splash()
{
    std::cout << " exiting splash " << std::endl;
    if ( font )
        spFontDelete( font );
}

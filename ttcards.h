/******************************************************************************
    ttcards.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef TTCARDS_HEADERS
#define TTCARDS_HEADERS

#include <iostream>

#include "SDL.h"

#include "cfg.h"
#include "audio.h"
#include "player.h"
#include "board.h"
#include "card.h"
#include "font.h"
//#include "input.h"
#include "sprite.h"
//#include "gfx.h"

class TTcards
{
public:
    TTcards();
    ~TTcards ( void );

    bool Run ( void );
    bool IsRunning ( void );
    void SetGameState ( bool state );

    bool Init ( void );
    void Input ( void );

private:
    bool game_state; // global app state

    SDL_Surface *screen = NULL; // primary video buffer
    SDL_Surface *background = NULL;
    SDL_Surface *card_buffer = NULL;

    Audio mixer;
    Font txt;
    Font score;
};

#endif // TTCARDS_HEADERS defined

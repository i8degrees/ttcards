/******************************************************************************
    ttcards.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef TTCARDS_HEADERS
#define TTCARDS_HEADERS

#include <iostream>

#include "SDL/SDL.h"

#include "cfg.h"
#include "gfx.h"
#include "sprite.h"
#include "audio.h"
#include "board.h"
#include "player.h"
#include "card.h"
#include "font.h"

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
    SDL_Surface *screen; // primary video buffer
    bool game_state; // global app state

    Gfx gfx;
    Font txt;

    Audio music;
    Audio mixer1, mixer2; // Two (2) audio mixing channels for playing sound effects
    Player player1, player2;
};

#endif // TTCARDS_HEADERS defined

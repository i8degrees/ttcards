/******************************************************************************
    ttcards.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef TTCARDS_HEADERS
#define TTCARDS_HEADERS

#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"

#include "cfg.h"
//#include "audio.h"
#include "board.h"
#include "card.h"
//#include "input.h"
#include "sprite.h"
//#include "gfx.h"

class TTcards
{
public:
    TTcards();
    bool Run ( void );
    bool IsRunning ( void );
    void SetGameState ( bool state );

    bool Init ( void );
    void Input ( void );
    bool CleanUp ( void );

private:
    bool game_state; // global app state

    SDL_Surface *screen = NULL; // primary video buffer
    SDL_Surface *background = NULL;
    SDL_Surface *p1_txtNorth = { 0 };
    SDL_Surface *p1_txtEast = { 0 };
    SDL_Surface *p1_txtSouth = { 0 };
    SDL_Surface *p1_txtWest = { 0 };

    SDL_Surface *p2_txtNorth = { 0 };
    SDL_Surface *p2_txtEast = { 0 };
    SDL_Surface *p2_txtSouth = { 0 };
    SDL_Surface *p2_txtWest = { 0 };

    TTF_Font *font = NULL;
};

#endif // TTCARDS_HEADERS

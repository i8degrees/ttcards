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
#include "card_collection.h"
#include "font.h"
#include "timer.h"

class TTcards
{
  public:
    TTcards ( void );
    ~TTcards ( void );

    bool Init ( void );

    bool Run ( void );
    bool IsRunning ( void );
    void SetGameState ( bool state );

    void ShowFPS ( void );
    void InterfaceInput ( SDL_Event &event );
    void Input ( void );

  private:
    SDL_Event input;

    Audio music;
    Board board;
    CardDebug debug;
    CardView cards;
    Collection collection;
    Gfx engine;
    Player player1, player2;

    FPS fps;
    Font timer_text;
    bool show_fps; // fps counter
    bool game_state; // global app state
};

#endif // TTCARDS_HEADERS defined

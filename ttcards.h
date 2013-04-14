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
#include "fps.h"

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

    bool LoadBackground ( std::string filename );
    bool DrawBackground ( void );

  private:
    SDL_Surface *background;
    SDL_Event input;

    CardDebug debug;
    Audio music;

    Collection collection;
    Gfx engine;
    Player player1, player2;

    FPS fps;
    Font timer_text;

    bool show_fps; // fps counter
    bool game_state; // global app state
};

#endif // TTCARDS_HEADERS defined

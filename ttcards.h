/******************************************************************************
    ttcards.h

  Final Fantasy VIII Triple Triad Remake

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_TTCARDS_HEADERS
#define GAMEAPP_TTCARDS_HEADERS

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

    bool IsRunning ( void );
    void SetGameState ( bool state );

    void ShowFPS ( void );
    void Input ( void );
    void InterfaceInput ( SDL_Event &event );
    bool LoadGameData ( void );
    bool Run ( void );

  private:
    SDL_Event input;
    FPS fps;
    Gfx engine;
    Font timer_text;
    Board board;
    Collection collection;
    Player player1, player2;
    Audio music;
    CardDebug debug;

    bool show_fps; // fps counter
    bool game_state; // global app state
};

#endif // GAMEAPP_TTCARDS_HEADERS defined

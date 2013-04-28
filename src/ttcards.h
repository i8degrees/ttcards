/******************************************************************************
    ttcards.h

    Final Fantasy VIII Triple Triad Remake

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_TTCARDS_HEADERS
#define GAMEAPP_TTCARDS_HEADERS

#include "gamelib/gamelib.h"

#include "cfg.h"
#include "board.h"
#include "player.h"
#include "card.h"
#include "card_collection.h"

class TTcards
{
  public:
    TTcards ( void );
    ~TTcards ( void );

    bool Init ( Gfx &engine );

    bool IsRunning ( void );
    void SetGameState ( bool state );
    void ShowFPS ( void );
    bool IsFullScreen ( void );

    void Input ( void );
    void InterfaceInput ( unsigned int type, SDLKey key, SDLMod mod );
    bool LoadGameData ( void );
    void Run ( void );

  private:
    SDL_Event input;
    FPS fps;
    Gfx *engine; // Pointer reference to our rendering interface; we ought not have more than one Gfx object instance at any given time
    Font timer_text;
    Board board;
    Collection collection;
    CardHand player1_hand, player2_hand;
    Player *player1, *player2;
    Audio music;
    CardDebug debug;

    bool game_state; // global app state
    bool show_fps; // fps counter
    bool fullscreen; // toggle window & fullscreen states
};

#endif // GAMEAPP_TTCARDS_HEADERS defined
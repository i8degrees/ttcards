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

    bool Init ( Gfx *engine );

    bool IsRunning ( void );
    void SetGameState ( bool state );
    void ShowFPS ( void );
    bool IsFullScreen ( void );

    unsigned int get_turn ( void );
    void player_turn ( unsigned int player );

    void Input ( void );
    void InterfaceInput ( unsigned int type, SDLKey key, SDLMod mod );

    void debug_input ( unsigned int type, SDLKey key, SDLMod mod );
    void board_input ( unsigned int type, SDLKey key, SDLMod mod );
    void cursor_input ( unsigned int type, SDLKey key, SDLMod mod );
    void mouse_input ( unsigned int type, SDL_MouseButtonEvent button );

    void check_cursor_movement ( void );
    void draw_cursor ( void );
    void update_cursor ( void );

    bool LoadGameData ( void );
    void Run ( void );

  private:
    SDL_Event input;
    FPS fps;
    Gfx *engine; // Pointer reference to our rendering interface; we ought not have more than one Gfx object instance at any given time
    Font timer_text;
    Board board;
    CardDebug debug;
    Collection collection;
    CardHand hand[2]; //CardHand player1_hand, player2_hand;
    Player player[2]; //Player player1, player2;
    CardView card; // card rendering
    Sprite cursor; // interface cursor
    Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
    Audio music; // holds our musical tracks

    unsigned int turn; // player1 = 0, player2 = 1
    bool game_state; // global app state
    bool show_fps; // fps counter
    bool fullscreen; // toggle window & fullscreen states
};

#endif // GAMEAPP_TTCARDS_HEADERS defined

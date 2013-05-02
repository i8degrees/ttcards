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
#include "card_rules.h"
#include "cpu_player.h"

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

    void moveTo ( unsigned int x, unsigned int y );

    void Input ( void );
    void InterfaceInput ( unsigned int type, SDLKey key, SDLMod mod );

    void debug_input ( unsigned int type, SDLKey key, SDLMod mod );
    void board_input ( unsigned int type, SDLKey key, SDLMod mod );
    void cursor_input ( unsigned int type, SDLKey key, SDLMod mod );
    void mouse_input ( unsigned int type, SDL_MouseButtonEvent button );

    void check_cursor_movement ( void );
    void draw_cursor ( void );
    void update_cursor ( void );

    void interface_GameOver ( void );

    bool LoadGameData ( void );
    void Run ( void );

  private:
    SDL_Event input; // input events; keyboard, mouse
    FPS fps; // timer for tracking frames per second
    Gfx *engine; // Pointer reference to our rendering interface; we ought not have more than one Gfx object instance at any given time
    Font timer_text; // fps timer font
    Font message_text;
    Board board; // game board
    CardDebug debug; // debug support for card attributes
    Collection collection; // cards database
    CardRules rules;
    CardView card; // card rendering
    CardHand hand[2]; // player hand
    Player player[2]; // player on board
    Sprite cursor; // interface cursor
    Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
    Audio music; // holds our musical tracks

    unsigned int turn; // player1 = 0, player2 = 1
    bool game_state; // global app state
    bool show_fps; // fps counter
    bool fullscreen; // toggle window & fullscreen states
};

#endif // GAMEAPP_TTCARDS_HEADERS defined

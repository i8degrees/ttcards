/******************************************************************************
    ttcards.h

    Final Fantasy VIII Triple Triad Remake

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_TTCARDS_HEADERS
#define GAMEAPP_TTCARDS_HEADERS

#ifdef EMSCRIPTEN
    #include "emscripten.h"
#endif

#include "gamelib.h"

#include "cfg.h"
#include "board.h"
#include "player.h"
#include "card.h"
#include "card_collection.h"
#include "card_rules.h"
#include "cpu_player.h"

#include <map>
#include <algorithm>

class TTcards: public SDLInput
{
  public:
    TTcards ( void );
    ~TTcards ( void );

    bool Init ( Gfx *engine );
    bool LoadGameData ( void );

    bool IsRunning ( void );
    void SetGameState ( bool state );
    void drawFPS ( void );
    bool IsFullScreen ( void );

    unsigned int get_turn ( void );
    void player_turn ( unsigned int player );

    void moveTo ( unsigned int x, unsigned int y );
    void removePlayerCard ( void );

    void Input ( void );
    void cursor_input ( SDL_Event *input );

    void check_cursor_movement ( void );
    void draw_cursor ( void );
    void update_cursor ( void );

    void updateScore ( void );
    void drawScore ( void );
    void interface_GameOver ( void );

    void Update ( void );
    void Draw ( void );
    void Run ( void ); // game loop
    static void Callback ( void ); // EMCC compiler related
    void Start ( void ); // EMCC compiler related

    void onResize ( unsigned int width, unsigned int height );
    void onMouseLeftButtonUp ( unsigned int x, unsigned int y );
    //void onMouseMotion ( unsigned int x, unsigned int y );
    void onMouseWheel ( bool up, bool down );
    void onKeyDown ( SDLKey key, SDLMod mod );
    void onExit ( void );
    void onResize ( void );
    void onMouseLeftButtonDown ( unsigned int x, unsigned int y );

  private:
    static TTcards *instance; // EMCC compiler related
    FPS fps; // timer for tracking frames per second
    Gfx *engine; // Pointer reference to our rendering interface; we ought not have more than one Gfx object instance at any given time
    Font timer_text; // fps timer font
    Font message_text;
    Font score_text; // scoreboard text
    Board board; // game board
    CardDebug debug; // debug support for card attributes
    Collection collection; // cards database
    CardRules rules;
    CPUPlayer AI;
    CardView card; // card rendering
    CardHand hand[2]; // player hand
    Player player[2]; // players in game
    Sprite cursor; // interface cursor
    Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
    Audio music; // holds our musical tracks

    unsigned int turn; // player1 = 0, player2 = 1
    bool game_state; // global app state
    bool show_fps; // fps counter
    bool fullscreen; // toggle window & fullscreen states
};

#endif // GAMEAPP_TTCARDS_HEADERS defined

/******************************************************************************

    Final Fantasy VIII Triple Triad Remake

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_TTCARDS_HEADERS
#define GAMEAPP_TTCARDS_HEADERS

#ifdef EMSCRIPTEN
  #include "emscripten.h"
#endif


#include <gfx/SDL_Font.hpp>
#include <gfx/SDL_BitmapFont.hpp>
#include <gfx/SDL_Drawable.hpp>
#include <gfx/SDL_Display.hpp>
#include <sys/SDL_Timer.hpp>
#include <sys/GameStates.hpp>
#include <gui/SDL_MessageBox.hpp>
#include <gui/SDL_Cursor.hpp>
#include <nomlib_types.hpp>

#include "board.h"
#include "player.h"
#include "card.h"
#include "card_debug.h"
#include "card_collection.h"
#include "card_rules.h"
//#include "cpu_player.h"
#include "CardsMenuState.h"
#include "GameOverState.h"
#include "cfg.h"

class Game: public nom::IState
{
  public:
    Game ( CardHand player1_hand );
    ~Game ( void );

    bool Init ( void );

    void onInit ( void );
    void onExit ( void );

    void Pause ( void );
    void Resume ( void );

    void Update ( float delta_time );
    void Draw ( void* video_buffer );

    //static void Callback ( void ); // EMCC compiler related
    //void Start ( void ); // EMCC compiler related
  private:
    void onKeyDown ( int32_t key, int32_t mod );
    void onMouseLeftButtonDown ( nom::int32 x, nom::int32 y );
    void onMouseWheel ( bool up, bool down );
    void onJoyButtonDown ( int32_t which, int32_t button );

    // Game_debug.cpp
    void debugListCards ( int32_t mod );
    void debugListCollection ( int32_t mod );
    void debugCardsDiscard ( void );
    void debugCardsNoRuleset ( void );
    void debugCardsSameRuleset ( void );
    void removePlayerCard ( void );
    void debugBox ( void );

    unsigned int get_turn ( void );
    void player_turn ( unsigned int player );
    void endTurn ( void );

    void showCardInfoBox ( void *video_buffer );
    bool isCursorLocked ( void );
    void lockCursor ( bool lock );
    void resetCursor ( void );
    void unlockSelectedCard ( void );
    void lockSelectedCard ( void );
    void moveTo ( unsigned int x, unsigned int y );

    nom::Coords getCursorBoardPos ( nom::int32 x, nom::int32 y );
    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    void updateCursor ( void );
    void drawCursor ( void* video_buffer );

    void updateScore ( void );
    void drawScore ( void* video_buffer );
    //static Game *instance; // EMCC compiler related
    nom::SDL_Display context;   // our public / visible display context handle
    nom::SDL_Canvas background; /// pointer holding our board background image

    nom::Timer update;
    nom::SDL_Font gameOver_text; /// dialog text
    nom::SDL_BitmapFont info_text;
    nom::SDL_BitmapFont info_small_text;

    nom::SDL_MessageBox info_box;
    nom::SDL_MessageBox debug_box;
    nom::SDL_Gradient linear;
    std::vector<nom::Color> msgbox;

    nom::SDL_Font score_text; /// scoreboard text
    Board board; /// game board

    /// debug support for card attributes
    CardDebug debug;

    Collection collection; /// cards database
    CardRules rules; /// TODO
    CardView card; /// card rendering
    CardHand hand[2]; /// player hand
    Player player[2]; /// players in game

    nom::SDL_Cursor cursor; /// interface cursor

    /// x, y coords mapping for player1, player2 cursor starting position
    nom::Coords player_cursor_coords[2];

    /// y coords mapping for cursor -> card position index
    nom::Coords cursor_coords_map[5];

    /// x, y coords for selectedCard from cursor to board placement
    nom::Coords board_coords_map[9];

    //Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
    //Audio music; // holds our musical tracks

    unsigned int turn; /// FIXME: player1 = 0, player2 = 1
    bool cursor_locked; /// locks cursor state to board placement

    /// visual indication of which player's turn
    /// it is
    nom::Rectangle player_rect;
};

#endif // GAMEAPP_TTCARDS_HEADERS defined

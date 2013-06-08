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

#include "cfg.h"
#include "board.h"
#include "player.h"
#include "card.h"
#include "card_collection.h"
#include "card_rules.h"
//#include "cpu_player.h"

#include "Logger.h"
#include "SDLMessageBox.h"
#include "SDL_Font.h"
#include "SDL_BitmapFont.h"
#include "SDL_Cursor.h"
#include "SDL_Drawable.hpp"
//#include "audio.h"
#include "fps.h"

#include "GameState.hpp"
#include "GameStates.hpp"
#include "CardsMenuState.h"
#include "GameOverState.h"
#include "SDL_Display.h"

class TTcards: public GameState
{
  public:
    TTcards ( CardHand player1_hand );
    ~TTcards ( void );

    bool Init ( void );
    void Load ( void );

    void Pause ( void );
    void Resume ( void );

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

    void onKeyDown ( int32_t key, int32_t mod );
    void onMouseLeftButtonDown ( int32_t x, int32_t y );
    void onMouseWheel ( bool up, bool down );
    void onJoyButtonDown ( int32_t which, int32_t button );

    nom::Coords getCursorBoardPos ( unsigned int x, unsigned int y );
    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    void updateCursor ( void );
    void drawCursor ( void* video_buffer );

    void updateScore ( void );
    void drawScore ( void* video_buffer );

    void Update ( void );
    void Draw ( void* video_buffer );

    //static void Callback ( void ); // EMCC compiler related
    //void Start ( void ); // EMCC compiler related
  private:
    //static TTcards *instance; // EMCC compiler related
    SDL_Surface *background; /// pointer holding our board background image

    Timer update;
    nom::SDL_Font gameOver_text; /// dialog text
    nom::SDL_BitmapFont info_text;
    nom::SDL_BitmapFont info_small_text;

    SDLMessageBox info_box;
    SDLMessageBox debug_box;
    SDLGradient linear;
    nom::Color msgbox[8];

    nom::SDL_Font score_text; /// scoreboard text
    Board board; /// game board

    CardDebug debug; /// debug support for card attributes

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

    typedef std::vector<nom::SDL_Drawable*> drawable_t;
    drawable_t drawableRects;
    nom::Rectangle rect;
};

#endif // GAMEAPP_TTCARDS_HEADERS defined

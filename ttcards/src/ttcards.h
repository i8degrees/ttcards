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
#include "SDL_TFont.h"
#include "SDL_BitmapFont.h"
#include "SDL_Cursor.h"
//#include "audio.h"

#include "GameState.h"
#include "CardsMenuState.h"
#include "GameOverState.h"

class TTcards: public GameState
{
  public:
    TTcards ( Gfx *engine, CardHand player1_hand );
    ~TTcards ( void );

    bool Init ( void );
    bool Load ( void );

    void Pause ( void );
    void Resume ( void );

    void debugListCards ( SDLMod mod );
    void debugListCollection ( SDLMod mod );
    void debugCardsDiscard ( void );
    void debugCardsNoRuleset ( void );
    void debugCardsSameRuleset ( void );
    void removePlayerCard ( void );
    void debugBox ( void );

    unsigned int get_turn ( void );
    void player_turn ( unsigned int player );
    void endTurn ( void );

    void showCardInfoBox ( SDL_Surface *video_buffer );
    bool isCursorLocked ( void );
    void lockCursor ( bool lock );
    void resetCursor ( void );
    void unlockSelectedCard ( void );
    void lockSelectedCard ( void );
    void moveTo ( unsigned int x, unsigned int y );

    void onExit ( void );
    void onResize ( unsigned int width, unsigned int height );
    void onKeyDown ( SDLKey key, SDLMod mod );
    void onMouseLeftButtonDown ( unsigned int x, unsigned int y );
    void onMouseWheel ( bool up, bool down );
    void onJoyButtonDown ( unsigned int which, unsigned int button );

    nom::Coords getCursorBoardPos ( unsigned int x, unsigned int y );
    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    void updateCursor ( void );
    void drawCursor ( SDL_Surface *video_buffer );

    void updateScore ( void );
    void drawScore ( SDL_Surface *video_buffer );

    void HandleInput ( void );
    void Update ( void );
    void Draw ( SDL_Surface *video_buffer );

    //static void Callback ( void ); // EMCC compiler related
    //void Start ( void ); // EMCC compiler related

  private:
    //static TTcards *instance; // EMCC compiler related
    Gfx *engine; // Pointer reference to our rendering interface; we ought not have more than one Gfx object instance at any given time
    SDL_Surface *background; // pointer holding our board background image

    Timer update;
    SDL_TFont gameOver_text; // dialog text
    SDLBitmapFont info_text;
    SDLBitmapFont info_small_text;

    SDLMessageBox info_box;
    SDLMessageBox debug_box;
    SDLGradient linear;
    nom::Color msgbox[8];

    SDL_TFont score_text; // scoreboard text
    Board board; // game board

    CardDebug debug; // debug support for card attributes

    Collection collection; // cards database
    CardRules rules; // TODO
    CardView card; // card rendering
    CardHand hand[2]; // player hand
    Player player[2]; // players in game

    nom::SDL_Cursor cursor; // interface cursor

    // x, y coords mapping for player1, player2 cursor starting position
    nom::Coords player_cursor_coords[2];

    // y coords mapping for cursor -> card position index
    nom::Coords cursor_coords_map[5];

    // x, y coords for selectedCard from cursor to board placement
    nom::Coords board_coords_map[9];

    //Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
    //Audio music; // holds our musical tracks

    unsigned int turn; // FIXME: player1 = 0, player2 = 1
    bool cursor_locked; // locks cursor state to board placement
};

#endif // GAMEAPP_TTCARDS_HEADERS defined
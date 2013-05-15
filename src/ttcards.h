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

class TTcards: public SDLInput
{
  public:
    TTcards ( void );
    ~TTcards ( void );

    bool Init ( Gfx *engine );
    bool LoadGameData ( void );

    void debugListCards ( SDLMod mod );
    void debugListCollection ( SDLMod mod );
    void debugCardsDiscard ( void );
    void debugCardsNoRuleset ( void );
    void debugCardsSameRuleset ( void );
    void removePlayerCard ( void );
    void showCardID ( void );
    void debugBox ( void );
    void menuBox ( void );
    void reloadDebugFile ( void );

    bool IsFullScreen ( void );
    void setFullscreen ( bool fs );
    bool IsRunning ( void );
    void setGameState ( bool state );
    bool getShowFPS ( void );
    void showFPS ( bool show );
    void toggleFPS ( void );
    void drawFPS ( void );

    unsigned int get_turn ( void );
    void player_turn ( unsigned int player );
    void endTurn ( void );

    bool isCursorLocked ( void );
    void lockCursor ( bool lock );
    void resetCursor ( void );
    void unlockSelectedCard ( void );
    void lockSelectedCard ( void );
    void moveTo ( unsigned int x, unsigned int y );

    void onExit ( void );
    void onResize ( unsigned int width, unsigned int height );
    void Input ( void );
    void onKeyDown ( SDLKey key, SDLMod mod );
    void onMouseLeftButtonDown ( unsigned int x, unsigned int y );
    void onMouseWheel ( bool up, bool down );
    void onJoyButtonDown ( unsigned int which, unsigned int button );

    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    void updateCursor ( void );
    void drawCursor ( void );

    void interface_pickOutCards ( void );
    void interface_playingCards ( void );
    void interface_gameOver ( void );

    void updateScore ( void );
    void drawScore ( void );

    void Update ( void );
    void Draw ( void );
    void Run ( void ); // game loop
    static void Callback ( void ); // EMCC compiler related
    void Start ( void ); // EMCC compiler related

  private:
    static TTcards *instance; // EMCC compiler related
    FPS fps; // timer for tracking frames per second
    Gfx *engine; // Pointer reference to our rendering interface; we ought not have more than one Gfx object instance at any given time

    SDLBitmapFont info_text;
    SDLBitmapFont info_small_text;

    SDLMessageBox menu_box;
    SDLMessageBox info_box;
    SDLMessageBox debug_box;
    SDLGradient linear;
    GColor msgbox[6];

    SDL_TFont message_text; // dialog text
    SDL_TFont score_text; // scoreboard text
    Board board; // game board

    CardDebug debug; // debug support for card attributes
    Logger logDebug;
    std::vector<int> logger;

    Collection collection; // cards database
    CardRules rules;
    CardView card; // card rendering
    CardHand hand[2]; // player hand
    Player player[2]; // players in game

    Sprite cursor; // interface cursor
    Sprite menu_element; // interface menu elements

    unsigned int total_pages;
    unsigned int per_page;
    unsigned int current_index;

    std::pair<int, int> player_cursor_coords[2];
    std::pair<int, int> cursor_coords_map[4]; // coords mapping: cursor XY to card index

    Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
    Audio music; // holds our musical tracks

    unsigned int turn; // player1 = 0, player2 = 1
    bool cursor_locked; // locks cursor state to board placement
    bool game_state; // global app state
    bool show_fps; // fps counter
    bool fullscreen; // toggle window & fullscreen states
};

#endif // GAMEAPP_TTCARDS_HEADERS defined

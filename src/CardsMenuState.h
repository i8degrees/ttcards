/******************************************************************************
    CardsMenuState.h

  Cards Menu state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARDS_MENU_HEADERS
#define GAMEAPP_CARDS_MENU_HEADERS

#include <iostream>
#include <string>

#include "ttcards.h"

#include "SDL_Cursor.h"

class CardsMenu: public GameState
{
  public:
    CardsMenu ( Gfx *engine );
    ~CardsMenu ( void );

    void Pause ( void );
    void Resume ( void );

    void HandleInput ( void );
    void Update ( void );
    void Draw ( SDL_Surface *video_buffer );

  private:
    void onExit ( void );
    void onKeyDown ( SDLKey key, SDLMod mod );
    void onJoyButtonDown ( unsigned int which, unsigned int button );
    void onMouseLeftButtonDown ( unsigned int x, unsigned int y );
    void onMouseRightButtonDown ( unsigned int x, unsigned int y );
    void onMouseWheel ( bool up, bool down );
    void Load ( void );

    void reloadDebugFile ( void );
    void updateCursor ( void );
    void drawCursor ( SDL_Surface *video_buffer );
    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    Gfx *engine; // Pointer reference to our rendering interface
    Collection collection; // cards database
    SDL_Surface *background; // pointer holding our background image

    SDLBitmapFont info_text;
    SDLBitmapFont info_small_text;
    SDLBitmapFont info_text_gray;

    CardDebug debug; // debug support for card attributes
    Logger logDebug;
    std::vector<int> logger;

    SDLMessageBox menu_box;
    SDLGradient linear;
    nom::Color msgbox[8];

    CardView card; // card rendering
    CardHand hand; // player1 hand
    Card selectedCard; // CardHand-derived implementation

    nom::SDL_Cursor cursor;
    nom::Sprite menu_element; // interface menu elements

    unsigned int total_pages; // MAX_COLLECTION / per_page
    unsigned int per_page; // number of cards to display per menu page
    unsigned int current_index; // current card position
    unsigned int info_text_height; // height of the card name text

    // y coords mapping for cursor -> card position index
    std::pair<int, int> cursor_coords_map[10]; // minus one (1) padding
};

#endif // GAMEAPP_CARDS_MENU_HEADERS defined

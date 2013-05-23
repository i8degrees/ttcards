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

class CardsMenu: public GameState
{
  public:
    CardsMenu ( Gfx *engine );
    ~CardsMenu ( void );

    void Pause ( void );
    void Resume ( void );

    void HandleInput ( void );
    void Update ( void );
    void Draw ( void );

  private:
    void onExit ( void );
    void onKeyDown ( SDLKey key, SDLMod mod );
    void onMouseLeftButtonDown ( unsigned int x, unsigned int y );
    void onMouseRightButtonDown ( unsigned int x, unsigned int y );
    void onMouseWheel ( bool up, bool down );
    void Load ( void );

    void reloadDebugFile ( void );
    void updateCursor ( void );
    void drawCursor ( void );
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

    CardDebug debug; // debug support for card attributes
    Logger logDebug;
    std::vector<int> logger;

    SDLMessageBox menu_box;
    SDLGradient linear;
    GColor msgbox[6];

    //CardHand *hand[2]; // player hand
    CardView card; // card rendering

    Sprite cursor; // interface cursor
    Sprite menu_element; // interface menu elements

    unsigned int total_pages; // MAX_COLLECTION / per_page
    unsigned int per_page; // number of cards to display per menu page
    unsigned int current_index; // current card position
    unsigned int info_text_height; // height of the card name text

    // y coords mapping for cursor -> card position index
    std::pair<int, int> cursor_coords_map[10]; // minus one (1) padding
};

#endif // GAMEAPP_CARDS_MENU_HEADERS defined

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

#define DEBUG_CARDS_MENU
#define DEBUG_CARDS_MENU_OBJ

class CardsMenu: public GameState
{
  public:
    CardsMenu ( Gfx *engine, Collection *cards_db );
    ~CardsMenu ( void );

    void Pause ( void );
    void Resume ( void );

    void HandleInput ( void );
    void Update ( void );
    void Draw ( void );

  private:
    void onExit ( void );
    void onKeyDown ( SDLKey key, SDLMod mod );
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
    Collection *collection; // cards database pointer ref

    SDLBitmapFont info_text;
    SDLBitmapFont info_small_text;

    CardDebug debug; // debug support for card attributes
    Logger logDebug;
    std::vector<int> logger;

    SDLMessageBox menu_box;
    SDLGradient linear;
    GColor msgbox[6];

    //CardView *card; // card rendering
    //CardHand *hand[2]; // player hand

    Sprite cursor; // interface cursor
    Sprite menu_element; // interface menu elements

    unsigned int total_pages;
    unsigned int per_page;
    unsigned int current_index;
    unsigned int current_page;
    unsigned int card_name_height; // height of the card name text

    //std::pair<int, int> cursor_coords_map[4]; // coords mapping: cursor XY to card index
};

#endif // GAMEAPP_CARDS_MENU_HEADERS defined

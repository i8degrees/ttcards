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

#include "SDL_Canvas.hpp"

class CardsMenu: public GameState
{
  public:
    CardsMenu ( void );
    ~CardsMenu ( void );

    void Pause ( void );
    void Resume ( void );

    void Update ( void );
    void Draw ( void *video_buffer );

  private:
    void onKeyDown ( int32_t key, int32_t mod );
    void onJoyButtonDown ( int32_t which, int32_t button );
    void onMouseLeftButtonDown ( int32_t x, int32_t y );
    void onMouseRightButtonDown ( int32_t x, int32_t y );
    void onMouseWheel ( bool up, bool down );
    void Load ( void );

    void reloadDebugFile ( void );
    void updateCursor ( void );
    void drawCursor ( void* video_buffer );
    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    Collection collection; /// cards database
    nom::SDL_Canvas background; /// pointer holding our background image

    nom::SDL_BitmapFont info_text;
    nom::SDL_BitmapFont info_small_text;
    nom::SDL_BitmapFont info_text_gray;

    CardDebug debug; /// debug support for card attributes
    Logger logDebug;
    std::vector<int> logger;

    SDLMessageBox menu_box;
    SDLGradient linear;
    nom::Color msgbox[8];

    CardView card; /// card rendering
    CardHand hand; /// player1 hand
    Card selectedCard; /// CardHand-derived implementation

    nom::SDL_Cursor cursor;
    nom::Sprite menu_element; /// interface menu elements

    unsigned int total_pages; /// MAX_COLLECTION / per_page
    unsigned int per_page; /// number of cards to display per menu page
    unsigned int current_index; /// current card position
    unsigned int info_text_height; /// height of the card name text

    /// y coords mapping for cursor -> card position index
    std::pair<int, int> cursor_coords_map[10]; /// minus one (1) padding
};

#endif // GAMEAPP_CARDS_MENU_HEADERS defined

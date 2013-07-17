/******************************************************************************

    Cards Menu state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARDS_MENU_HEADERS
#define GAMEAPP_CARDS_MENU_HEADERS

#include <iostream>
#include <string>

#include "nomlib/graphics.hpp"
#include "nomlib/gui.hpp"
#include "nomlib/system.hpp"

#include "card.h"
#include "card_debug.h"
#include "card_collection.h"
#include "card_hand.h"
#include "card_view.h"
#include "Game.hpp"
#include "cfg.h"

class CardsMenu: public nom::IState
{
  public:
    CardsMenu ( void );
    ~CardsMenu ( void );

    void onInit ( void );
    void onExit ( void );

    void Pause ( void );
    void Resume ( void );

    void Update ( float delta_time );
    void Draw ( void *video_buffer );

  private:
    void onKeyDown ( int32_t key, int32_t mod );
    void onJoyButtonDown ( int32_t which, int32_t button );
    void onMouseLeftButtonDown ( int32_t x, int32_t y );
    void onMouseRightButtonDown ( int32_t x, int32_t y );
    void onMouseWheel ( bool up, bool down );

    void reloadDebugFile ( void );
    void updateCursor ( void );
    void drawCursor ( void* video_buffer );
    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    /// our public / visible display context handle
    nom::SDL_Display context;
    /// cards database
    Collection collection;
    /// Board background image
    nom::SDL_Canvas background;

    nom::SDL_BitmapFont info_text;
    nom::SDL_BitmapFont info_small_text;
    nom::SDL_BitmapFont info_text_gray;

    /// debug support for card attributes
    CardDebug debug;

    nom::SDL_MessageBox menu_box;
    nom::SDL_Gradient linear;
    std::vector<nom::Color> msgbox;

    /// card rendering
    CardView card;
    /// player1 hand
    CardHand hand;
    /// CardHand-derived implementation
    Card selectedCard;

    /// interface cursor
    nom::SDL_Cursor cursor;
    /// interface menu elements
    nom::Sprite menu_element;

    /// MAX_COLLECTION / per_page
    unsigned int total_pages;
    /// number of cards to display per menu page
    unsigned int per_page;
    /// current card position
    unsigned int current_index;
     /// height of the card name text
    unsigned int info_text_height;

    /// y coords mapping for cursor -> card position index
    /// minus one (1) padding
    std::pair<int, int> cursor_coords_map[10];

    nom::OpenAL::AudioDevice dev;
    nom::OpenAL::SoundBuffer sound_buffer;
    //nom::OpenAL::SoundBuffer move_buffer;
    //nom::OpenAL::SoundBuffer cancel_buffer;
    //nom::OpenAL::SoundBuffer place_buffer;

    nom::OpenAL::Sound cursor_move;
    nom::OpenAL::Sound cursor_cancel;
    nom::OpenAL::Sound card_place;
};

#endif // GAMEAPP_CARDS_MENU_HEADERS defined

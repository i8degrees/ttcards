/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef GAMEAPP_CARDS_MENU_HEADERS
#define GAMEAPP_CARDS_MENU_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/system.hpp>

#include "config.hpp"
#include "resources.hpp"
#include "Card.hpp"
#include "CardsMenuStateCursor.hpp"

#include "Game.hpp"

class CardsMenuState: public nom::IState
{
  public:
    CardsMenuState ( const nom::SDLApp::shared_ptr& object );
    ~CardsMenuState ( void );

    void on_init ( nom::void_ptr data );
    void on_exit ( nom::void_ptr data );

    void on_pause ( nom::void_ptr data );
    void on_resume ( nom::void_ptr data );

    void on_update ( float delta_time );
    void on_draw ( nom::IDrawable::RenderTarget& target );

  private:
    void on_key_down( const nom::Event& ev );

    void on_mouse_left_button_down( const nom::Event& ev );
    void on_mouse_middle_button_down( const nom::Event& ev );
    void on_mouse_right_button_down( const nom::Event& ev );
    void on_mouse_wheel( const nom::Event& ev );

    void on_joy_button_down( const nom::Event& ev );

    void reloadDebugFile ( void );
    void updateCursor ( void );
    void drawCursor ( nom::IDrawable::RenderTarget& target );
    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    Game::SharedPtr game;

    std::shared_ptr<nom::UIWidget> menu_box_window;
    nom::MessageBox* menu_box;

    /// CardHand-derived implementation
    Card selectedCard;

    /// Card::CARDS_COLLECTION / per_page
    unsigned int total_pages;
    /// number of cards to display per menu page
    unsigned int per_page;
    /// current card position
    unsigned int current_index;

    /// height of the card name text
    unsigned int info_text_height;

    /// Y coords mapping for cursor -> card position index
    std::pair<int, int> cursor_coords_map[11];  // Should be the same value as
                                                // the per_page instance var

    //CardsMenuStateCursor cursor;

    /// Position of the player's hand (player 1)
    nom::Point2i player1_pos;

    /// Position of the opponent's hand (player 2)
    nom::Point2i player2_pos;

    /// Position of the card selection from the menu list
    nom::Point2i card_pos;

    nom::Text title_text;
    nom::Text card_text;
};

// Convenience declarations for changing state
typedef std::unique_ptr<CardsMenuState> CardsMenuStatePtr;

#endif // GAMEAPP_CARDS_MENU_HEADERS defined

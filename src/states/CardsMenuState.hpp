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
#include "CardsPageDataSource.hpp"
#include "Card.hpp"

// Forward declarations
class Game;

class CardsMenuState: public nom::IState
{
  public:
    CardsMenuState(nom::SDLApp* object);

    virtual ~CardsMenuState();

    /// \todo Change return type to bool
    void on_init( nom::void_ptr data );
    void on_exit( nom::void_ptr data );

    void on_pause(nom::void_ptr data);
    void on_resume(nom::void_ptr data);

    void on_update( float delta_time );
    void on_draw( nom::RenderWindow& target );

  private:
    /// \brief Injection of the GUI event loop.
    ///
    /// \note This is the current context's event loop (libRocket).
    bool on_event(const nom::Event& ev);

    /// \brief GUI event callback for mouse button actions.
    ///
    /// \note This is processed on libRocket's event loop.
    void on_mouse_button_up(Rocket::Core::Event& ev);

    // void on_left_mouse_button_up(const nom::Event& ev);
    // void on_right_mouse_button_up(const nom::Event& ev);

    void update_cursor();

    /// \brief Set the appropriate page arrows for the shown page.
    void update_page_indicators();

    /// \brief Get the cursor position.
    ///
    /// \returns The card entry's position index, relative to the shown page.
    int cursor_position();

    /// \brief Set the rendering position of the game cursor.
    ///
    /// \param pos The position index of the game cursor, relative to the shown
    /// page.
    void set_cursor_position(int pos);

    /// \brief Go to the previous page.
    void prev_page();

    /// \brief Go to the next page.
    void next_page();

    /// \brief Move the game cursor to the previous card entry.
    void cursor_prev();

    /// \brief Move the game cursor to the next card entry.
    void cursor_next();

    /// \brief Add a card to the player's hand.
    void add_card(const Card& card);

    /// \brief Remove a card from the player's hand.
    void remove_card(const Card& card);

    Game* game;

    Card selected_card_;

    /// \brief Bounds map for game cursor movement.
    std::vector<nom::IntRect> cursor_coords_map_;

    /// \brief Cursor state
    ///
    /// \remarks Not implemented
    nom::uint32 cursor_state_;

    /// Position of the player's hand (player 1)
    nom::Point2i player1_pos;

    /// Position of the opponent's hand (player 2)
    nom::Point2i player2_pos;

    /// Position of the card selection from the menu list
    nom::Point2i card_pos;
};

// Convenience declarations for changing state
typedef std::unique_ptr<CardsMenuState> CardsMenuStatePtr;

#endif // GAMEAPP_CARDS_MENU_HEADERS defined

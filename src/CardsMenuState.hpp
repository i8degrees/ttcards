/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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

#include "card.hpp"
#include "card_debug.hpp"
#include "PlayState.hpp"
#include "GameObject.hpp"
#include "PauseState.hpp"
#include "config.hpp"

class CardsMenuState: public nom::IState
{
  public:
    CardsMenuState ( std::shared_ptr<GameObject> object );
    ~CardsMenuState ( void );

    void onInit ( void );
    void onExit ( void );

    void Pause ( void );
    void Resume ( void );

    void Update ( nom::uint32 delta_time );
    void Draw ( void *video_buffer );

  private:
    void onKeyDown ( int32_t key, int32_t mod );
    void onJoyButtonDown ( int32_t which, int32_t button );
    void onMouseLeftButtonDown ( int32_t x, int32_t y );
    void onMouseRightButtonDown ( nom::int32 x, nom::int32 y );
    void onMouseWheel ( bool up, bool down );

    void reloadDebugFile ( void );
    void updateCursor ( void );
    void drawCursor ( void* video_buffer );
    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    std::shared_ptr<GameObject> game;

    /// debug support for card attributes
    CardDebug debug;

    nom::ui::MessageBox menu_box;

    /// CardHand-derived implementation
    Card selectedCard;

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
};

// Convenience declarations for changing state
typedef std::unique_ptr<CardsMenuState> CardsMenuStatePtr;

#endif // GAMEAPP_CARDS_MENU_HEADERS defined

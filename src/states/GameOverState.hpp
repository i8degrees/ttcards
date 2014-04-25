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
#ifndef GAMEAPP_GAMEOVER_HEADERS
#define GAMEAPP_GAMEOVER_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/system.hpp>

#include "config.hpp"
#include "resources.hpp"
#include "Card.hpp"
#include "CardRules.hpp"
#include "GameOverStateCursor.hpp"

#include "Game.hpp"

class GameOverState: public nom::IState
{
  public:
    GameOverState (
                    const nom::SDLApp::shared_ptr& object,
                    nom::void_ptr state
                  );

    ~GameOverState ( void );

  private:
    void on_init ( nom::void_ptr data );
    void on_exit ( nom::void_ptr data );

    void on_pause ( nom::void_ptr data );
    void on_resume ( nom::void_ptr data );

    void on_key_down( const nom::Event& ev );

    void on_mouse_left_button_down( const nom::Event& ev );
    void on_mouse_middle_button_down( const nom::Event& ev );
    void on_mouse_wheel( const nom::Event& ev );

    void on_joy_button_down( const nom::Event& ev );

    void on_user_event( const nom::Event& ev );

    void on_update ( float delta_time );
    void on_draw ( nom::IDrawable::RenderTarget& target );

    Game::SharedPtr game;

    nom::Timer transistion;
    bool show_results;

    //nom::void_ptr gameover_state;

    /// Interface cursor
    GameOverStateCursor cursor;

    Card selected_card;

    /// \note We segfault if this is not declared as a pointer; nom::Window
    /// *must* be initialized first, so we can pass onto this object instance.
    nom::MessageBox::unique_ptr info_box;

    /// \note We segfault if this is not declared as a pointer; nom::Window
    /// *must* be initialized first, so we can pass onto this object instance.
    nom::MessageBox::unique_ptr card_info_box;

    /// Position of player 1 hand
    nom::Point2i player1_pos;

    /// Position of player 2 hand
    nom::Point2i player2_pos;

    nom::EventDispatcher event;

    nom::Text info_text;
    nom::Text card_text;
};

// Convenience declarations for changing state
typedef std::unique_ptr<GameOverState> GameOverStatePtr;

#endif // GAMEAPP_GAMEOVER_HEADERS defined

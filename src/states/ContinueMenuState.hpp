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
#ifndef GAMEAPP_CONTINUE_MENU_STATE_HEADERS
#define GAMEAPP_CONTINUE_MENU_STATE_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

#include "config.hpp"
#include "ContinueMenuStateCursor.hpp"
#include "Game.hpp"

class ContinueMenuState: public nom::IState
{
  public:
    ContinueMenuState ( const nom::SDLApp::shared_ptr& object );
    ~ContinueMenuState ( void );

  private:
    void on_init ( nom::void_ptr data );
    void on_exit ( nom::void_ptr data );
    void on_resume ( nom::void_ptr data );

    void on_key_down( const nom::Event& ev );
    void on_mouse_wheel( const nom::Event& ev );
    void on_joy_button_down( const nom::Event& ev );
    void on_user_event( const nom::Event& ev );

    /// \remarks This is a delegate method that is used by question_box.
    void on_key_event( const nom::UIEvent& ev );

    /// \remarks This is an event listener callback used by question_box.
    void on_mouse_event( const nom::UIEvent& ev );

    /// \remarks This is an event listener callback used by question_box.
    void on_mouse_dclick( const nom::UIEvent& ev );

    /// \remarks This is an event listener callback used by question_box.
    void on_wheel( const nom::UIEvent& ev );

    /// \brief Implements optional IState::on_event interface method.
    void on_event( const nom::Event& ev );

    void on_update ( float delta_time );
    void on_draw ( nom::IDrawable::RenderTarget& target );

    /// \brief Event handler for player's choice selection.
    void send_response ( void );

    Game::SharedPtr game;

    nom::QuestionDialogBox::unique_ptr question_box;

    ContinueMenuStateCursor cursor;

    nom::Point2i position_map;
};

// Convenience declarations for changing state
typedef std::unique_ptr<ContinueMenuState> ContinueMenuStatePtr;

#endif // include guard defined

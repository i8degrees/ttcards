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

#include <memory>

#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/system.hpp>

#include "config.hpp"
#include "types.hpp"
#include "resources.hpp"
#include "Card.hpp"
#include "GameOverStateCursor.hpp"

namespace tt {

// Forward declarations
class Game;

class GameOverState: public nom::IState
{
  public:
    GameOverState(nom::SDLApp* object, nom::void_ptr state);

    virtual ~GameOverState();

  private:
    /// \todo Change return type to bool
    void on_init( nom::void_ptr data );
    void on_exit( nom::void_ptr data );

    void on_pause( nom::void_ptr data );
    void on_resume( nom::void_ptr data );

    /// \brief The default event handler for this state.
    bool on_event(const nom::Event& ev) override;

    /// \brief Method callback for mouse button actions.
    ///
    /// \see nom::InputMapper.
    void on_mouse_button_down( const nom::Event& ev );

    /// \brief User events handler for this state.
    void on_user_event(const nom::Event& ev);

    void on_update ( float delta_time );
    void on_draw( nom::RenderWindow& target );

    Game* game;

    nom::Timer transistion;
    bool show_results;

    nom::uint32 gameover_state_;

    /// Interface cursor
    GameOverStateCursor cursor_;

    Card selected_card;

    /// Position of player 1 hand
    nom::Point2i player1_pos;

    /// Position of player 2 hand
    nom::Point2i player2_pos;
};

// Convenience declarations for changing state
typedef std::unique_ptr<GameOverState> GameOverStatePtr;

} // namespace tt

#endif // GAMEAPP_GAMEOVER_HEADERS defined

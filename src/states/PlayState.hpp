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
#ifndef GAMEAPP_PLAYSTATE_HEADERS
#define GAMEAPP_PLAYSTATE_HEADERS

#include <string>
#include <iostream>
#include <memory>
#include <algorithm>

#ifdef EMSCRIPTEN
  #include <emscripten.h>
#endif

#include <nomlib/audio.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

#include "config.hpp"
#include "version.hpp"
#include "resources.hpp"
#include "Card.hpp"

// Forward declarations
class Game;
class IPlayer;

class PlayState: public nom::IState
{
  public:
    PlayState(nom::SDLApp* object);
    virtual ~PlayState();

    void on_init( nom::void_ptr data );
    void on_exit( nom::void_ptr data );
    void on_pause( nom::void_ptr data );
    void on_resume( nom::void_ptr data );

    void on_update( float delta_time );
    void on_draw( nom::RenderWindow& target );

  private:
    /// \brief Injection of the GUI event loop.
    ///
    /// \note This is the current context's event loop (libRocket).
    bool on_event(const nom::Event& ev);

    /// \brief Method callback for mouse button actions.
    ///
    /// \see nom::InputMapper.
    void on_mouse_button_down( const nom::Event& ev );

    unsigned int get_turn ( void );
    void player_turn ( unsigned int player );
    void endTurn ( void );

    bool isCursorLocked ( void );
    void lockCursor ( bool lock );
    void resetCursor ( void );
    void unlockSelectedCard ( void );
    void lockSelectedCard ( void );
    void moveTo ( unsigned int x, unsigned int y );

    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    /// Interface Helper method; shows Card's ID number in a message box for
    /// both cursor states; player's hand and placed board cards -- debug
    /// handling included.
    void on_update_info_dialogs( void );
    void updateCursor ( void );
    void drawCursor ( nom::IDrawable::RenderTarget& target );

    /// Update each player's scoreboard
    void updateScore ( void );

    /// \brief Method action callback to load a game.
    ///
    /// \see nom::InputMapper.
    ///
    /// \todo Re-enable debug load game
    bool load_game( nom::uint32 flags );

    /// \brief Method action callback to save a game.
    ///
    /// \see nom::InputMapper.
    bool save_game( nom::uint32 flags );

    Game* game;

    /// \brief Game players
    std::unique_ptr<IPlayer> players_[TOTAL_PLAYERS];

    /// x, y coords mapping for player1, player2 cursor starting position
    nom::Point2i player_cursor_coords[2];

    /// x, y coords mapping for player1, player2 scoreboard positions
    nom::Point2i player_scoreboard[2];

    /// y coords mapping for cursor -> card position index
    nom::Point2i cursor_coords_map[5];

    nom::uint32 turn; // FIXME: player1 = 0, player2 = 1

    enum CursorState
    {
      /// \brief Player hand
      PLAYER = 0,

      /// \brief Board positions
      BOARD
    };

    /// \brief Cursor state
    enum CursorState cursor_state_;

    /// locks cursor state to board placement
    bool cursor_locked;

    /// Animation "blink" timer for visual notification when interface cursor
    /// is locked on a card (board drop mode).
    nom::Timer cursor_blink;
    bool blink_cursor;

    /// Simulate an AI player taking their time in thought about the next move
    nom::AnimationTimer player_timer[2];

    /// Debug option -- when toggled on, we are able to control both players.
    bool skip_turn;

    enum GameOverType gameover_state;
};

// Convenience declarations for changing state
typedef std::unique_ptr<PlayState> PlayStatePtr;

#endif // include guard defined

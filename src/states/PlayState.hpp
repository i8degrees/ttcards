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
#include <memory>

#include <nomlib/graphics.hpp>
#include <nomlib/actions.hpp>
#include <nomlib/system.hpp>

#include "config.hpp"

namespace tt {

// Forward declarations
class Game;
class IPlayer;
class Card;

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
    /// \brief The default event handler for this state.
    bool on_event(const nom::Event& ev) override;

    /// \brief Method callback for mouse button actions.
    ///
    /// \see nom::InputMapper.
    void on_mouse_button_down( const nom::Event& ev );

    nom::uint32 turn();
    void set_player_turn(nom::uint32 player_id);

    // Helper method for incrementing to next player's turn
    void end_turn();

    bool isCursorLocked ( void );
    void lockCursor ( bool lock );
    void resetCursor ( void );
    void unlockSelectedCard ( void );
    void lockSelectedCard ( void );

    /// \brief Helper method for updating board with player's selected card.
    void move_to(const nom::Point2i& rel_board_pos);

    void
    flip_cards( const nom::Point2i& rel_board_pos,
                const std::function<void()>& on_completion_func );

    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    /// Interface Helper method; shows Card's ID number in a message box for
    /// both cursor states; player's hand and placed board cards -- debug
    /// handling included.
    void on_update_info_dialogs();
    void updateCursor ( void );

    /// Update each player's scoreboard
    void updateScore ( void );

    bool save_game();
    bool load_game();

    Game* game;

    /// \brief Game players
    std::unique_ptr<IPlayer> players_[TOTAL_PLAYERS];

    /// x, y coords mapping for player1, player2 cursor starting position
    nom::Point2i player_cursor_coords[2];

    /// x, y coords mapping for player1, player2 scoreboard positions
    nom::Point2i player_scoreboard[2];

    /// y coords mapping for cursor -> card position index
    nom::Point2i cursor_coords_map[5];

    // TODO: We probably ought to be using the value of Card::NOPLAYER,
    // Card::PLAYER1, Card::PLAYER2 here!
    nom::uint32 turn_;

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

    nom::uint32 last_delta_ = 0;

    /// \brief Simulates the CPU player spending time in thought about their
    /// next action.
    nom::real32 cpu_hand_delay_seconds_ = 0.0f;

    /// Debug option -- when toggled on, we are able to control both players.
    bool skip_turn;

    enum GameOverType gameover_state_;

    std::shared_ptr<nom::Sprite> move_card_up_sprite_;
    std::shared_ptr<nom::Sprite> flip_card_sprite_;

    std::shared_ptr<nom::Sprite> text_action_sprite_;
    std::shared_ptr<nom::Sprite> gameover_text_action_sprite_;

    typedef std::function<void(const Card& card)> move_card_up_action_callback;

    void
    move_card_up_action(  const nom::Point2i& rel_board_pos,
                          const move_card_up_action_callback&
                          on_completion_func );

    void
    flip_card_action(const nom::Point2i& rel_board_pos);

    void check_gameover_conditions();

    std::shared_ptr<nom::IActionObject>
    create_gameover_text_action(  GameOverType type,
                                  const std::string& action_name );

    void initialize_cpu_player_turn();
};

// Convenience declarations for changing state
typedef std::unique_ptr<PlayState> PlayStatePtr;

} // namespace tt

#endif // include guard defined

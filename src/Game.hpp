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
#ifndef TTCARDS_GAME_APP_HPP
#define TTCARDS_GAME_APP_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

#include <nomlib/core.hpp>
#include <nomlib/audio.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/actions.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/system.hpp>

#include "config.hpp"
#include "CardHand.hpp"
#include "CardRules.hpp"
#include "GameConfig.hpp"
#include "CardsPageDataSource.hpp"

#define TT_RENDER_ACTION(sprite, action_id) \
  if( this->game->actions_.action_running(action_id) == true && \
      sprite != nullptr && sprite->valid() == true ) \
  { \
    sprite->draw(this->game->window); \
  }

#define TT_RENDER_SPRITE(sprite) \
  if( sprite != nullptr && sprite->valid() == true ) { \
    sprite->draw(this->game->window); \
  }

namespace tt {

// Forward declarations
class CardCollection;
class Board;
class CardRenderer;
class CardResourceLoader;

class Game: public nom::SDLApp
{
  public:
    Game( nom::int32 argc, char* argv[] );
    virtual ~Game();

    bool on_init();

    /// \brief Te main game loop.
    ///
    /// \see main.cpp
    int Run();

    /// \see SDLApp
    void set_state(nom::uint32 id, nom::void_ptr data = nullptr);

    bool
    init_game_rules(const GameConfig* config, tt::RegionRuleSet& region);

    /// \param alpha The starting opacity (origin) to fade the color with.
    void
    fade_window(  nom::real32 duration, const nom::Color4i& color,
                  nom::uint8 alpha, const nom::Size2i& window_dims,
                  const std::function<void()>& on_completion_func = nullptr );

    void on_game_quit(const nom::Event& evt);

    std::shared_ptr<nom::IActionObject>
    create_flip_card_action(  const std::shared_ptr<nom::Sprite>& sp,
                              const nom::Point2i& card_pos );

    bool init_deck(CardCollection* deck, const std::string& filename);

    /// \brief Method callback action for dumping the board data in the game.
    ///
    /// \see nom::InputMapper.
    ///
    /// \remarks Only available when the game is built with debug (developer)
    /// flags.
    void dump_board();

    /// \brief Method callback action for dumping the player's hand data.
    ///
    /// \see nom::InputMapper.
    ///
    /// \remarks Only available when the game is built with debug (developer)
    /// flags.
    void dump_hand(PlayerIndex player_index);

    /// \brief Method callback action for dumping the card collection.
    ///
    /// \see nom::InputMapper.
    ///
    /// \remarks Only available when the game is built with debug (developer)
    /// flags.
    void dump_collection(PlayerIndex player_index);

    /// Audio subsystem
    std::unique_ptr<nom::IAudioDevice> audio_dev_;

    /// Master volume control
    std::unique_ptr<nom::IListener> listener_;

    static const int NUM_SOUND_BUFFERS = 9;

    /// Audio buffers (one buffer per sound)
    std::unique_ptr<nom::ISoundBuffer> sound_buffers[NUM_SOUND_BUFFERS];

    /// Cursor has been moved sound event
    std::unique_ptr<nom::ISoundSource> cursor_move;

    /// Action has been canceled sound event
    std::unique_ptr<nom::ISoundSource> cursor_cancel;

    /// Invalid action sound event
    std::unique_ptr<nom::ISoundSource> cursor_wrong;

    /// Card has been placed sound event
    std::unique_ptr<nom::ISoundSource> card_place;

    /// Card has been flipped sound event
    std::unique_ptr<nom::ISoundSource> card_flip;

    /// Load saved game sound event
    std::unique_ptr<nom::ISoundSource> load_game;

    /// Save game sound event
    std::unique_ptr<nom::ISoundSource> save_game;

    std::unique_ptr<nom::ISoundSource> theme_track_;

    /// Player 1 has won track
    std::unique_ptr<nom::ISoundSource> winning_track;

    // Font resources
    nom::Font card_font_;
    nom::Font gameover_font;
    nom::Font scoreboard_font;
    nom::Font menu_font_;

    nom::Text scoreboard_text[2];
    nom::Text gameover_text;
    nom::Text menu_text_;

    /// Game board
    std::unique_ptr<Board> board_;

    /// \brief The current game rules in effect.
    tt::RegionRuleSet rules_;

    /// \brief The card pool of playable cards.
    std::unique_ptr<CardCollection> cards_db_[TOTAL_PLAYERS];

    /// Player hands
    CardHand hand[2];

    /// \brief Card resources container; background, face, elements, text
    std::unique_ptr<CardResourceLoader> card_res_;

    /// Board background image
    nom::Texture background;

    /// GameOver background image
    nom::Texture gameover_background;

    /// interface cursor
    nom::SpriteSheet right_cursor_frames_;
    nom::SpriteSheet left_cursor_frames_;
    nom::Texture cursor_tex_;
    std::shared_ptr<nom::SpriteBatch> cursor_;
    std::shared_ptr<nom::IActionObject> blinking_cursor_action_;

    /// our public / visible display context handle
    nom::RenderWindow window;

    /// \brief General game configuration.
    std::unique_ptr<GameConfig> config_;

    /// \brief Game resource file paths.
    std::unique_ptr<GameConfig> res_cfg_;

    /// \brief The top-level GUI "window" (desktop)
    nom::UIContext gui_window_;

    /// \brief The UI view for building the player's hand from a deck.
    ///
    /// \see CardsMenuState.
    std::unique_ptr<CardsPageDataSource> cards_page_model_;
    CardStatusFormatter card_status_;
    CardNameFormatter card_;
    CardsAvailableFormatter card_num_;

    /// \brief UI for selection of cards.
    ///
    /// \remarks Used in CardsMenuState.
    nom::UIDataViewList cards_menu_;

    /// \brief UI for pause dialog.
    ///
    /// \remarks Used in PauseState.
    nom::UIMessageBox pause_window_;

    /// \brief UI for confirmation dialogs.
    ///
    /// \remarks Used in ConfirmationDialogState.
    nom::UIQuestionDialogBox question_box_;

    nom::UIMessageBox debug_box_;
    nom::UIMessageBox info_box_;
    nom::UIMessageBox card_info_box_;

    enum State
    {
      CardsMenu = 0,
      Play,
      GameOver,
      Pause,
      ConfirmationDialog,
      MainMenu,
      Options,
    };

    nom::InputStateMapper input_mapper;

    /// Destination directory we descend into to locate game resources
    std::string working_directory;

    /// visual indication of which player's turn it is
    std::shared_ptr<nom::SpriteBatch> triad_;

    std::shared_ptr<nom::IActionObject> triad_action_;

    /// \brief Game animations.
    ///
    /// \remarks This is globally shared across states.
    nom::ActionPlayer actions_;

    std::shared_ptr<nom::Sprite> won_text_sprite_;
    std::shared_ptr<nom::Sprite> lost_text_sprite_;
    std::shared_ptr<nom::Sprite> tied_text_sprite_;

    std::shared_ptr<nom::Sprite> combo_text_sprite_;
    std::shared_ptr<nom::Sprite> same_text_sprite_;
    std::shared_ptr<nom::IActionObject> combo_text_action_;
    std::shared_ptr<nom::IActionObject> same_text_action_;

    /// \brief Toggle switch for in-game debugging features.
    bool debug_game_;

    nom::EventHandler evt_handler_;
    nom::JoystickID joystick_id_ = 0;

    std::string existing_game_cards_db_;
    std::string new_game_cards_db_;

  private:
    /// \brief Method callback action for pausing the music tracks in the game.
    ///
    /// \see nom::InputMapper.
    void pause_music( void );

    /// \brief Method callback action for muting the global audio volume in the
    /// game.
    ///
    /// \see nom::InputMapper.
    void mute_volume( void );

    /// \brief Method callback action for creating a snapshot image in the game.
    ///
    /// \remarks The screen-shots are saved under ~/Documents/ttcards
    ///
    /// \see nom::InputMapper.
    void save_screenshot();

    /// \brief Callback action for reloading the game's configuration files.
    ///
    /// \todo Support hot-loading the configuration file without resetting the
    /// game state.
    ///
    /// \todo Finish reloading of all the applicable game variables in here --
    /// i.e.: animations -- we only do DEBUG_GAME at the moment!
    void reload_config();

    /// \brief The event handler that is called when our window is shown, such
    /// as upon initial creation, restored from a hidden or minimized state,
    /// etc.
    ///
    /// \remarks Re-implements nom::SDLApp::on_window_shown
    void on_window_shown(const nom::Event& evt) override;

    /// \brief The event handler that is called when our window is hidden.
    ///
    /// \remarks Re-implements nom::SDLApp::on_window_hidden
    void on_window_hidden(const nom::Event& evt) override;

    /// \brief The event handler that is called when our window is minimized.
    ///
    /// \remarks Re-implements nom::SDLApp::on_window_minimized
    void on_window_minimized(const nom::Event& evt) override;

    /// \brief The event handler that is called when our window is restored to
    /// normal size and position.
    ///
    /// \remarks Re-implements nom::SDLApp::on_window_restored
    void on_window_restored(const nom::Event& evt) override;

    /// \see Game::fade_window_out
    std::shared_ptr<nom::Sprite> fade_window_sprite_;

    Game* game;

    // IMPORTANT: The running state of this timer controls whether the max or
    // min frame rate is in use.
    nom::Timer fps_timer_;
    nom::uint32 max_frame_interval_ = 0;
    nom::uint32 min_frame_interval_ = 0;
};

} // namespace tt

#endif // include guard defined

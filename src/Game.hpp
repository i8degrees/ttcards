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

#include <nomlib/audio.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/system.hpp>

#include "config.hpp"
#include "version.hpp"
#include "resources.hpp"

#include "Board.hpp"
#include "CardCollection.hpp"
#include "CardDebug.hpp"
#include "CardHand.hpp"
#include "CardView.hpp"
#include "CardRules.hpp"
#include "GameConfig.hpp"

#include "CardsPageDataSource.hpp"

class Game: public nom::SDLApp
{
  public:
    typedef std::shared_ptr<Game> SharedPtr;

    Game();
    Game( nom::int32 argc, char* argv[] );
    virtual ~Game();

    bool on_init( void );

    /// Run app loop
    nom::int32 Run( void );

    /// \see SDLApp
    void set_state( nom::uint32 id, nom::void_ptr data = nullptr );

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

    /// Theme song track
    std::unique_ptr<nom::ISoundSource> music_track;

    /// Player 1 has won track
    std::unique_ptr<nom::ISoundSource> winning_track;

    // Font resources
    nom::Font card_font;
    nom::Font gameover_font;
    nom::Font scoreboard_font;

    nom::Text scoreboard_text[2];
    nom::Text gameover_text;

    /// Game board
    Board board;

    /// Rules logic
    CardRules rules;

    /// Cards database
    CardCollection collection;

    /// Debug support for card attributes
    CardDebug debug;

    /// Player hands
    CardHand hand[2];

    /// Card rendering
    CardView card;

    /// Board background image
    nom::Texture background;

    /// GameOver background image
    nom::Texture gameover_background;

    /// interface cursor
    nom::AnimatedSprite cursor;

    /// our public / visible display context handle
    nom::RenderWindow window;

    /// Variable set configuration properties
    GameConfig config;

    /// \brief The top-level GUI "window" (desktop)
    nom::UIContext gui_window_;

    /// \brief The view used for paged cards list.
    ///
    /// \remarks Used in CardsMenuState.
    std::unique_ptr<CardsPageDataSource> cards_page_model_;
    CardStatusFormatter card_status_;
    CardNameFormatter card_;

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
      ConfirmationDialog
    };

    nom::InputStateMapper input_mapper;

    /// Destination directory we descend into to locate game resources
    std::string working_directory;

  private:
    /// \remarks Re-implements nom::SDLApp::on_event.
    ///
    /// \fixme This is currently required for GUI events processing in
    /// ConfirmationDialogState, and probably can be handled better.
    void on_event( const nom::Event& ev );

    /// Event handler for resize app request
    void on_window_resized( const nom::Event& ev );

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
    void save_screenshot( void );

    /// \brief Method callback action for reloading the configuration -- restart
    /// the game.
    ///
    /// \see nom::InputMapper.
    void reload_config( void );

    /// \brief Method callback action for dumping the board data in the game.
    ///
    /// \see nom::InputMapper.
    ///
    /// \remarks Only available when the game is built with debug (developer)
    /// flags.
    void dump_board( void );

    /// \brief Method callback action for dumping the player's hand data.
    ///
    /// \see nom::InputMapper.
    ///
    /// \remarks Only available when the game is built with debug (developer)
    /// flags.
    void dump_hand( nom::uint32 player_id );

    /// \brief Method callback action for dumping the card collection.
    ///
    /// \see nom::InputMapper.
    ///
    /// \remarks Only available when the game is built with debug (developer)
    /// flags.
    void dump_collection( void );

    Game::SharedPtr game;

    /// Timer for tracking frames per second
    nom::FPS fps;
};

namespace tt {

/// Custom deleter for std::shared_ptr<Game> object; fix for double delete
/// issues.
void free_game ( Game* game );

}

#endif // include guard defined

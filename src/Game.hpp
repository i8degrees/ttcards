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

class Game: public nom::SDLApp
{
  public:
    typedef std::shared_ptr<Game> SharedPtr;

    Game ( void );
    Game ( nom::int32 argc, char* argv[] );
    ~Game ( void );

    bool on_init ( void );

    /// Handle key events
    void on_key_down( const nom::Event& ev );

    /// Event handler for resize app request
    void on_window_resized( const nom::Event& ev );

    /// Run app loop
    nom::int32 Run ( void );

    void set_state ( nom::uint32 id, nom::void_ptr data = nullptr );

    /// Audio subsystem
    nom::OpenAL::AudioDevice dev;

    /// Master volume control
    nom::OpenAL::Listener listener;

    /// Audio buffers (one buffer per sound)
    nom::OpenAL::SoundBuffer sound_buffers[12]; // Four buffers are intentionally
                                                // left unallocated

    /// Cursor has been moved sound event
    nom::OpenAL::Sound cursor_move;

    /// Action has been canceled sound event
    nom::OpenAL::Sound cursor_cancel;

    /// Invalid action sound event
    nom::OpenAL::Sound cursor_wrong;

    /// Card has been placed sound event
    nom::OpenAL::Sound card_place;

    /// Card has been flipped sound event
    nom::OpenAL::Sound card_flip;

    /// Load saved game sound event
    nom::OpenAL::Sound load_game;

    /// Save game sound event
    nom::OpenAL::Sound save_game;

    /// Theme song track
    nom::OpenAL::Music music_track;

    /// Player 1 has won track
    nom::OpenAL::Music winning_track;

    // Font resources
    nom::Font info_text;
    nom::Font info_small_text;
    nom::Font card_font;
    nom::Font gameover_font;
    nom::Font scoreboard_font;

    /// Menu elements
    nom::SpriteBatch menu_elements;

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
    nom::Cursor cursor;

    /// our public / visible display context handle
    nom::RenderWindow window;

    /// Variable set configuration properties
    GameConfig config;

    enum State
    {
      CardsMenu = 0,
      Play,
      GameOver,
      Pause,
      ContinueMenu
    };

  private:
    Game::SharedPtr game;

    nom::Event event;

    /// Timer for tracking frames per second
    nom::FPS fps;
};

namespace tt {

/// Custom deleter for std::shared_ptr<Game> object; fix for double delete
/// issues.
void free_game ( Game* game );

}

#endif // include guard defined

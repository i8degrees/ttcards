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
#ifndef GAMEAPP_GAMEOBJECT_HPP
#define GAMEAPP_GAMEOBJECT_HPP

#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/audio.hpp>

#include "config.hpp"
#include "Board.hpp"
#include "CardCollection.hpp"
#include "CardDebug.hpp"
#include "CardHand.hpp"
#include "CardView.hpp"
#include "CardRules.hpp"
#include "GameConfig.hpp"

struct GameObject
{
  GameObject ( void ) {}
  ~GameObject ( void ) {}

  /// Audio subsystem
  nom::OpenAL::AudioDevice dev;

  /// Master volume control
  nom::OpenAL::Listener listener;

  /// Cursor has been moved sound event
  nom::OpenAL::SoundBuffer cursor_move_buffer;
  nom::OpenAL::Sound cursor_move;

  /// Action has been canceled sound event
  nom::OpenAL::SoundBuffer cursor_cancel_buffer;
  nom::OpenAL::Sound cursor_cancel;

  /// Invalid action sound event
  nom::OpenAL::SoundBuffer cursor_wrong_buffer;
  nom::OpenAL::Sound cursor_wrong;

  /// Card has been placed sound event
  nom::OpenAL::SoundBuffer card_place_buffer;
  nom::OpenAL::Sound card_place;

  /// Card has been flipped sound event
  nom::OpenAL::SoundBuffer card_flip_buffer;
  nom::OpenAL::Sound card_flip;

  /// Load saved game sound event
  nom::OpenAL::SoundBuffer load_game_buffer;
  nom::OpenAL::Sound load_game;

  /// Save game sound event
  nom::OpenAL::SoundBuffer save_game_buffer;
  nom::OpenAL::Sound save_game;

  /// Theme song track
  nom::OpenAL::SoundBuffer music_buffer;
  nom::OpenAL::Music music_track;

  /// Player 1 has won track
  nom::OpenAL::SoundBuffer winning_buffer;
  nom::OpenAL::Music winning_track;

  /// dialog text
  nom::BitmapFont info_text;
  nom::BitmapFont info_small_text;
  nom::BitmapFont info_text_gray;

  /// Game Over text
  nom::TrueTypeFont gameover_text;

  /// Scoreboard text
  nom::TrueTypeFont score_text[2];

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
  nom::ui::Cursor cursor;

  /// our public / visible display context handle
  nom::Window window;

  /// Variable set configuration properties
  GameConfig config;
};


#endif // include guard defined

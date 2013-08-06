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
#ifndef GAMEAPP_TTCARDS_HEADERS
#define GAMEAPP_TTCARDS_HEADERS

#include <string>
#include <iostream>
#include <chrono>
#include <random>
#include <memory>

#ifdef EMSCRIPTEN
  #include <emscripten.h>
#endif

#include <nomlib/audio.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

#include "board.h"
#include "player.h"
#include "card.h"
#include "card_debug.h"
#include "card_collection.h"
#include "card_rules.h"
//#include "cpu_player.h"
#include "CardsMenuState.h"
#include "GameOverState.h"
#include "GameObject.hpp"
#include "cfg.h"

class Game: public nom::IState
{
  public:
    Game ( CardHand& player1_hand, std::shared_ptr<GameObject> object );
    ~Game ( void );

    bool Init ( void );

    void onInit ( void );
    void onExit ( void );

    void Update ( nom::uint32 delta_time );
    void Draw ( void* video_buffer );

    //static void Callback ( void ); // EMCC compiler related
    //void Start ( void ); // EMCC compiler related
  private:
    void onKeyDown ( int32_t key, int32_t mod );
    void onMouseLeftButtonDown ( nom::int32 x, nom::int32 y );
    void onMouseRightButtonDown ( nom::int32 x, nom::int32 y );
    void onMouseWheel ( bool up, bool down );
    void onJoyButtonDown ( int32_t which, int32_t button );

    // Game_debug.cpp
    void debugListCards ( int32_t mod );
    void debugListCollection ( int32_t mod );
    void debugCardsDiscard ( void );
    void debugCardsNoRuleset ( void );
    void debugCardsSameRuleset ( void );
    void removePlayerCard ( void );
    void debugBox ( void );
    void debugModifyCardRank ( bool modifier, nom::uint32 direction );

    unsigned int get_turn ( void );
    void player_turn ( unsigned int player );
    void endTurn ( void );

    void showCardInfoBox ( void *video_buffer );
    bool isCursorLocked ( void );
    void lockCursor ( bool lock );
    void resetCursor ( void );
    void unlockSelectedCard ( void );
    void lockSelectedCard ( void );
    void moveTo ( unsigned int x, unsigned int y );

    nom::Coords getCursorBoardPos ( nom::int32 x, nom::int32 y );
    unsigned int getCursorPos ( void );
    void moveCursorLeft ( void );
    void moveCursorRight ( void );
    void moveCursorUp ( void );
    void moveCursorDown ( void );

    void updateCursor ( void );
    void drawCursor ( void* video_buffer );

    void updateScore ( void );
    void drawScore ( void* video_buffer );

    std::shared_ptr<GameObject> state;

    /// our public / visible display context handle
    nom::Display context;
    /// Board background image
    nom::Canvas background;

    nom::Timer update;

    /// Game Over text
    nom::Text gameOver_text;
    /// dialog text
    nom::BitmapFont info_text;
    nom::BitmapFont info_small_text;

    nom::MessageBox info_box;
    nom::MessageBox debug_box;

    /// scoreboard text
    nom::TrueTypeFont score_text;
    /// game board
    Board board;

    /// debug support for card attributes
    CardDebug debug;

    /// cards database
    Collection collection;
    /// TODO
    CardRules rules;
    /// card rendering
    CardView card;
    /// player hand
    CardHand hand[2];
    /// players in game
    Player player[2];

    /// interface cursor
    nom::Cursor cursor;

    /// x, y coords mapping for player1, player2 cursor starting position
    nom::Coords player_cursor_coords[2];

    /// y coords mapping for cursor -> card position index
    nom::Coords cursor_coords_map[5];

    /// x, y coords for selectedCard from cursor to board placement
    nom::Coords board_coords_map[9];

    nom::OpenAL::AudioDevice dev;
    //nom::OpenAL::SoundBuffer move_buffer;
    //nom::OpenAL::SoundBuffer cancel_buffer;
    //nom::OpenAL::SoundBuffer wrong_buffer;
    //nom::OpenAL::SoundBuffer flip_buffer;
    //nom::OpenAL::SoundBuffer place_buffer;

    nom::OpenAL::Sound cursor_move;
    nom::OpenAL::Sound cursor_cancel;
    nom::OpenAL::Sound cursor_wrong;
    nom::OpenAL::Sound card_flip;
    nom::OpenAL::Sound card_place;

    nom::OpenAL::SoundBuffer music_buffer;
    nom::OpenAL::Music music_track;

    unsigned int turn; // FIXME: player1 = 0, player2 = 1
    /// locks cursor state to board placement
    bool cursor_locked;

    /// visual indication of which player's turn it is
    nom::Rectangle player_rect;
};

#endif // GAMEAPP_TTCARDS_HEADERS defined

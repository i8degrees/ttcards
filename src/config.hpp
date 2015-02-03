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
#ifndef GAMEAPP_CFG_HEADERS
#define GAMEAPP_CFG_HEADERS

#include <string>

#include <nomlib/config.hpp>
#include <nomlib/math.hpp>

#include <cassert>

#if defined(NOM_ASSERT) && ! defined(NDEBUG)
  #undef NOM_ASSERT
  #define NOM_ASSERT(expr) assert(expr)
#endif

#include "debug.hpp"
/// Site-specific constants -- this is likely specific to your own local system
/// setup; auto-generated at compile-time and therefore must recompile to modify
/// said constants.
#include "version.hpp"

/// Scale factor of rendered graphics
///
/// Scale factor is used in computing width, height and origin positioning for
/// the game data used at the time of the compile.
///
/// It is also used to determine which resource files are loaded from
/// Resources/config.json.
///
/// If set to one (1), this will use the original resource data, which is quite
/// low in resolution (384x224).
///
/// If set to two (2), this will use the rescaled resource data (using Scale2X
/// algorithm), giving us a game resolution of 768x448. This is the preferred
/// default.
///
/// On an older system (i.e.: a AMD64 ~1.8Ghz Single Core, GeForce 6200), you
/// may need to drop this down to one (1), in order to get decent frame-rate,
/// as per my experience on the alpha builds of TTcards for WindowsOS. This will
/// hopefully slowly improve as I find time for optimizations!
#define SCALE_FACTOR 2

// Global configuration
const nom::int32 SCREEN_WIDTH = 768;
const nom::int32 SCREEN_HEIGHT = 448;
const nom::int32 SCREEN_BPP = 32;

#if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
  const nom::Size2i GAME_RESOLUTION = nom::Size2i(384,224);
#else
  const nom::Size2i GAME_RESOLUTION = nom::Size2i(768,448);
#endif

/// As per PSX_SCUS Final Fantasy VIII
const nom::uint32 TICKS_PER_SECOND = 15;

/// Calculation used to determine how many ticks to wait between updating
const nom::uint32 SKIP_TICKS = 1000 / TICKS_PER_SECOND;

/// Max amount of frames we can skip when the frame-rate is unstable / variable
const nom::uint32 MAX_FRAMESKIP = 5;

/// \todo Not implemented; this is what TICKS_PER_SECOND is set at
const nom::uint32 FRAMES_PER_SECOND = 15;

/// Maximum number of players -- both human & AI
const nom::int32 TOTAL_PLAYERS = 2; // +1 padding

/// Maximum number of cards in cards database (see Resources/cards.json)
///
/// \remark Loading of the database will fail a sanity check if we go beyond
/// this number
const nom::int32 MAX_COLLECTION = 220;

/// Variable number of cards found in the cards database
/// (see Resources/cards.json)
///
/// \remark This number cannot exceed MAX_COLLECTION.
///
/// \FIXME We have this defined in the Card class as a workaround patch, until
/// we figure out how we want to approach the initialization order issues.
///
/// Card::setID has its ID check disabled as well; Card class appears to be
/// the origin point of troubles.
//extern nom::int32 CARDS_COLLECTION;

/// Invalid Card identifier -- NULL value, if you will.
const nom::sint BAD_CARD_ID = -1;

/// Minimum number of cards a player can hold in a game round
const nom::int32 MIN_PLAYER_HAND = 0;

/// Maximum number of cards a player can hold in a game round
const nom::int32 MAX_PLAYER_HAND = 5;

// Board grid is a dynamically generated 2D array
const nom::int32 BOARD_GRID_WIDTH = 3;
const nom::int32 BOARD_GRID_HEIGHT = 3;

/// Sprite sheet: elements.json
const nom::int32 ELEMENT_NONE = 0;
const nom::int32 ELEMENT_EARTH = 1;
const nom::int32 ELEMENT_FIRE = 2;
const nom::int32 ELEMENT_HOLY = 3;
const nom::int32 ELEMENT_ICE = 4;
const nom::int32 ELEMENT_POISON = 5;
const nom::int32 ELEMENT_THUNDER = 6;
const nom::int32 ELEMENT_WATER = 7;
const nom::int32 ELEMENT_WIND = 8;

/// Sprite sheet: faces.json
const nom::int32 NOFACE_ID = 110;

/// Sprite sheet: cursors.json
const nom::int32 INTERFACE_CURSOR_NONE = 0;
const nom::int32 INTERFACE_CURSOR_LEFT = 1;
const nom::int32 INTERFACE_CURSOR_LEFT_BLINK = 2;
const nom::int32 INTERFACE_CURSOR_RIGHT = 3;
const nom::int32 INTERFACE_CURSOR_RIGHT_BLINK = 4;

/// Sprite sheet: menu_elements.json
const nom::int32 INTERFACE_MENU_ELEMENT = 0;
const nom::int32 INTERFACE_MENU_ELEMENT_USED = 1;
const nom::int32 INTERFACE_MENU_ELEMENT_PAGE_LEFT = 2;
const nom::int32 INTERFACE_MENU_ELEMENT_PAGE_RIGHT = 3;

/// Additional resource data; width, height, origin coordinates
const nom::int32 CARD_WIDTH = 64 * SCALE_FACTOR;
const nom::int32 CARD_HEIGHT = 64 * SCALE_FACTOR;

const nom::int32 ELEMENT_WIDTH = 16 * SCALE_FACTOR;
const nom::int32 ELEMENT_HEIGHT = 16 * SCALE_FACTOR;

const nom::int32 CURSOR_WIDTH = 26 * SCALE_FACTOR;
const nom::int32 CURSOR_HEIGHT = 16 * SCALE_FACTOR;

const nom::int32 MENU_ELEMENT_WIDTH = 16 * SCALE_FACTOR;
const nom::int32 MENU_ELEMENT_HEIGHT = 16 * SCALE_FACTOR;

// top-left of board grid
const nom::int32 BOARD_ORIGIN_X = 96 * SCALE_FACTOR;
const nom::int32 BOARD_ORIGIN_Y = 16 * SCALE_FACTOR;

const nom::int32 PLAYER2_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 ); // 16
const nom::int32 PLAYER2_ORIGIN_Y = BOARD_ORIGIN_Y; // 16

const nom::int32 PLAYER1_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 ) + BOARD_ORIGIN_X + ( CARD_WIDTH * 3 );
const nom::int32 PLAYER1_ORIGIN_Y = BOARD_ORIGIN_Y;

const nom::int32 CURSOR_ORIGIN_X = ( GAME_RESOLUTION.w - CURSOR_WIDTH ) / 2 ;
const nom::int32 CURSOR_ORIGIN_Y = ( GAME_RESOLUTION.h - CURSOR_HEIGHT ) / 2;

const nom::int32 PLAYER2_CURSOR_ORIGIN_X = BOARD_ORIGIN_X - PLAYER2_ORIGIN_X; // 80
const nom::int32 PLAYER2_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 ); // 48

const nom::int32 PLAYER1_CURSOR_ORIGIN_X = BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) - ( CURSOR_WIDTH / 2 ); // 275
const nom::int32 PLAYER1_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 ); // 48

// TODO (player scores):
//
// SCREEN_HEIGHT - 48
// 64 * 5
const nom::int32 PLAYER1_SCORE_ORIGIN_X = 320 * SCALE_FACTOR;
const nom::int32 PLAYER1_SCORE_ORIGIN_Y = 176 * SCALE_FACTOR;

const nom::int32 PLAYER2_SCORE_ORIGIN_X = 32 * SCALE_FACTOR;
const nom::int32 PLAYER2_SCORE_ORIGIN_Y = 176 * SCALE_FACTOR;

// TODO: relocate
const nom::int32 PLAYER1_GAMEOVER_ORIGIN_X = BOARD_ORIGIN_X - ( CARD_WIDTH );
const nom::int32 PLAYER1_GAMEOVER_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT ) + ( CARD_HEIGHT / 2 ) + ( CARD_HEIGHT / 4 );

const nom::int32 PLAYER2_GAMEOVER_ORIGIN_X = BOARD_ORIGIN_X - ( CARD_WIDTH );
const nom::int32 PLAYER2_GAMEOVER_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 3 );

const nom::int32 PLAYER2_GAMEOVER_CURSOR_ORIGIN_X = PLAYER2_GAMEOVER_ORIGIN_X;
const nom::int32 PLAYER2_GAMEOVER_CURSOR_ORIGIN_Y = PLAYER2_GAMEOVER_ORIGIN_Y * 2;

const nom::int32 ELEMENT_ORIGIN_X = 42 * SCALE_FACTOR;
const nom::int32 ELEMENT_ORIGIN_Y = 5 * SCALE_FACTOR;

const nom::int32 RANK_NORTH_ORIGIN_X = 8 * SCALE_FACTOR;
const nom::int32 RANK_NORTH_ORIGIN_Y = 3 * SCALE_FACTOR;

const nom::int32 RANK_EAST_ORIGIN_X = 12 * SCALE_FACTOR;
const nom::int32 RANK_EAST_ORIGIN_Y = 11 * SCALE_FACTOR;

const nom::int32 RANK_SOUTH_ORIGIN_X = RANK_NORTH_ORIGIN_X;
const nom::int32 RANK_SOUTH_ORIGIN_Y = 20 * SCALE_FACTOR;

const nom::int32 RANK_WEST_ORIGIN_X = 4 * SCALE_FACTOR;
const nom::int32 RANK_WEST_ORIGIN_Y = RANK_EAST_ORIGIN_Y;

const nom::Point2i PLAYER1_TRIAD_ORIGIN =
  nom::Point2i(320 * SCALE_FACTOR, 0 * SCALE_FACTOR);

const nom::Point2i PLAYER2_TRIAD_ORIGIN =
  nom::Point2i(40 * SCALE_FACTOR, 0 * SCALE_FACTOR);

/// Card elemental type
enum {
  NONE=0, // type 0; no element
  EARTH=1, // type 1 element
  FIRE=2, // type 2 element
  HOLY=3, // type 3 element
  ICE=4, // type 4 element
  POISON=5, // type 5 element
  THUNDER=6, // type 6 element
  WATER=7, // type 7 element
  WIND=8 // type 8 element
};

/// Card level category
enum {
  MONSTER=1,
  BOSS=2,
  GF=3,
  PLAYER=4
};

// Card ranking sides
enum {
  NORTH=0,
  EAST=1,
  SOUTH=2,
  WEST=3
};

// Game::turn[players]
enum {
  PLAYER1=0,
  PLAYER2=1
};

enum GameOverType: nom::uint32
{
  NotOver = 0,
  Tie,
  Won,
  Lost
};

enum BoardPosition: nom::int32
{
  TopLeft = 0,
  TopCenter,
  TopRight,
  MiddleLeft,
  MiddleCenter,
  MiddleRight,
  BottomLeft,
  BottomCenter,
  BottomRight
};

enum GameEvent: nom::int32
{
  AnimationEvent,
  DebugEvent,
  GUIEvent,
  AudioEvent,

  /// \remarks Starting event number for Game::State enum (to ensure unique
  /// code identifiers).
  NUM_EVENTS
};

// The gradient colors used to render card backgrounds.

const nom::Color4iColors CARD_BG_PLAYER1 = {{
  // light to dark blue
  nom::Color4i( 208, 223, 255 ), nom::Color4i( 50, 59, 114 )
}};

const nom::Color4iColors CARD_BG_PLAYER2 = {{
  // light to dark pink
  nom::Color4i( 251, 222, 232 ), nom::Color4i( 114, 66, 66 )
}};

const nom::Color4iColors CARD_BG_NO_PLAYER = {{
  // light to dark gray
  nom::Color4i( 197, 197, 197 ), nom::Color4i( 84, 84, 84 )
}};

#endif // GAMEAPP_CFG_HEADERS defined

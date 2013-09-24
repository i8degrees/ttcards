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
#ifndef GAMEAPP_CFG_HEADERS
#define GAMEAPP_CFG_HEADERS

#include <string>

// We borrow several C macros from here for conditional debug logging
#include <nomlib/config.hpp>

/// Cross-platform support files
#include <nomlib/types.hpp> // data types

/// Site-specific constants -- this is likely specific to your own local system
/// setup; auto-generated at compile-time and therefore must recompile to modify
/// said constants.
#include "version.hpp"

/// Scale factor of graphics; only a scaling factor of 1..3 is supported at the
/// moment. Resource data is not scaled in real-time (yet), so you must also
/// have the accompanying resource files.
///
/// Scale factor is used in computing width, height and origin positioning for
/// applicable resources at compile-time.
///
/// Resources are scaled up using either the scale2x [1] or hq2x [2] algorithm.
///
/// If left undefined or if set to one (1), this will trigger the default of
/// the original scaling of resource data (low resolution).
///
/// Scale factor of 2..3 scales up by said factor to produce high resolution
/// graphics for display.
///
/// I do not support anything above a scale factor of 3 due to hardware
/// limitations; my native resolution only goes up to 1440x900.
///
/// 1. http://scale2x.sourceforge.net/
///
/// Note that our current implementation of the hq2x algorithm is far less
/// efficient than the scale2x one and therefore expect to see a considerable
/// difference in load times in addition to memory usage.
///
/// On my system -- mid 2011 MacBook Air -- hq2x requires ~65MB more than
/// scale2x and requires roughly 2..3 seconds compared to scale2x being almost
/// instant (~1s or less) for the initial scaling computations and caching
/// to complete.
///
/// To be fair, I am rescaling several of the images well beyond the recommended
/// 256x256 guidelines and do not know how the algorithms compare on equal
/// ground.
#define SCALE_FACTOR 2

// Global configuration
constexpr nom::int32 SCREEN_WIDTH = 384 * SCALE_FACTOR;
constexpr nom::int32 SCREEN_HEIGHT = 224 * SCALE_FACTOR;
constexpr nom::int32 SCREEN_BPP = 32;

/// As per PSX_SCUS Final Fantasy VIII
constexpr nom::uint32 TICKS_PER_SECOND = 15;

/// Calculation used to determine how many ticks to wait between updating
constexpr nom::uint32 SKIP_TICKS = 1000 / TICKS_PER_SECOND;

/// Max amount of frames we can skip when the frame-rate is unstable / variable
constexpr nom::uint32 MAX_FRAMESKIP = 5;

/// \todo Not implemented; this is what TICKS_PER_SECOND is set at
constexpr nom::uint32 FRAMES_PER_SECOND = 15;

/// Maximum number of players -- both human & AI
constexpr nom::int32 TOTAL_PLAYERS = 2; // +1 padding

/// Maximum number of cards in cards database; see cards.json
constexpr nom::int32 MAX_COLLECTION = 110;

/// Maximum number of cards a player can hold in a game round
constexpr nom::int32 MAX_PLAYER_HAND = 5;

// Board grid is a dynamically generated 2D array
constexpr nom::int32 BOARD_GRID_WIDTH = 3;
constexpr nom::int32 BOARD_GRID_HEIGHT = 3;

/// Sprite sheet: elements.json
constexpr nom::int32 ELEMENT_NONE = 0;
constexpr nom::int32 ELEMENT_EARTH = 1;
constexpr nom::int32 ELEMENT_FIRE = 2;
constexpr nom::int32 ELEMENT_HOLY = 3;
constexpr nom::int32 ELEMENT_ICE = 4;
constexpr nom::int32 ELEMENT_POISON = 5;
constexpr nom::int32 ELEMENT_THUNDER = 6;
constexpr nom::int32 ELEMENT_WATER = 7;
constexpr nom::int32 ELEMENT_WIND = 8;

/// Sprite sheet: faces.json
constexpr nom::int32 NOFACE_ID = 110;

/// Sprite sheet: cursors.json
constexpr nom::int32 INTERFACE_CURSOR_NONE = 0;
constexpr nom::int32 INTERFACE_CURSOR_LEFT = 1;
constexpr nom::int32 INTERFACE_CURSOR_LEFT_BLINK = 2;
constexpr nom::int32 INTERFACE_CURSOR_RIGHT = 3;
constexpr nom::int32 INTERFACE_CURSOR_RIGHT_BLINK = 4;

/// Sprite sheet: menu_elements.json
constexpr nom::int32 INTERFACE_MENU_ELEMENT = 0;
constexpr nom::int32 INTERFACE_MENU_ELEMENT_USED = 1;
constexpr nom::int32 INTERFACE_MENU_ELEMENT_PAGE_LEFT = 2;
constexpr nom::int32 INTERFACE_MENU_ELEMENT_PAGE_RIGHT = 3;

/// Additional resource data; width, height, origin coordinates
constexpr nom::int32 CARD_WIDTH = 64 * SCALE_FACTOR;
constexpr nom::int32 CARD_HEIGHT = 64 * SCALE_FACTOR;

constexpr nom::int32 ELEMENT_WIDTH = 16 * SCALE_FACTOR;
constexpr nom::int32 ELEMENT_HEIGHT = 16 * SCALE_FACTOR;

constexpr nom::int32 CURSOR_WIDTH = 26 * SCALE_FACTOR;
constexpr nom::int32 CURSOR_HEIGHT = 16 * SCALE_FACTOR;

constexpr nom::int32 MENU_ELEMENT_WIDTH = 16 * SCALE_FACTOR;
constexpr nom::int32 MENU_ELEMENT_HEIGHT = 16 * SCALE_FACTOR;

// top-left of board grid
constexpr nom::int32 BOARD_ORIGIN_X = 96 * SCALE_FACTOR;
constexpr nom::int32 BOARD_ORIGIN_Y = 16 * SCALE_FACTOR;

constexpr nom::int32 PLAYER2_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 ); // 16
constexpr nom::int32 PLAYER2_ORIGIN_Y = BOARD_ORIGIN_Y; // 16

constexpr nom::int32 PLAYER1_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 ) + BOARD_ORIGIN_X + ( CARD_WIDTH * 3 );
constexpr nom::int32 PLAYER1_ORIGIN_Y = BOARD_ORIGIN_Y;

constexpr nom::int32 CURSOR_ORIGIN_X = ( SCREEN_WIDTH - CURSOR_WIDTH ) / 2 ;
constexpr nom::int32 CURSOR_ORIGIN_Y = ( SCREEN_HEIGHT - CURSOR_HEIGHT ) / 2;

constexpr nom::int32 PLAYER2_CURSOR_ORIGIN_X = BOARD_ORIGIN_X - PLAYER2_ORIGIN_X; // 80
constexpr nom::int32 PLAYER2_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 ); // 48

constexpr nom::int32 PLAYER1_CURSOR_ORIGIN_X = BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) - ( CURSOR_WIDTH / 2 ); // 275
constexpr nom::int32 PLAYER1_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 ); // 48

// TODO (player scores):
//
// SCREEN_HEIGHT - 48
// 64 * 5
constexpr nom::int32 PLAYER1_SCORE_ORIGIN_X = 320 * SCALE_FACTOR;
constexpr nom::int32 PLAYER1_SCORE_ORIGIN_Y = 176 * SCALE_FACTOR;

constexpr nom::int32 PLAYER2_SCORE_ORIGIN_X = 32 * SCALE_FACTOR;
constexpr nom::int32 PLAYER2_SCORE_ORIGIN_Y = 176 * SCALE_FACTOR;

// TODO: relocate
constexpr nom::int32 PLAYER1_GAMEOVER_ORIGIN_X = BOARD_ORIGIN_X - ( CARD_WIDTH );
constexpr nom::int32 PLAYER1_GAMEOVER_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT ) + ( CARD_HEIGHT / 2 ) + ( CARD_HEIGHT / 4 );

constexpr nom::int32 PLAYER2_GAMEOVER_ORIGIN_X = BOARD_ORIGIN_X - ( CARD_WIDTH );
constexpr nom::int32 PLAYER2_GAMEOVER_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 3 );

constexpr nom::int32 PLAYER2_GAMEOVER_CURSOR_ORIGIN_X = PLAYER2_GAMEOVER_ORIGIN_X;
constexpr nom::int32 PLAYER2_GAMEOVER_CURSOR_ORIGIN_Y = PLAYER2_GAMEOVER_ORIGIN_Y * 2;

constexpr nom::int32 ELEMENT_ORIGIN_X = 42 * SCALE_FACTOR;
constexpr nom::int32 ELEMENT_ORIGIN_Y = 5 * SCALE_FACTOR;

constexpr nom::int32 RANK_NORTH_ORIGIN_X = 8 * SCALE_FACTOR;
constexpr nom::int32 RANK_NORTH_ORIGIN_Y = 3 * SCALE_FACTOR;

constexpr nom::int32 RANK_EAST_ORIGIN_X = 12 * SCALE_FACTOR;
constexpr nom::int32 RANK_EAST_ORIGIN_Y = 11 * SCALE_FACTOR;

constexpr nom::int32 RANK_SOUTH_ORIGIN_X = RANK_NORTH_ORIGIN_X;
constexpr nom::int32 RANK_SOUTH_ORIGIN_Y = 20 * SCALE_FACTOR;

constexpr nom::int32 RANK_WEST_ORIGIN_X = 4 * SCALE_FACTOR;
constexpr nom::int32 RANK_WEST_ORIGIN_Y = RANK_EAST_ORIGIN_Y;

// interface_pickOutCards() Menu
constexpr nom::int32 PICK_CARDS_MENU_ORIGIN_X = 60 * SCALE_FACTOR;
constexpr nom::int32 PICK_CARDS_MENU_ORIGIN_Y = 25 * SCALE_FACTOR;

constexpr nom::int32 PICK_CARDS_MENU_WIDTH = 164 * SCALE_FACTOR;
constexpr nom::int32 PICK_CARDS_MENU_HEIGHT = 196 * SCALE_FACTOR;

// Starting cursor positioning
constexpr nom::int32 MENU_CARDS_CURSOR_ORIGIN_X = 40 * SCALE_FACTOR;
constexpr nom::int32 MENU_CARDS_CURSOR_ORIGIN_Y = 34 * SCALE_FACTOR;

// "Cards" Title Text
constexpr nom::int32 MENU_CARDS_TITLE_ORIGIN_X = 64 * SCALE_FACTOR;
constexpr nom::int32 MENU_CARDS_TITLE_ORIGIN_Y = PICK_CARDS_MENU_ORIGIN_Y;

// "P" (page) number header text
constexpr nom::int32 MENU_CARDS_TITLE_PAGE_ORIGIN_X = 119 * SCALE_FACTOR;
constexpr nom::int32 MENU_CARDS_TITLE_PAGE_ORIGIN_Y = PICK_CARDS_MENU_ORIGIN_Y;

// "Num" header text
constexpr nom::int32 MENU_CARDS_TITLE_NUM_ORIGIN_X = 184 * SCALE_FACTOR;
constexpr nom::int32 MENU_CARDS_TITLE_NUM_ORIGIN_Y = PICK_CARDS_MENU_ORIGIN_Y;

// Field elements
constexpr nom::int32 MENU_CARDS_FIELD_ORIGIN_X = 64 * SCALE_FACTOR;
constexpr nom::int32 MENU_CARDS_FIELD_ORIGIN_Y = 30 * SCALE_FACTOR;

// Card selection helper element
constexpr nom::int32 MENU_CARDS_HELPER_ORIGIN_X = MENU_CARDS_FIELD_ORIGIN_X;
constexpr nom::int32 MENU_CARDS_HELPER_ORIGIN_Y = MENU_CARDS_FIELD_ORIGIN_Y;

// Card name text
constexpr nom::int32 MENU_CARDS_NAME_ORIGIN_X = 80 * SCALE_FACTOR;
constexpr nom::int32 MENU_CARDS_NAME_ORIGIN_Y = MENU_CARDS_FIELD_ORIGIN_Y;

// Number of cards text
constexpr nom::int32 MENU_CARDS_NUM_ORIGIN_X = 210 * SCALE_FACTOR;
constexpr nom::int32 MENU_CARDS_NUM_ORIGIN_Y = MENU_CARDS_FIELD_ORIGIN_Y;

constexpr nom::int32 MENU_CARDS_PAGE_LEFT_ORIGIN_X = 58 * SCALE_FACTOR;
constexpr nom::int32 MENU_CARDS_PAGE_LEFT_ORIGIN_Y = 208 * SCALE_FACTOR;

constexpr nom::int32 MENU_CARDS_PAGE_RIGHT_ORIGIN_X = 210 * SCALE_FACTOR;
constexpr nom::int32 MENU_CARDS_PAGE_RIGHT_ORIGIN_Y = MENU_CARDS_PAGE_LEFT_ORIGIN_Y;

constexpr nom::int32 INFO_BOX_WIDTH = 176 * SCALE_FACTOR;
constexpr nom::int32 INFO_BOX_HEIGHT = 24 * SCALE_FACTOR;

constexpr nom::int32 INFO_BOX_ORIGIN_X = ( SCREEN_WIDTH - INFO_BOX_WIDTH ) / 2;
constexpr nom::int32 INFO_BOX_ORIGIN_Y = 194 * SCALE_FACTOR;

constexpr nom::int32 DEBUG_BOX_WIDTH = 88 * SCALE_FACTOR;
constexpr nom::int32 DEBUG_BOX_HEIGHT = 24 * SCALE_FACTOR;

constexpr nom::int32 DEBUG_BOX_ORIGIN_X = ( SCREEN_WIDTH - DEBUG_BOX_WIDTH ) / 2;
constexpr nom::int32 DEBUG_BOX_ORIGIN_Y = 8 * SCALE_FACTOR;

constexpr nom::int32 PAUSE_BOX_WIDTH = 176 * SCALE_FACTOR;
constexpr nom::int32 PAUSE_BOX_HEIGHT = 24 * SCALE_FACTOR;

constexpr nom::int32 PAUSE_BOX_ORIGIN_X = ( SCREEN_WIDTH - PAUSE_BOX_WIDTH ) / 2;
constexpr nom::int32 PAUSE_BOX_ORIGIN_Y = ( SCREEN_HEIGHT - PAUSE_BOX_HEIGHT ) / 2;

constexpr nom::int32 OPTION_BOX_WIDTH = 116 * SCALE_FACTOR;
constexpr nom::int32 OPTION_BOX_HEIGHT = 72 * SCALE_FACTOR;

constexpr nom::int32 OPTION_BOX_ORIGIN_X = ( SCREEN_WIDTH - OPTION_BOX_WIDTH ) / 2;
constexpr nom::int32 OPTION_BOX_ORIGIN_Y = ( SCREEN_HEIGHT - OPTION_BOX_HEIGHT ) / 2;

constexpr nom::int32 PLAYER_INDICATOR_WIDTH = 16 * SCALE_FACTOR;
constexpr nom::int32 PLAYER_INDICATOR_HEIGHT = 16 * SCALE_FACTOR;

constexpr nom::int32 PLAYER1_INDICATOR_ORIGIN_X = 320 * SCALE_FACTOR;
constexpr nom::int32 PLAYER1_INDICATOR_ORIGIN_Y = 0 * SCALE_FACTOR;

constexpr nom::int32 PLAYER2_INDICATOR_ORIGIN_X = 40 * SCALE_FACTOR;
constexpr nom::int32 PLAYER2_INDICATOR_ORIGIN_Y = 0 * SCALE_FACTOR;

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

enum GameOverType
{
  NotOver = 0,
  Tie,
  Won,
  Lost
};

enum class BoardPosition: nom::int32
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

#endif // GAMEAPP_CFG_HEADERS defined

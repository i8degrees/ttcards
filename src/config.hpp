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
#include <nomlib/types.hpp> // portable fixed-types
#include <nomlib/system/clock.hpp>

#ifdef TTCARDS_DEBUG

  // Output info messages if debugging is turned on
  #define TTCARDS_LOG_INFO(message) \
    ( std::cout << "TTCARDS_LOG_INFO at " << nom::getCurrentTime() << message << std::endl << std::endl )

#else // Do not add any overhead
  #define TTCARDS_LOG_INFO(message)
#endif

#ifdef TTCARDS_DEBUG_TRACE

  // If trace debugging is turned on, show class construction and destruction
  #define TTCARDS_LOG_CLASSINFO \
    ( std::cout << "TTCARDS_LOG_CLASSINFO at " << nom::getCurrentTime() << __func__ << std::endl << std::endl )

#else // Do not add any overhead
  #define TTCARDS_LOG_CLASSINFO
#endif

/// Scale factor of graphics; only a scaling factor of 1..3 is supported at the
/// moment. Resource data is not scaled in real-time (yet), so you must also
/// have the accompanying resource files.
///
/// Scale factor is used in computing width, height and origin positioning for
/// applicable resources at compile-time.
///
/// Resources are scaled up using the scale2x [1] algorithm.
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
#define SCALE_FACTOR 2

extern const std::string APP_NAME;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

/// As per PSX_SCUS Final Fantasy VIII
extern const nom::uint32 TICKS_PER_SECOND;

/// Calculation used to determine how many ticks to wait between updating
extern const nom::uint32 SKIP_TICKS;

/// Max amount of frames we can skip when the frame-rate is unstable / variable
extern const nom::uint32 MAX_FRAMESKIP;

/// \todo Not implemented; this is what TICKS_PER_SECOND is set at
extern const nom::uint32 FRAMES_PER_SECOND;

/// Maximum number of players -- both human & AI
extern const nom::uint32 TOTAL_PLAYERS;

/// Maximum number of cards in cards database; see cards.json
extern const int MAX_COLLECTION;

/// Maximum number of cards a player can hold in a game round
extern const nom::int32 MAX_PLAYER_HAND;

// Board grid is a dynamically generated 2D array
extern const nom::int32 BOARD_GRID_WIDTH;
extern const nom::int32 BOARD_GRID_HEIGHT;

// Resources directory is run-time dependent; set in main.cpp
extern std::string WORKING_DIR;

// App data
extern const std::string APP_ICON;
extern const std::string CARDS_DB;

// Fonts
extern const std::string SCORE_FONTFACE;
extern const std::string CARD_FONTFACE;
extern const std::string INFO_FONTFACE;
extern const std::string INFO_SMALL_FONTFACE;

// Static backgrounds & sprites
extern const std::string BOARD_BACKGROUND;
extern const std::string GAMEOVER_BACKGROUND;
extern const std::string CARD_ELEMENTS;
extern const std::string CARD_FACES;
extern const std::string CARD_BACKGROUNDS;
extern const std::string INTERFACE_CURSOR;
extern const std::string MENU_ELEMENTS;

// Audio
extern const std::string CURSOR_MOVE;
extern const std::string CURSOR_CANCEL;
extern const std::string CURSOR_WRONG;
extern const std::string CARD_FLIP;
extern const std::string CARD_PLACE;
extern const std::string MUSIC_TRACK;
extern const std::string MUSIC_ALT_TRACK;
extern const std::string MUSIC_WINNING_TRACK;

// Sprite sheet IDs
extern const int ELEMENT_NONE;
extern const int ELEMENT_EARTH;
extern const int ELEMENT_FIRE;
extern const int ELEMENT_HOLY;
extern const int ELEMENT_ICE;
extern const int ELEMENT_POISON;
extern const int ELEMENT_THUNDER;
extern const int ELEMENT_WATER;
extern const int ELEMENT_WIND;

extern const int NOFACE_ID;
extern const int NOPLAYER_BACKGROUND_ID;
extern const int PLAYER1_BACKGROUND_ID;
extern const int PLAYER2_BACKGROUND_ID;

extern const int INTERFACE_CURSOR_NONE;
extern const int INTERFACE_CURSOR_LEFT;
extern const int INTERFACE_CURSOR_RIGHT;

extern const int INTERFACE_MENU_ELEMENT;
extern const int INTERFACE_MENU_ELEMENT_USED;
extern const int INTERFACE_MENU_ELEMENT_PAGE_LEFT;
extern const int INTERFACE_MENU_ELEMENT_PAGE_RIGHT;

extern const int NOPLAYER_ID;
extern const int PLAYER1_ID;
extern const int PLAYER2_ID;
extern const int PLAYER_AI_ID;

/// Additional resource data; width, height, origin coordinates
extern const nom::int32 CARD_WIDTH;
extern const nom::int32 CARD_HEIGHT;
extern const int BACKGROUND_WIDTH;
extern const int BACKGROUND_HEIGHT;
extern const int ELEMENT_WIDTH;
extern const int ELEMENT_HEIGHT;
extern const int CURSOR_WIDTH;
extern const int CURSOR_HEIGHT;
extern const int MENU_ELEMENT_WIDTH;
extern const int MENU_ELEMENT_HEIGHT;

extern const nom::int32 BOARD_ORIGIN_X;
extern const nom::int32 BOARD_ORIGIN_Y;

extern const int PLAYER1_ORIGIN_X;
extern const int PLAYER1_ORIGIN_Y;

extern const int PLAYER2_ORIGIN_X;
extern const int PLAYER2_ORIGIN_Y;

extern const int CURSOR_ORIGIN_X;
extern const int CURSOR_ORIGIN_Y;

extern const nom::int32 PLAYER1_CURSOR_ORIGIN_X;
extern const nom::int32 PLAYER1_CURSOR_ORIGIN_Y;

extern const int PLAYER2_CURSOR_ORIGIN_X;
extern const int PLAYER2_CURSOR_ORIGIN_Y;

extern const int PLAYER1_SCORE_ORIGIN_X;
extern const int PLAYER1_SCORE_ORIGIN_Y;

extern const int PLAYER2_SCORE_ORIGIN_X;
extern const int PLAYER2_SCORE_ORIGIN_Y;

extern const int PLAYER1_GAMEOVER_ORIGIN_X;
extern const int PLAYER1_GAMEOVER_ORIGIN_Y;

extern const int PLAYER2_GAMEOVER_ORIGIN_X;
extern const int PLAYER2_GAMEOVER_ORIGIN_Y;

extern const int CARD_ORIGIN_X;
extern const int CARD_ORIGIN_Y;

extern const int CARD_FACE_ORIGIN_X;
extern const int CARD_FACE_ORIGIN_Y;

extern const int BACKGROUND_ORIGIN_X;
extern const int BACKGROUND_ORIGIN_Y;

extern const int ELEMENT_ORIGIN_X;
extern const int ELEMENT_ORIGIN_Y;

extern const int RANK_NORTH_ORIGIN_X;
extern const int RANK_NORTH_ORIGIN_Y;

extern const int RANK_EAST_ORIGIN_X;
extern const int RANK_EAST_ORIGIN_Y;

extern const int RANK_SOUTH_ORIGIN_X;
extern const int RANK_SOUTH_ORIGIN_Y;

extern const int RANK_WEST_ORIGIN_X;
extern const int RANK_WEST_ORIGIN_Y;

extern const int CARD_ID_ORIGIN_X;
extern const int CARD_ID_ORIGIN_Y;

extern const int PICK_CARDS_MENU_ORIGIN_X;
extern const int PICK_CARDS_MENU_ORIGIN_Y;

extern const nom::int32 PICK_CARDS_MENU_WIDTH;
extern const nom::int32 PICK_CARDS_MENU_HEIGHT;

extern const nom::int32 MENU_CARDS_CURSOR_ORIGIN_X;
extern const nom::int32 MENU_CARDS_CURSOR_ORIGIN_Y;

extern const int MENU_CARDS_TITLE_ORIGIN_X;
extern const int MENU_CARDS_TITLE_ORIGIN_Y;

extern const int MENU_CARDS_TITLE_PAGE_ORIGIN_X;
extern const int MENU_CARDS_TITLE_PAGE_ORIGIN_Y;

extern const int MENU_CARDS_TITLE_NUM_ORIGIN_X;
extern const int MENU_CARDS_TITLE_NUM_ORIGIN_Y;

extern const int MENU_CARDS_FIELD_ORIGIN_X;
extern const int MENU_CARDS_FIELD_ORIGIN_Y;

extern const int MENU_CARDS_HELPER_ORIGIN_X;
extern const int MENU_CARDS_HELPER_ORIGIN_Y;

extern const int MENU_CARDS_NAME_ORIGIN_X;
extern const int MENU_CARDS_NAME_ORIGIN_Y;

extern const int MENU_CARDS_NUM_ORIGIN_X;
extern const int MENU_CARDS_NUM_ORIGIN_Y;

extern const int MENU_CARDS_PAGE_LEFT_ORIGIN_X;
extern const int MENU_CARDS_PAGE_LEFT_ORIGIN_Y;

extern const int MENU_CARDS_PAGE_RIGHT_ORIGIN_X;
extern const int MENU_CARDS_PAGE_RIGHT_ORIGIN_Y;

extern const nom::int32 INFO_BOX_WIDTH;
extern const nom::int32 INFO_BOX_HEIGHT;

extern const nom::int32 INFO_BOX_ORIGIN_X;
extern const nom::int32 INFO_BOX_ORIGIN_Y;
extern const nom::int32 INFO_BOX_TEXT_ORIGIN_Y;

extern const nom::int32 INFO_BOX_SMALL_TEXT_ORIGIN_X;
extern const nom::int32 INFO_BOX_SMALL_TEXT_ORIGIN_Y;

extern const nom::int32 DEBUG_BOX_WIDTH;
extern const nom::int32 DEBUG_BOX_HEIGHT;

extern const nom::int32 DEBUG_BOX_ORIGIN_X;
extern const nom::int32 DEBUG_BOX_ORIGIN_Y;
extern const nom::int32 DEBUG_BOX_TEXT_ORIGIN_Y;

extern const nom::int32 PAUSE_BOX_WIDTH;
extern const nom::int32 PAUSE_BOX_HEIGHT;

extern const nom::int32 PAUSE_BOX_ORIGIN_X;
extern const nom::int32 PAUSE_BOX_ORIGIN_Y;

extern const nom::int32 PLAYER_INDICATOR_WIDTH;
extern const nom::int32 PLAYER_INDICATOR_HEIGHT;

extern const nom::int32 PLAYER1_INDICATOR_ORIGIN_X;
extern const nom::int32 PLAYER1_INDICATOR_ORIGIN_Y;

extern const nom::int32 PLAYER2_INDICATOR_ORIGIN_X;
extern const nom::int32 PLAYER2_INDICATOR_ORIGIN_Y;

extern const std::string LOADING_TEXT;

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

#endif // GAMEAPP_CFG_HEADERS defined

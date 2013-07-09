/******************************************************************************

    TTcards configuration declarations

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CFG_HEADERS
#define GAMEAPP_CFG_HEADERS

#include <string> // for std::string support
#include <nomlib/types.hpp> // for portable fixed-types

#define TTCARDS_DEBUG
//#define TTCARDS_RELEASE

#define DEBUG_GAME
#define DEBUG_GAME_OBJ

#define DEBUG_BOARD
#define DEBUG_BOARD_CMP
#define DEBUG_BOARD_OBJ

#define DEBUG_PLAYER
#define DEBUG_PLAYER_OBJ

#define DEBUG_CPU_PLAYER
#define DEBUG_CPU_PLAYER_OBJ

//#define DEBUG_CARD_HAND
#define DEBUG_CARD_HAND_OBJ

#define DEBUG_CARD_DEBUG
//#define DEBUG_CARD_DEBUG_OBJ

#define DEBUG_CARD_VIEW
#define DEBUG_CARD_VIEW_OBJ

//#define DEBUG_CARD
//#define DEBUG_CARD_OBJ

//#define DEBUG_CARD_COLLECTION
//#define DEBUG_CARD_COLLECTION_OBJ

#define DEBUG_CARD_RULES
#define DEBUG_CARD_RULES_OBJ

#define DEBUG_GAMEOVER
#define DEBUG_GAMEOVER_OBJ

#define DEBUG_CARDS_MENU
//#define DEBUG_CARDS_MENU_CURSOR
#define DEBUG_CARDS_MENU_OBJ

extern std::string APP_NAME;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern const nom::uint32 TICKS_PER_SECOND;
extern const nom::uint32 SKIP_TICKS;
extern const nom::uint32 MAX_FRAMESKIP;
extern const nom::uint32 FRAMES_PER_SECOND;

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

extern const nom::uint32 TOTAL_PLAYERS;
extern const int MAX_COLLECTION;
extern const nom::int32 MAX_PLAYER_HAND;

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

extern const nom::int32 BOARD_GRID_WIDTH;
extern const nom::int32 BOARD_GRID_HEIGHT;

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

extern std::string WORKING_DIR;

extern const std::string CARDS_DB;
extern const std::string MENU_ELEMENTS;
extern const std::string CARD_FONTFACE;
extern const std::string INFO_FONTFACE;
extern const std::string INFO_SMALL_FONTFACE;
extern const std::string SCORE_FONTFACE;
extern const std::string APP_ICON;
extern const std::string BOARD_BACKGROUND;
extern const std::string GAMEOVER_BACKGROUND;
extern const std::string CARD_ELEMENTS;
extern const std::string CARD_FACES;
extern const std::string CARD_BACKGROUNDS;
extern const std::string INTERFACE_CURSOR;
extern const std::string CURSOR_MOVE;
extern const std::string CURSOR_CANCEL;
extern const std::string CURSOR_WRONG;
extern const std::string MUSIC_TRACK;
extern const std::string MUSIC_ALT_TRACK;
extern const std::string MUSIC_WIN_TRACK;

// Card elemental type
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

// Card level category
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

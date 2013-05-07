/******************************************************************************
    cfg.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CFG_HEADERS
#define GAMEAPP_CFG_HEADERS

#include <string>

#define DEBUG_TTCARDS // unused
#define DEBUG_TTCARDS_OBJ

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

extern const char APP_NAME[255];

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern const int BOARD_ORIGIN_X;
extern const int BOARD_ORIGIN_Y;

extern const int CARD_WIDTH;
extern const int CARD_HEIGHT;
extern const int BACKGROUND_WIDTH;
extern const int BACKGROUND_HEIGHT;
extern const int ELEMENT_WIDTH;
extern const int ELEMENT_HEIGHT;
extern const int CURSOR_WIDTH;
extern const int CURSOR_HEIGHT;

extern const int TOTAL_PLAYERS;
extern const int MAX_COLLECTION;
extern const int MAX_PLAYER_HAND;

extern const int PLAYER1_ORIGIN_X;
extern const int PLAYER1_ORIGIN_Y;

extern const int PLAYER2_ORIGIN_X;
extern const int PLAYER2_ORIGIN_Y;

extern const int PLAYER1_CURSOR_ORIGIN_X;
extern const int PLAYER1_CURSOR_ORIGIN_Y;

extern const int PLAYER2_CURSOR_ORIGIN_X;
extern const int PLAYER2_CURSOR_ORIGIN_Y;

extern const int CURSOR_ORIGIN_X;
extern const int CURSOR_ORIGIN_Y;

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

extern const int BOARD_GRID_WIDTH;
extern const int BOARD_GRID_HEIGHT;

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

extern const std::string WORKING_DIR;
extern const std::string CARDS_DB;
extern const std::string CARD_FONTFACE;
extern const std::string SCORE_FONTFACE;
extern const std::string APP_ICON;
extern const std::string BOARD_BACKGROUND;
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


#endif // GAMEAPP_CFG_HEADERS defined

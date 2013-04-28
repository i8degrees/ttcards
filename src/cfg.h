/******************************************************************************
    cfg.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CFG_HEADERS
#define GAMEAPP_CFG_HEADERS

#define DEBUG_TTCARDS // unused
#define DEBUG_TTCARDS_OBJ

#define DEBUG_BOARD
#define DEBUG_BOARD_CMP
#define DEBUG_BOARD_OBJ

#define DEBUG_PLAYER
#define DEBUG_PLAYER_OBJ

#define DEBUG_CARD_HAND
#define DEBUG_CARD_HAND_OBJ

#define DEBUG_CARD_DEBUG
//#define DEBUG_CARD_DEBUG_OBJ

#define DEBUG_CARD_VIEW
#define DEBUG_CARD_VIEW_OBJ

//#define DEBUG_CARD
//#define DEBUG_CARD_OBJ

//#define DEBUG_CARD_COLLECTION
//#define DEBUG_CARD_COLLECTION_OBJ

extern const char APP_NAME[255];
extern const char APP_ICON[255];

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern const int CARD_WIDTH;
extern const int CARD_HEIGHT;
extern const int ELEMENT_WIDTH;
extern const int ELEMENT_HEIGHT;

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

extern const int BOARD_ORIGIN_X;
extern const int BOARD_ORIGIN_Y;

extern const int BOARD_GRID_WIDTH;
extern const int BOARD_GRID_HEIGHT;
extern const char BOARD_BACKGROUND[255];

extern const char CARDS_DB[255];
extern const char CARD_FONTFACE[255];
extern const char SCORE_FONTFACE[255];

extern const char CARD_ELEMENTS[255];
extern const int ELEMENT_NONE;
extern const int ELEMENT_EARTH;
extern const int ELEMENT_FIRE;
extern const int ELEMENT_HOLY;
extern const int ELEMENT_ICE;
extern const int ELEMENT_POISON;
extern const int ELEMENT_THUNDER;
extern const int ELEMENT_WATER;
extern const int ELEMENT_WIND;

extern const char CARD_FACES[255];

extern const char CARD_BACKGROUNDS[255];
extern const int NOFACE_ID;
extern const int NOPLAYER_BACKGROUND_ID;
extern const int PLAYER1_BACKGROUND_ID;
extern const int PLAYER2_BACKGROUND_ID;

extern const char LEFT_CURSOR[255];
extern const char RIGHT_CURSOR[255];

extern const char CURSOR_MOVE[255];
extern const char CURSOR_CANCEL[255];
extern const char CURSOR_WRONG[255];
extern const char MUSIC_TRACK[255];
extern const char MUSIC_ALT_TRACK[255];

#endif // CFG_HEADERS defined
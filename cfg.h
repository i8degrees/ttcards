/******************************************************************************
    cfg.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CFG_HEADERS
#define CFG_HEADERS

#include "SDL/SDL.h"

#include <string>

#define DEBUG

extern const char APP_NAME[255];
extern const char APP_ICON[255];

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern const int CARD_WIDTH;
extern const int CARD_HEIGHT;
extern const int ELEMENT_WIDTH;
extern const int ELEMENT_HEIGHT;

extern const int MAX_DECKSET;
extern const int MAX_HAND;

extern const int PLAYER1_ORIGIN_X;
extern const int PLAYER1_ORIGIN_Y;

extern const int PLAYER2_ORIGIN_X;
extern const int PLAYER2_ORIGIN_Y;

extern const int BOARD_ORIGIN_X;
extern const int BOARD_ORIGIN_Y;

extern const int BOARD_GRID_WIDTH;
extern const int BOARD_GRID_HEIGHT;
extern const char BOARD_BACKGROUND[255];

extern const char CARDS_DB[255];
extern const char CARD_FONTFACE[255];
extern const char SCORE_FONTFACE[255];

extern const char ELEMENT_NONE[255];
extern const char ELEMENT_EARTH[255];
extern const char ELEMENT_FIRE[255];
extern const char ELEMENT_HOLY[255];
extern const char ELEMENT_ICE[255];
extern const char ELEMENT_POISON[255];
extern const char ELEMENT_THUNDER[255];
extern const char ELEMENT_WATER[255];
extern const char ELEMENT_WIND[255];

extern const char CARD_FACES[255];
extern const char HIDDEN_CARD[255];
extern const char PLAYER1_CARD[255];
extern const char PLAYER2_CARD[255];
extern const char NOPLAYER_CARD[255];

extern const char LEFT_CURSOR[255];
extern const char RIGHT_CURSOR[255];

extern const char CURSOR_MOVE[255];
extern const char CURSOR_CANCEL[255];
extern const char CURSOR_WRONG[255];
extern const char MUSIC_TRACK[255];
extern const char MUSIC_ALT_TRACK[255];

#endif // CFG_HEADERS defined

/******************************************************************************
    cfg.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CFG_HEADERS
#define CFG_HEADERS

#include <iostream>

#include "SDL.h"

extern const char APP_NAME[255];
extern const char APP_ICON[255];

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern const struct SDL_Color BLACK;
extern const struct SDL_Color WHITE;

extern const int CARD_WIDTH;
extern const int CARD_HEIGHT;
extern const int ELEMENT_WIDTH;
extern const int ELEMENT_HEIGHT;

extern const int MAX_DECKSET;
extern const int MAX_CARDSET;

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

extern const char ELEMENT_EARTH[255];
extern const char ELEMENT_FIRE[255];
extern const char ELEMENT_HOLY[255];
extern const char ELEMENT_ICE[255];
extern const char ELEMENT_POISON[255];
extern const char ELEMENT_THUNDER[255];
extern const char ELEMENT_WATER[255];
extern const char ELEMENT_WIND[255];

extern const char CARDFACE[255];
extern const char NO_CARDFACE[255];
extern const char PLAYER1_CARDFACE[255];
extern const char PLAYER2_CARDFACE[255];

extern const char CURSOR_MOVE[255];
extern const char MUSIC_TRACK[255];

#endif // CFG_HEADERS defined

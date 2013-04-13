/******************************************************************************
    cfg.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "cfg.h"

#include <string>

const char APP_NAME[255] = "TTcards";
const char APP_ICON[255] = "./data/images/ttcards.bmp";

const int SCREEN_WIDTH = 384;
const int SCREEN_HEIGHT = 224;
const int SCREEN_BPP = 32;

const struct SDL_Color BLACK = { 0, 0, 0 };
const struct SDL_Color WHITE = { 255, 255, 255 };

const int CARD_WIDTH = 64;
const int CARD_HEIGHT = 64;
const int ELEMENT_WIDTH = 16;
const int ELEMENT_HEIGHT = 16;

const int MAX_DECKSET = 110;
const int MAX_HAND = 5;

const int PLAYER1_ORIGIN_X = 16;
const int PLAYER1_ORIGIN_Y = 16;

const int PLAYER2_ORIGIN_X = 304;
const int PLAYER2_ORIGIN_Y = 16;

// top-left of 3x3 grid
const int BOARD_ORIGIN_X = 96;
const int BOARD_ORIGIN_Y = 18;

const int BOARD_GRID_WIDTH = 3;
const int BOARD_GRID_HEIGHT = 3;
const char BOARD_BACKGROUND[255] = "./data/images/board.bmp";

const char CARDS_DB[255] = "./data/cards.txt";
const char CARD_FONTFACE[255] = "./data/fonts/EnvyCodeRb.ttf";
const char SCORE_FONTFACE[255] = "./data/fonts/EnvyCodeRb.ttf";

const char ELEMENT_NONE[255] = "./data/images/elements/none_16.bmp";
const char ELEMENT_EARTH[255] = "./data/images/elements/earth_16.bmp";
const char ELEMENT_FIRE[255] = "./data/images/elements/fire_16.bmp";
const char ELEMENT_HOLY[255] = "./data/images/elements/holy_16.bmp";
const char ELEMENT_ICE[255] = "./data/images/elements/ice_16.bmp";
const char ELEMENT_POISON[255] = "./data/images/elements/poison_16.bmp";
const char ELEMENT_THUNDER[255] = "./data/images/elements/thunder_16.bmp";
const char ELEMENT_WATER[255] = "./data/images/elements/water_16.bmp";
const char ELEMENT_WIND[255] = "./data/images/elements/wind_16.bmp";

const char FACES_DIR[255] = "./data/images/faces/";
const char CARDFACE[255] = "./data/images/faces/face.bmp";
const char NO_CARDFACE[255] = "./data/images/faces/noface.bmp";
const char PLAYER1_CARDFACE[255] = "./data/images/player1.bmp";
const char PLAYER2_CARDFACE[255] = "./data/images/player2.bmp";
const char NOPLAYER_CARD[255] = "./data/images/noplayer.png";

const char LEFT_CURSOR[255] = "./data/images/left_cursor.png";
const char RIGHT_CURSOR[255] = "./data/images/right_cursor.png";

const char CURSOR_MOVE[255] = "./data/audio/cursor_move.wav";
const char CURSOR_CANCEL[255] = "./data/audio/cursor_cancel.wav";
const char CURSOR_WRONG[255] = "./data/audio/cursor_wrong.wav";
const char MUSIC_TRACK[255] = "./data/audio/15_ShuffleOrBoogie.ogg";
const char MUSIC_ALT_TRACK[255] = "./data/audio/06_ShuffleOrBoogie.ogg";

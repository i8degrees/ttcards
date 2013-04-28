/******************************************************************************
    cfg.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "cfg.h"

const char APP_NAME[255] = "TTcards";

const int SCREEN_WIDTH = 384;
const int SCREEN_HEIGHT = 224;
const int SCREEN_BPP = 32;

const int CARD_WIDTH = 64;
const int CARD_HEIGHT = 64;
const int BACKGROUND_WIDTH = CARD_WIDTH;
const int BACKGROUND_HEIGHT = CARD_HEIGHT;
const int ELEMENT_WIDTH = 16;
const int ELEMENT_HEIGHT = 16;
const int CURSOR_WIDTH = 26;
const int CURSOR_HEIGHT = 16;

const int TOTAL_PLAYERS = 2; // +1 padding
const int MAX_COLLECTION = 110; // number of cards in CARDS_DB
const int MAX_PLAYER_HAND = 5;

// top-left of board grid
const int BOARD_ORIGIN_X = 96;
const int BOARD_ORIGIN_Y = 16;

const int CURSOR_ORIGIN_X = SCREEN_WIDTH - ( CURSOR_WIDTH / 2 );
const int CURSOR_ORIGIN_Y = SCREEN_HEIGHT - ( CURSOR_HEIGHT / 2 );

const int PLAYER1_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 );
const int PLAYER1_ORIGIN_Y = BOARD_ORIGIN_Y;

const int PLAYER2_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 ) + BOARD_ORIGIN_X + ( CARD_WIDTH * 3 );
const int PLAYER2_ORIGIN_Y = BOARD_ORIGIN_Y;

const int PLAYER1_CURSOR_ORIGIN_X = BOARD_ORIGIN_X;
const int PLAYER1_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 );

const int PLAYER2_CURSOR_ORIGIN_X = BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) - ( CURSOR_WIDTH / 2 ) - 16;
const int PLAYER2_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 );

const int CARD_ORIGIN_X = 0;
const int CARD_ORIGIN_Y = 0;

const int CARD_FACE_ORIGIN_X = CARD_ORIGIN_X;
const int CARD_FACE_ORIGIN_Y = CARD_ORIGIN_Y;

const int BACKGROUND_ORIGIN_X = CARD_ORIGIN_X;
const int BACKGROUND_ORIGIN_Y = CARD_ORIGIN_Y;

const int ELEMENT_ORIGIN_X = CARD_ORIGIN_X + 46;
const int ELEMENT_ORIGIN_Y = CARD_ORIGIN_Y + 4;

const int RANK_NORTH_ORIGIN_X = 8;
const int RANK_NORTH_ORIGIN_Y = 0;

const int RANK_EAST_ORIGIN_X = 12;
const int RANK_EAST_ORIGIN_Y = 8;

const int RANK_SOUTH_ORIGIN_X = 8;
const int RANK_SOUTH_ORIGIN_Y = 16;

const int RANK_WEST_ORIGIN_X = 4;
const int RANK_WEST_ORIGIN_Y = 8;

const int CARD_ID_ORIGIN_X = 40;
const int CARD_ID_ORIGIN_Y = 0;

const int BOARD_GRID_WIDTH = 3;
const int BOARD_GRID_HEIGHT = 3;

const int ELEMENT_NONE = 0;
const int ELEMENT_EARTH = 1;
const int ELEMENT_FIRE = 2;
const int ELEMENT_HOLY = 3;
const int ELEMENT_ICE = 4;
const int ELEMENT_POISON = 5;
const int ELEMENT_THUNDER = 6;
const int ELEMENT_WATER = 7;
const int ELEMENT_WIND = 8;

const int NOFACE_ID = 0;
const int NOPLAYER_BACKGROUND_ID = 1;
const int PLAYER1_BACKGROUND_ID = 2;
const int PLAYER2_BACKGROUND_ID = 3;
const int INTERFACE_CURSOR_NONE = 0;
const int INTERFACE_CURSOR_LEFT = 1;
const int INTERFACE_CURSOR_RIGHT = 2;

const char CARDS_DB[255] = "./data/cards.txt";
const char CARD_FONTFACE[255] = "./data/fonts/EnvyCodeRb.ttf";
const char SCORE_FONTFACE[255] = "./data/fonts/EnvyCodeRb.ttf";
const char APP_ICON[255] = "./data/images/ttcards.bmp";
const char BOARD_BACKGROUND[255] = "./data/images/board.png";
const char CARD_ELEMENTS[255] = "./data/images/elements.png";
const char CARD_FACES[255] = "./data/images/faces.png";
const char CARD_BACKGROUNDS[255] = "./data/images/backgrounds.png";
const char INTERFACE_CURSOR[255] = "./data/images/cursors.png";
const char CURSOR_MOVE[255] = "./data/audio/cursor_move.wav";
const char CURSOR_CANCEL[255] = "./data/audio/cursor_cancel.wav";
const char CURSOR_WRONG[255] = "./data/audio/cursor_wrong.wav";
const char MUSIC_TRACK[255] = "./data/audio/15_ShuffleOrBoogie.ogg";
const char MUSIC_ALT_TRACK[255] = "./data/audio/06_ShuffleOrBoogie.ogg";

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

const int PLAYER1_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 ); // 16
const int PLAYER1_ORIGIN_Y = BOARD_ORIGIN_Y; // 16

const int PLAYER2_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 ) + BOARD_ORIGIN_X + ( CARD_WIDTH * 3 );
const int PLAYER2_ORIGIN_Y = BOARD_ORIGIN_Y;

const int CURSOR_ORIGIN_X = ( SCREEN_WIDTH - CURSOR_WIDTH ) / 2 ;
const int CURSOR_ORIGIN_Y = ( SCREEN_HEIGHT - CURSOR_HEIGHT ) / 2;

const int PLAYER1_CURSOR_ORIGIN_X = BOARD_ORIGIN_X - PLAYER1_ORIGIN_X; // 80
const int PLAYER1_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 ); // 48

const int PLAYER2_CURSOR_ORIGIN_X = BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) - ( CURSOR_WIDTH / 2 ); // 275
const int PLAYER2_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 ); // 48

// SCREEN_HEIGHT - 48
// 64 * 5
const int PLAYER1_SCORE_ORIGIN_X = 32; // TODO
const int PLAYER1_SCORE_ORIGIN_Y = 176; // TODO

const int PLAYER2_SCORE_ORIGIN_X = 320; // TODO
const int PLAYER2_SCORE_ORIGIN_Y = PLAYER1_SCORE_ORIGIN_Y; // TODO

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

std::string WORKING_DIR;

const std::string CARDS_DB =            WORKING_DIR + "./data/cards.txt";
const std::string CARD_FONTFACE =       WORKING_DIR + "./data/fonts/EnvyCodeRb.ttf";
const std::string SCORE_FONTFACE =      WORKING_DIR + "./data/fonts/EnvyCodeRb.ttf";
const std::string APP_ICON =            WORKING_DIR + "./data/images/ttcards.bmp";
const std::string BOARD_BACKGROUND =    WORKING_DIR + "./data/images/board.png";
const std::string CARD_ELEMENTS =       WORKING_DIR + "./data/images/elements.png";
const std::string CARD_FACES =          WORKING_DIR + "./data/images/faces.png";
const std::string CARD_BACKGROUNDS =    WORKING_DIR + "./data/images/backgrounds.png";
const std::string INTERFACE_CURSOR =    WORKING_DIR + "./data/images/cursors.png";
const std::string CURSOR_MOVE =         WORKING_DIR + "./data/audio/cursor_move.wav";
const std::string CURSOR_CANCEL =       WORKING_DIR + "./data/audio/cursor_cancel.wav";
const std::string CURSOR_WRONG =        WORKING_DIR + "./data/audio/cursor_wrong.wav";
const std::string MUSIC_TRACK =         WORKING_DIR + "./data/audio/15_ShuffleOrBoogie.ogg";
const std::string MUSIC_ALT_TRACK =     WORKING_DIR + "./data/audio/06_ShuffleOrBoogie.ogg";
const std::string MUSIC_WIN_TRACK =     WORKING_DIR + "./data/audio/05_TheWinner.ogg";

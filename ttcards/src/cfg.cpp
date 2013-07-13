/******************************************************************************

    TTcards configuration

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "cfg.h"

std::string APP_NAME = "TTcards";

const int SCREEN_WIDTH = 384;
const int SCREEN_HEIGHT = 224;
const int SCREEN_BPP = 32;

const nom::uint32 TICKS_PER_SECOND = 15; // As per PSX_SCUS Final Fantasy VIII
const nom::uint32 SKIP_TICKS = 1000 / TICKS_PER_SECOND; // TODO: calculation used to determine how many ticks to wait between updating
const nom::uint32 MAX_FRAMESKIP = 5; // TODO: Max amount of frames we can skip if the framerate takes a dip
const nom::uint32 FRAMES_PER_SECOND = 15; // TODO: Not implemented

const nom::int32 CARD_WIDTH = 64;
const nom::int32 CARD_HEIGHT = 64;
const int BACKGROUND_WIDTH = CARD_WIDTH;
const int BACKGROUND_HEIGHT = CARD_HEIGHT;
const int ELEMENT_WIDTH = 16;
const int ELEMENT_HEIGHT = 16;
const int CURSOR_WIDTH = 26;
const int CURSOR_HEIGHT = 16;
const int MENU_ELEMENT_WIDTH = 16;
const int MENU_ELEMENT_HEIGHT = 16;

const nom::uint32 TOTAL_PLAYERS = 2; // +1 padding
const int MAX_COLLECTION = 110; // number of cards in CARDS_DB
const nom::int32 MAX_PLAYER_HAND = 5;

// top-left of board grid
const nom::int32 BOARD_ORIGIN_X = 96;
const nom::int32 BOARD_ORIGIN_Y = 16;

const int PLAYER2_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 ); // 16
const int PLAYER2_ORIGIN_Y = BOARD_ORIGIN_Y; // 16

const int PLAYER1_ORIGIN_X = ( BOARD_ORIGIN_X / 2 ) - ( CARD_WIDTH / 2 ) + BOARD_ORIGIN_X + ( CARD_WIDTH * 3 );
const int PLAYER1_ORIGIN_Y = BOARD_ORIGIN_Y;

const int CURSOR_ORIGIN_X = ( SCREEN_WIDTH - CURSOR_WIDTH ) / 2 ;
const int CURSOR_ORIGIN_Y = ( SCREEN_HEIGHT - CURSOR_HEIGHT ) / 2;

const int PLAYER2_CURSOR_ORIGIN_X = BOARD_ORIGIN_X - PLAYER2_ORIGIN_X; // 80
const int PLAYER2_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 ); // 48

const nom::int32 PLAYER1_CURSOR_ORIGIN_X = BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) - ( CURSOR_WIDTH / 2 ); // 275
const nom::int32 PLAYER1_CURSOR_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT / 2 ); // 48

// SCREEN_HEIGHT - 48
// 64 * 5
const int PLAYER1_SCORE_ORIGIN_X = 320; // TODO
const int PLAYER1_SCORE_ORIGIN_Y = 176; // TODO

const int PLAYER2_SCORE_ORIGIN_X = 32; // TODO
const int PLAYER2_SCORE_ORIGIN_Y = 176; // TODO

const int PLAYER1_GAMEOVER_ORIGIN_X = BOARD_ORIGIN_X - ( CARD_WIDTH ); // TODO: relocate
const int PLAYER1_GAMEOVER_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT ) + ( CARD_HEIGHT / 2 ); // TODO: relocate

const int PLAYER2_GAMEOVER_ORIGIN_X = BOARD_ORIGIN_X - ( CARD_WIDTH ); // TODO: relocate
const int PLAYER2_GAMEOVER_ORIGIN_Y = BOARD_ORIGIN_Y + ( CARD_HEIGHT )  + ( CARD_HEIGHT / 2 ) + ( CARD_HEIGHT / 2 ); // TODO: relocate

const int CARD_ORIGIN_X = 0;
const int CARD_ORIGIN_Y = 0;

const int CARD_FACE_ORIGIN_X = CARD_ORIGIN_X;
const int CARD_FACE_ORIGIN_Y = CARD_ORIGIN_Y;

const int BACKGROUND_ORIGIN_X = CARD_ORIGIN_X;
const int BACKGROUND_ORIGIN_Y = CARD_ORIGIN_Y;

const int ELEMENT_ORIGIN_X = CARD_ORIGIN_X + 42;
const int ELEMENT_ORIGIN_Y = CARD_ORIGIN_Y + 5;

const int RANK_NORTH_ORIGIN_X = 8;
const int RANK_NORTH_ORIGIN_Y = 3;

const int RANK_EAST_ORIGIN_X = 12;
const int RANK_EAST_ORIGIN_Y = 11;

const int RANK_SOUTH_ORIGIN_X = RANK_NORTH_ORIGIN_X;
const int RANK_SOUTH_ORIGIN_Y = 20;

const int RANK_WEST_ORIGIN_X = 4;
const int RANK_WEST_ORIGIN_Y = RANK_EAST_ORIGIN_Y;

const int CARD_ID_ORIGIN_X = 26;
const int CARD_ID_ORIGIN_Y = 4;

// interface_pickOutCards() Menu
const nom::int32 PICK_CARDS_MENU_ORIGIN_X = 60;
const nom::int32 PICK_CARDS_MENU_ORIGIN_Y = 25;

const nom::int32 PICK_CARDS_MENU_WIDTH = 164;
const nom::int32 PICK_CARDS_MENU_HEIGHT = 196;

// Starting cursor positioning
const nom::int32 MENU_CARDS_CURSOR_ORIGIN_X = 40;
const nom::int32 MENU_CARDS_CURSOR_ORIGIN_Y = 34; // 20

// "Cards" Title Text
const int MENU_CARDS_TITLE_ORIGIN_X = 64;
const int MENU_CARDS_TITLE_ORIGIN_Y = PICK_CARDS_MENU_ORIGIN_Y;

// "P" (page) number header text
const int MENU_CARDS_TITLE_PAGE_ORIGIN_X = 119;
const int MENU_CARDS_TITLE_PAGE_ORIGIN_Y = PICK_CARDS_MENU_ORIGIN_Y;

// "Num" header text
const int MENU_CARDS_TITLE_NUM_ORIGIN_X = 184;
const int MENU_CARDS_TITLE_NUM_ORIGIN_Y = PICK_CARDS_MENU_ORIGIN_Y;

// Field elements
const int MENU_CARDS_FIELD_ORIGIN_X = 64;
const int MENU_CARDS_FIELD_ORIGIN_Y = 30;

// Card selection helper element
const int MENU_CARDS_HELPER_ORIGIN_X = MENU_CARDS_FIELD_ORIGIN_X;
const int MENU_CARDS_HELPER_ORIGIN_Y = MENU_CARDS_FIELD_ORIGIN_Y;

// Card name text
const int MENU_CARDS_NAME_ORIGIN_X = 80;
const int MENU_CARDS_NAME_ORIGIN_Y = MENU_CARDS_FIELD_ORIGIN_Y;

// Number of cards text
const int MENU_CARDS_NUM_ORIGIN_X = 210;
const int MENU_CARDS_NUM_ORIGIN_Y = MENU_CARDS_FIELD_ORIGIN_Y;

// Page left
const int MENU_CARDS_PAGE_LEFT_ORIGIN_X = 58;
const int MENU_CARDS_PAGE_LEFT_ORIGIN_Y = 208;

// Page right
const int MENU_CARDS_PAGE_RIGHT_ORIGIN_X = 210;
const int MENU_CARDS_PAGE_RIGHT_ORIGIN_Y = MENU_CARDS_PAGE_LEFT_ORIGIN_Y;

const nom::int32 BOARD_GRID_WIDTH = 3;
const nom::int32 BOARD_GRID_HEIGHT = 3;

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

const int INTERFACE_MENU_ELEMENT = 0;
const int INTERFACE_MENU_ELEMENT_USED = 1;
const int INTERFACE_MENU_ELEMENT_PAGE_LEFT = 2;
const int INTERFACE_MENU_ELEMENT_PAGE_RIGHT = 3;

const int NOPLAYER_ID = 0;
const int PLAYER1_ID = 1;
const int PLAYER2_ID = 2;
const int PLAYER_AI_ID = 4;

std::string WORKING_DIR; // See main.cpp

const std::string CARDS_DB =            WORKING_DIR + "data/cards.json";
const std::string MENU_ELEMENTS =       WORKING_DIR + "data/images/menu_elements.png";
const std::string CARD_FONTFACE =       WORKING_DIR + "data/fonts/VIII_cards.png";
const std::string INFO_FONTFACE =       WORKING_DIR + "data/fonts/VIII.png";
const std::string INFO_SMALL_FONTFACE = WORKING_DIR + "data/fonts/VIII_small.png";
const std::string SCORE_FONTFACE =      WORKING_DIR + "data/fonts/EnvyCodeRb.ttf";
const std::string APP_ICON =            WORKING_DIR + "data/icon.bmp";
const std::string BOARD_BACKGROUND =    WORKING_DIR + "data/images/board.png";
const std::string GAMEOVER_BACKGROUND = WORKING_DIR + "data/images/game-over_background.png";
const std::string CARD_ELEMENTS =       WORKING_DIR + "data/images/elements.png";
const std::string CARD_FACES =          WORKING_DIR + "data/images/faces.png";
const std::string CARD_BACKGROUNDS =    WORKING_DIR + "data/images/backgrounds.png";
const std::string INTERFACE_CURSOR =    WORKING_DIR + "data/images/cursors.png";
const std::string CURSOR_MOVE =         WORKING_DIR + "data/audio/cursor_move.wav";
const std::string CURSOR_CANCEL =       WORKING_DIR + "data/audio/cursor_cancel.wav";
const std::string CURSOR_WRONG =        WORKING_DIR + "data/audio/cursor_wrong.wav";
const std::string CARD_FLIP =           WORKING_DIR + "data/audio/card_flip.wav";
const std::string CARD_PLACE =          WORKING_DIR + "data/audio/card_place.wav";
const std::string MUSIC_TRACK =         WORKING_DIR + "data/audio/15_ShuffleOrBoogie.ogg";
const std::string MUSIC_ALT_TRACK =     WORKING_DIR + "data/audio/06_ShuffleOrBoogie.ogg";
const std::string MUSIC_WIN_TRACK =     WORKING_DIR + "data/audio/05_TheWinner.ogg";

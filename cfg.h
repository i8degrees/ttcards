/******************************************************************************
    cfg.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CFG_HEADERS
#define CFG_HEADERS

const int SCREEN_WIDTH = 384;
const int SCREEN_HEIGHT = 224;
const int SCREEN_COLORBIT = 32;

const int CARD_WIDTH = 64;
const int CARD_HEIGHT = 64;
const int MAX_DECKSET = 110; // +1
const int MAX_CARDSET = 5;

const char BOARD_BACKGROUND[255] = "./data/images/board.bmp";
const char CARD_FONTFACE[255] = "./data/fonts/EnvyCodeRb.ttf";
const char CARDS_DB[255] = "./data/cards.txt";

const char ELEMENT_EARTH[255] = "./data/images/elements/earth_16.bmp";
const char ELEMENT_FIRE[255] = "./data/images/elements/fire_16.bmp";
const char ELEMENT_HOLY[255] = "./data/images/elements/holy_16.bmp";
const char ELEMENT_ICE[255] = "./data/images/elements/ice_16.bmp";
const char ELEMENT_POISON[255] = "./data/images/elements/poison_16.bmp";
const char ELEMENT_THUNDER[255] = "./data/images/elements/thunder_16.bmp";
const char ELEMENT_WATER[255] = "./data/images/elements/water_16.bmp";
const char ELEMENT_WIND[255] = "./data/images/elements/wind_16.bmp";

const char CARDFACE[255] = "./data/images/faces/face.bmp";
const char NO_CARDFACE[255] = "./data/images/faces/noface.bmp";
const char PLAYER1_CARDFACE[255] = "./data/images/player1.bmp";
const char PLAYER2_CARDFACE[255] = "./data/images/player2.bmp";

const char MUSIC_TRACK[255] = "./data/audio/15_ShuffleOrBoogie.mp3";

#endif // CFG_HEADERS

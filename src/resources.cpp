/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "resources.hpp"

// Resource configuration
const nom::Path path;
std::string WORKING_DIR;

const std::string SCORE_FONTFACE =      WORKING_DIR + "fonts/EnvyCodeRb.ttf";
const std::string GAMEOVER_FONTFACE =   WORKING_DIR + "fonts/EnvyCodeRb.ttf";
const std::string CARD_FONTFACE =       WORKING_DIR + "fonts/VIII_cards.png";
const std::string INFO_FONTFACE =       WORKING_DIR + "fonts/VIII.png";
const std::string INFO_SMALL_FONTFACE = WORKING_DIR + "fonts/VIII_small.png";

const std::string BOARD_BACKGROUND =    WORKING_DIR + "images/board.png";
const std::string GAMEOVER_BACKGROUND = WORKING_DIR + "images/game-over_background.png";
const std::string CARD_ELEMENTS =       WORKING_DIR + "images/elements.png";
const std::string CARD_FACES =          WORKING_DIR + "images/faces.png";
const std::string CARD_BACKGROUNDS =    WORKING_DIR + "images/backgrounds.png";
const std::string INTERFACE_CURSOR =    WORKING_DIR + "images/cursors.png";
const std::string MENU_ELEMENTS =       WORKING_DIR + "images/menu_elements.png";

const std::string CURSOR_MOVE =         WORKING_DIR + "audio" + path.native() + "cursor_move.wav";
const std::string CURSOR_CANCEL =       WORKING_DIR + "audio" + path.native() + "cursor_cancel.wav";
const std::string CURSOR_WRONG =        WORKING_DIR + "audio" + path.native() + "cursor_wrong.wav";
const std::string CARD_FLIP =           WORKING_DIR + "audio" + path.native() + "card_flip.wav";
const std::string CARD_PLACE =          WORKING_DIR + "audio" + path.native() + "card_place.wav";
const std::string SFX_LOAD_GAME =       WORKING_DIR + "audio" + path.native() + "load_game.wav";
const std::string SFX_SAVE_GAME =       WORKING_DIR + "audio" + path.native() + "save_game.wav";
const std::string MUSIC_TRACK =         WORKING_DIR + "audio" + path.native() + "ShuffleOrBoogie.ogg";
const std::string MUSIC_WINNING_TRACK = WORKING_DIR + "audio" + path.native() + "TheWinner.ogg";

const std::string APP_ICON =            WORKING_DIR + "icon.bmp";
const std::string CARDS_DB =            WORKING_DIR + "cards.json";

const std::string USER_PLAYER1_FILENAME = TTCARDS_DATA_DIR + path.native() + "player1.json";
const std::string USER_PLAYER2_FILENAME = TTCARDS_DATA_DIR + path.native() + "player2.json";
const std::string USER_BOARD_FILENAME =   TTCARDS_DATA_DIR + path.native() + "board.json";

const std::string LOADING_TEXT = APP_NAME + " - " + "Loading...";

#ifdef DEBUG
  const std::string SHORT_VERSION_INFO = APP_NAME  + " " + "v" + std::to_string ( TTCARDS_VERSION_MAJOR ) + "." + std::to_string ( TTCARDS_VERSION_MINOR ) + "." + std::to_string ( TTCARDS_VERSION_PATCH ) + "-Debug";
#else
  const std::string SHORT_VERSION_INFO = APP_NAME  + " " + "v" + std::to_string ( TTCARDS_VERSION_MAJOR ) + "." + std::to_string ( TTCARDS_VERSION_MINOR ) + "." + std::to_string ( TTCARDS_VERSION_PATCH );
#endif

/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#ifndef TTCARDS_RESOURCES_HPP
#define TTCARDS_RESOURCES_HPP

#include <string>
#include <nomlib/system/Path.hpp>

const nom::Path p;
const nom::File fp;

namespace tt {

const std::string APP_NAME = "TTcards";
const std::string APP_WINDOW_TITLE = "Triple Triad";

// Data folder for game storage -- configuration, screen-shots, saved games and
// so forth.
const std::string TTCARDS_DATA_DIR =
  fp.user_app_support_path() + p.native() + APP_NAME;

// Base paths
const std::string TTCARDS_CONFIG_DIR = TTCARDS_DATA_DIR;

const std::string TTCARDS_SAVED_GAME_DIR =
  TTCARDS_DATA_DIR + p.native() + "SavedGames";

const std::string TTCARDS_SCREEN_SHOTS_DIR =
  TTCARDS_DATA_DIR + p.native() + "Screenshots";

// Absolute file paths

const std::string TTCARDS_PLAYER1_FILENAME =
  TTCARDS_SAVED_GAME_DIR + p.native() + "player1.json";

const std::string TTCARDS_PLAYER2_FILENAME =
  TTCARDS_SAVED_GAME_DIR + p.native() + "player2.json";

const std::string TTCARDS_BOARD_FILENAME =
  TTCARDS_SAVED_GAME_DIR + p.native() + "board.json";

const std::string TTCARDS_CONFIG_GAME_FILENAME =
  TTCARDS_CONFIG_DIR + p.native() + "config_game.json";

const std::string TTCARDS_CONFIG_ASSETS_LOW_RES_FILENAME =
  TTCARDS_CONFIG_DIR + p.native() + "config_assets-low-res.json";

const std::string TTCARDS_CONFIG_ASSETS_HI_RES_FILENAME =
  TTCARDS_CONFIG_DIR + p.native() + "config_assets-hi-res.json";

} // namespace tt

#endif // include guard defined

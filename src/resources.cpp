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

// Platform-dependent paths

const nom::Path path;

#if ! defined (NOM_PLATFORM_OSX)
  // Data folder for game storage (screenshots, card dumps, ...)
  const std::string TTCARDS_DATA_DIR = "@TTCARDS_DATA_DIR@";
#else
  const std::string TTCARDS_DATA_DIR = nom::user_documents_path() + path.native() + "ttcards";
#endif

const std::string USER_PLAYER1_FILENAME =   TTCARDS_DATA_DIR + path.native() + "player1.json";
const std::string USER_PLAYER2_FILENAME =   TTCARDS_DATA_DIR + path.native() + "player2.json";
const std::string USER_BOARD_FILENAME =     TTCARDS_DATA_DIR + path.native() + "board.json";
const std::string TTCARDS_CONFIG_FILENAME = TTCARDS_DATA_DIR + path.native() + "config.json";

// Localization strings

const std::string LOADING_TEXT = APP_NAME + " - " + "Loading...";

#ifdef DEBUG
  const std::string SHORT_VERSION_INFO = APP_NAME  + " " + "v" + std::to_string ( TTCARDS_VERSION_MAJOR ) + "." + std::to_string ( TTCARDS_VERSION_MINOR ) + "." + std::to_string ( TTCARDS_VERSION_PATCH ) + "-Debug";
#else
  const std::string SHORT_VERSION_INFO = APP_NAME  + " " + "v" + std::to_string ( TTCARDS_VERSION_MAJOR ) + "." + std::to_string ( TTCARDS_VERSION_MINOR ) + "." + std::to_string ( TTCARDS_VERSION_PATCH );
#endif

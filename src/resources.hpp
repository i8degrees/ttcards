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
#ifndef GAMEAPP_RESOURCES_HEADERS
#define GAMEAPP_RESOURCES_HEADERS

#include <string>

/// Cross-platform support files
#include <nomlib/types.hpp> // data types
#include <nomlib/system/Path.hpp> // pathnames

/// Site-specific constants -- this is likely specific to your own local system
/// setup; auto-generated at compile-time and therefore must recompile to modify
/// said constants.
#include "version.hpp"

/// Platform-independent interface for obtaining native pathnames
extern const nom::Path path;

/// Resources directory is run-time dependent; set in main.cpp
extern std::string WORKING_DIR;

// Fonts
extern const std::string SCORE_FONTFACE;
extern const std::string GAMEOVER_FONTFACE;
extern const std::string CARD_FONTFACE;
extern const std::string INFO_FONTFACE;
extern const std::string INFO_SMALL_FONTFACE;

// Static backgrounds & sprites
extern const std::string BOARD_BACKGROUND;
extern const std::string GAMEOVER_BACKGROUND;
extern const std::string CARD_ELEMENTS;
extern const std::string CARD_FACES;
extern const std::string CARD_BACKGROUNDS;
extern const std::string INTERFACE_CURSOR;
extern const std::string MENU_ELEMENTS;

// Audio Resources
extern const std::string CURSOR_MOVE;
extern const std::string CURSOR_CANCEL;
extern const std::string CURSOR_WRONG;
extern const std::string CARD_FLIP;
extern const std::string CARD_PLACE;
extern const std::string SFX_LOAD_GAME;
extern const std::string SFX_SAVE_GAME;
extern const std::string MUSIC_TRACK;
extern const std::string MUSIC_WINNING_TRACK;

/// Localization strings

// App data
extern const std::string APP_ICON;
extern const std::string CARDS_DB;

/// Default absolute file path of user game data
extern const std::string USER_PLAYER1_FILENAME;
extern const std::string USER_PLAYER2_FILENAME;
extern const std::string USER_BOARD_FILENAME;

extern const std::string LOADING_TEXT;
extern const std::string SHORT_VERSION_INFO;


#endif // include guard defined

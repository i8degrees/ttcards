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

/// Cross-platform support files
#include <nomlib/system.hpp>

/// Site-specific constants -- this is likely specific to your own local system
/// setup; auto-generated at compile-time and therefore must recompile to modify
/// said constants.
#include "version.hpp"

/// Platform-dependent paths

extern const nom::Path path;

// Data folder for game storage (screenshots, card dumps, ...)
extern const std::string TTCARDS_DATA_DIR;

extern const std::string USER_PLAYER1_FILENAME;
extern const std::string USER_PLAYER2_FILENAME;
extern const std::string USER_BOARD_FILENAME;

extern const std::string TTCARDS_CONFIG_GAME_FILENAME;
extern const std::string TTCARDS_CONFIG_ASSETS_LOW_RES_FILENAME;
extern const std::string TTCARDS_CONFIG_ASSETS_HI_RES_FILENAME;

/// Localization strings
extern const std::string LOADING_TEXT;
extern const std::string SHORT_VERSION_INFO;


#endif // include guard defined

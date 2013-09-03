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
#ifndef GAMEAPP_CARD_VIEW_HEADERS
#define GAMEAPP_CARD_VIEW_HEADERS

#include <iostream>
#include <string>

#include <nomlib/graphics.hpp>

#include "config.hpp"
#include "resources.hpp"
#include "GameConfig.hpp"
#include "Card.hpp"

class CardView
{
  public:
    /// Default constructor
    CardView ( void );

    /// Default destructor
    ~CardView ( void );

    /// Load the resources used in rendering a card
    ///
    /// The failure of any of these resources results in failure
    bool load ( GameConfig* config );

    bool drawFaceDown ( void* video_buffer, nom::int32 x, nom::int32 y );

    bool drawElement  (
                        void* video_buffer, nom::int32 element_id,
                        nom::int32 x, nom::int32 y
                      );

    bool DrawCard ( void* video_buffer, const Card& card, nom::int32 x, nom::int32 y );

  private:
    nom::BitmapFont card_text;
    nom::Sprite card_face;
    nom::Sprite card_background;
    nom::Sprite card_element;
};

#endif // GAMEAPP_CARD_VIEW_HEADERS defined

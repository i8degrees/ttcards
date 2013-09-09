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

class CardView:
                public nom::IDrawable
{
  public:
    typedef std::vector<std::shared_ptr<nom::IDrawable>> DrawableList;
    /// Default constructor
    CardView ( void );

    CardView ( const nom::Coords& coords );

    /// Default destructor
    ~CardView ( void );

    /// Load the resources used in rendering a card
    ///
    /// The failure of any of these resources results in failure
    bool load ( GameConfig* config );

    void draw (
                void* video_buffer, const Card& card,
                nom::int32 x, nom::int32 y, bool face_down = false
              ) const;

    void draw_face_down ( void* video_buffer, nom::int32 x, nom::int32 y ) const;

    void draw_background  (
                            void* video_buffer, nom::int32 player_id,
                            nom::int32 x, nom::int32 y
                          ) const;

    void draw_face  (
                      void* video_buffer, nom::int32 face_id,
                      nom::int32 x, nom::int32 y
                    ) const;

    void draw_element (
                        void* video_buffer, nom::int32 element_id,
                        nom::int32 x, nom::int32 y
                      ) const;

    void draw_text  (
                      void* video_buffer, nom::int32 rank,
                      nom::int32 x, nom::int32 y
                    ) const;

    void setViewCard ( const Card& card );
    void reposition ( const nom::Coords& coords );
    void face ( bool up = false );

    void Update ( void );
    void Draw ( void* ) const;

  private:
    nom::Coords position;
    Card render_card;
    bool card_face_down;

    std::shared_ptr<nom::BitmapFont> card_text;
    std::shared_ptr<nom::Sprite> card_face;
    std::shared_ptr<nom::Sprite> card_background;
    std::shared_ptr<nom::Sprite> card_element;

    DrawableList card;
};

#endif // GAMEAPP_CARD_VIEW_HEADERS defined

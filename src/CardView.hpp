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
#include <memory>

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

    /// Render the complete card -- its background, face, element and text.
    void draw (
                SDL_Renderer* target, const Card& card,
                nom::int32 x, nom::int32 y, bool face_down = false
              ) const;

    /// Render the card's face turned away from the player (face down)
    void draw_face_down ( SDL_Renderer* target, nom::int32 x, nom::int32 y ) const;

    /// Render the card's background color
    void draw_background  (
                            SDL_Renderer* target, nom::int32 player_id,
                            nom::int32 x, nom::int32 y
                          ) const;

    /// Render the card's face
    void draw_face  (
                      SDL_Renderer* target, nom::int32 face_id,
                      nom::int32 x, nom::int32 y
                    ) const;

    /// Render the card's element
    void draw_element (
                        SDL_Renderer* target, nom::int32 element_id,
                        nom::int32 x, nom::int32 y
                      ) const;

    /// Render the card's ranks (North, East, South, West)
    void draw_text  (
                      SDL_Renderer* target, nom::int32 rank,
                      nom::int32 x, nom::int32 y
                    ) const;

    /// Set a new card up for rendering
    void setViewCard ( const Card& card );

    /// Set new rendering coordinates for the card
    void reposition ( const nom::Coords& coords );

    /// Render the face of the card either shown to the player or not
    void face ( bool up = false );

    /// Refresh the renderer (this should be done anytime after you set new
    /// values within this object's instance.
    void update ( void );

    /// Render the card.
    void draw ( SDL_Renderer* target ) const;

  private:
    nom::Coords position;
    Card render_card;
    bool card_face_down;

    DrawableList card;
    std::shared_ptr<nom::BitmapFont> card_text;
    std::shared_ptr<nom::SpriteBatch> card_face;
    std::shared_ptr<nom::SpriteBatch> card_element;
    std::shared_ptr<nom::Gradient> card_background;
};

#endif // GAMEAPP_CARD_VIEW_HEADERS defined

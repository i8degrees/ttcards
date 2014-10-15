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

// class CardView: public nom::IDrawable
class CardView
{
  public:
    typedef std::vector<std::shared_ptr<nom::IDrawable>> DrawableList;

    /// Default constructor
    CardView ( void );

    CardView ( const nom::IntRect& coords );

    /// Default destructor
    virtual ~CardView ( void );

    /// Load the resources used in rendering a card
    ///
    /// The failure of any of these resources results in failure
    bool load ( const GameConfig* config, const nom::Font& card_font );

    /// Render the complete card -- its background, face, element and text.
    void draw (
                nom::IDrawable::RenderTarget& target, const Card& card,
                nom::int32 x, nom::int32 y, bool face_down = false
              ) const;

    /// Render the card's face turned away from the player (face down)
    void draw_face_down ( nom::IDrawable::RenderTarget& target, nom::int32 x, nom::int32 y ) const;

    /// Render the card's background color
    void draw_background  (
                            nom::IDrawable::RenderTarget& target, nom::int32 player_id,
                            nom::int32 x, nom::int32 y
                          ) const;

    /// Render the card's face
    void draw_face  (
                      nom::IDrawable::RenderTarget& target, nom::int32 face_id,
                      nom::int32 x, nom::int32 y
                    ) const;

    /// Render the card's element
    void draw_element (
                        nom::IDrawable::RenderTarget& target, nom::int32 element_id,
                        nom::int32 x, nom::int32 y
                      ) const;

    /// Render the card's ranks (North, East, South, West)
    void draw_text  (
                      nom::IDrawable::RenderTarget& target, nom::int32 rank,
                      nom::int32 x, nom::int32 y
                    ) const;

    /// Set a new card up for rendering
    void setViewCard ( const Card& card );

    /// Set new rendering coordinates for the card
    void reposition ( const nom::Point2i& pos );

    /// Render the face of the card either shown to the player or not
    void face ( bool up = false );

    /// Refresh the renderer (this should be done anytime after you set new
    /// values within this object's instance.
    void update ( void );

    /// Render the card.
    void draw ( nom::IDrawable::RenderTarget& target ) const;

  private:
    nom::Point2i position_;
    Card render_card;
    bool card_face_down;

    DrawableList card; // TODO: Rename to cards
    std::shared_ptr<nom::Text> card_text;
    std::shared_ptr<nom::SpriteBatch> card_face;
    std::shared_ptr<nom::SpriteBatch> card_element;
    std::shared_ptr<nom::Gradient> card_background;
};

#endif // GAMEAPP_CARD_VIEW_HEADERS defined

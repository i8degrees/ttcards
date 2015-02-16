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
#include "CardRenderer.hpp"

// Forward declarations
#include "Card.hpp"
#include "CardResourceLoader.hpp"

// Private headers
#include "helpers.hpp"

using namespace nom;
using namespace ttcards;

// Static initializations
// std::unique_ptr<nom::Sprite> CardRenderer::INVALID_CARD;
// std::unique_ptr<nom::Sprite> CardRenderer::FACE_DOWN_CARD;

CardRenderer::CardRenderer()
{
  //
}

CardRenderer::~CardRenderer()
{
  //
}

CardRenderer::CardRenderer(std::unique_ptr<nom::Sprite> sprite)
{
  this->rendered_card_ = std::move(sprite);
}

bool CardRenderer::valid() const
{
  return( this->rendered_card_ != nullptr &&
          this->rendered_card_->valid() != false );
}

std::unique_ptr<nom::Sprite> CardRenderer::rendered_card()
{
  return( std::move(this->rendered_card_) );
}

const Point2i& CardRenderer::position() const
{
  return this->position_;
}

const Size2i& CardRenderer::size() const
{
  return this->size_;
}

void CardRenderer::set_position(const Point2i& pos)
{
  this->position_ = pos;

  if( this->valid() == true ) {
    this->rendered_card_->set_position(pos);
  }
}

void CardRenderer::set_size(const Size2i& dims)
{
  this->size_ = dims;

  if( this->valid() == true ) {
    this->rendered_card_->set_size(dims);
  }
}

void CardRenderer::set_rendered_card(std::unique_ptr<nom::Sprite> sprite)
{
  this->rendered_card_ = std::move(sprite);
}

bool CardRenderer::render(nom::RenderTarget& target) const
{
  if( this->valid() == true ) {

    this->rendered_card_->draw(target);
    return true;
  }

  return false;
}

CardRenderer* create_placeholder_card_renderer()
{
  Point2i placeholder_pos(0, 0);
  Size2i placeholder_dims(CARD_WIDTH, CARD_HEIGHT);
  IntRect placeholder_bounds(placeholder_pos, placeholder_dims);

  auto rect =
    std::make_shared<nom::Rectangle>(placeholder_bounds, Color4i::Magenta);
  NOM_ASSERT(rect != nullptr);

  auto texture =
    std::shared_ptr<Texture>( rect->texture() );
  NOM_ASSERT(texture != nullptr);

  auto card_sprite =
    nom::make_unique<Sprite>(texture);
  NOM_ASSERT(card_sprite != nullptr);

  return( new CardRenderer( std::move(card_sprite) ) );
}

CardRenderer*
create_card_renderer(const CardResourceLoader* res, const Card& card)
{
  uint32 player_id = card.getPlayerID();

  auto texture = std::make_shared<Texture>();
  NOM_ASSERT(texture != nullptr);

  Size2i texture_dims;
  texture_dims.w = CARD_WIDTH;
  texture_dims.h = CARD_HEIGHT;

  RenderWindow* context = nom::render_interface();
  NOM_ASSERT(context != nullptr);
  if( context == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION, "Could not update cache",
                  "invalid renderer." );
    return nullptr;
  }

  // Obtain the optimal pixel format for the platform
  RendererInfo caps = context->caps();

  if( texture->initialize( caps.optimal_texture_format(),
      SDL_TEXTUREACCESS_TARGET, texture_dims ) == false )
  {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update cache: failed texture creation." );
    return nullptr;
  }

  // Use an alpha channel; otherwise the text is rendered on a black
  // background!
  texture->set_blend_mode(SDL_BLENDMODE_BLEND);

  // Set the destination (screen) positioning of the card
  texture->set_position( Point2i(0,0) );

  if( context->set_render_target( texture.get() ) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update cache: render targets not supported." );
    return nullptr;
  }

  // Clear the rendering backdrop color to be fully transparent; this preserves
  // any existing alpha channel data from the rendered text
  if( context->fill(Color4i::Transparent) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update cache:",
                  "failed to set the render target's color." );
    return nullptr;
  }

  if( card == Card::null || card.face_down() == true ) {
    render_card_face( NOFACE_ID, Point2i::zero, res->card_faces_.get(),
                      *context, texture.get() );
  } else {
    render_card_background( player_id, Point2i::zero,
                            res->card_backgrounds_[player_id].get(), *context,
                            texture.get() );

    render_card_face( card.getID(), Point2i::zero, res->card_faces_.get(),
                      *context, texture.get() );

    render_card_element(  card.getElement(), ELEMENT_ORIGIN,
                          res->card_elements_.get(), *context, texture.get() );

    render_card_text( card.getNorthRank(), RANK_NORTH_ORIGIN,
                      res->card_text_.get(), *context, texture.get() );

    render_card_text( card.getEastRank(), RANK_EAST_ORIGIN,
                      res->card_text_.get(), *context, texture.get() );

    render_card_text( card.getWestRank(), RANK_WEST_ORIGIN,
                      res->card_text_.get(), *context, texture.get() );

    render_card_text( card.getSouthRank(), RANK_SOUTH_ORIGIN,
                      res->card_text_.get(), *context, texture.get() );
  }

  if( context->reset_render_target() == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not render card:",
                  "failed to reset the rendering target." );
    return nullptr;
  }

  auto card_sprite =
    nom::make_unique<Sprite>(texture);

  return( new CardRenderer( std::move(card_sprite) ) );
}

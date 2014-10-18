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
#include "CardView.hpp"

using namespace nom;

CardView::CardView ( void )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );

  this->render_card = Card();

  this->card_background =
    std::make_shared<nom::Gradient>( nom::Gradient() );

  this->card_text =
    std::make_shared<nom::Text>( nom::Text() );

  this->card.push_back( this->card_background );
  this->card.push_back( this->card_text );

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    this->card_background->set_size( Size2i(CARD_WIDTH-2, CARD_HEIGHT-2) );
    this->card_background->set_margins( Point2i(1,1) );
  #else
    this->card_background->set_size( Size2i(CARD_WIDTH-4, CARD_HEIGHT-4) );
    this->card_background->set_margins( Point2i(2,2) );
  #endif

  this->card_background->set_fill_direction( nom::Gradient::FillDirection::Top );
}

CardView::CardView ( const nom::IntRect& coords )
{
  // Stub
}

CardView::~CardView ( void )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

bool CardView::load ( const GameConfig* config, const nom::Font& card_font )
{
  // Any file resources must be initialized *after* construction of this class;
  // our working directory where we load resources from is not set at the time
  // of CardView construction.
  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    this->card_face =
      std::make_shared<nom::SpriteBatch>( nom::SpriteBatch ( config->getString("CARD_FACES_ATLAS") ) );
  #else
    this->card_face =
      std::make_shared<nom::SpriteBatch>( nom::SpriteBatch ( config->getString("CARD_FACES_ATLAS_SCALE2X") ) );
  #endif

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    this->card_element =
      std::make_shared<nom::SpriteBatch>( nom::SpriteBatch( config->getString("CARD_ELEMENTS_ATLAS") ) );
  #else
    this->card_element =
      std::make_shared<nom::SpriteBatch>( nom::SpriteBatch( config->getString("CARD_ELEMENTS_ATLAS_SCALE2X") ) );
  #endif

  this->card.push_back(this->card_face);

  this->card.push_back(this->card_element);

  if ( config == nullptr ) return false;

  if ( card_font.valid() )
  {
    this->card_text->set_font( card_font );
  }
  else
  {
    NOM_LOG_ERR ( TTCARDS, "Could not assign card text font" );
    return false;
  }

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->card_face->load( config->getString("CARD_FACES"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + config->getString("CARD_FACES") );
      return false;
    }
  #else
    if( this->card_face->load( config->getString("CARD_FACES_SCALE2X"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + config->getString("CARD_FACES_SCALE2X") );
      return false;
    }
  #endif

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->card_element->load( config->getString("CARD_ELEMENTS"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + config->getString("CARD_ELEMENTS") );
      return false;
    }
  #else
    if( this->card_element->load( config->getString("CARD_ELEMENTS_SCALE2X"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + config->getString("CARD_ELEMENTS_SCALE2X") );
      return false;
    }
  #endif

  return true;
}

void CardView::draw_background  (
                                  nom::IDrawable::RenderTarget& target, nom::int32 player_id,
                                  nom::int32 x, nom::int32 y
                                ) const
{

  switch ( player_id )
  {
    case Card::NOPLAYER:
    default:
    {
      nom::Color4iColors player0_grad_bg = {
                                                nom::Color4i( 197, 197, 197 ),
                                                nom::Color4i( 84, 84, 84 )
                                              };

      this->card_background->set_colors( player0_grad_bg );
      break;
    }

    case Card::PLAYER1:
    {
      nom::Color4iColors player1_grad_bg = {
                                                nom::Color4i( 208, 223, 255 ),
                                                nom::Color4i( 50, 59, 114 )
                                              };

      this->card_background->set_colors( player1_grad_bg );
      break;
    }

    case Card::PLAYER2:
    {
      nom::Color4iColors player2_grad_bg = {
                                                nom::Color4i( 251, 222, 232 ),
                                                nom::Color4i( 114, 66, 66 )
                                              };

      this->card_background->set_colors( player2_grad_bg );
      break;
    }
  }

  this->card_background->set_position( nom::Point2i(x, y) );
  this->card_background->draw( target );
}

void CardView::draw_face  (
                            nom::IDrawable::RenderTarget& target, nom::int32 face_id,
                            nom::int32 x, nom::int32 y
                          ) const
{
  this->card_face->set_frame ( face_id );

  this->card_face->set_position ( nom::Point2i ( x, y ) );
  this->card_face->draw ( target );
}

void CardView::draw_element (
                              nom::IDrawable::RenderTarget& target, nom::int32 element_id,
                              nom::int32 x, nom::int32 y
                            ) const
{
  switch ( element_id )
  {
    default:
    case ::NONE: this->card_element->set_frame ( ELEMENT_NONE ); break;
    case EARTH: this->card_element->set_frame ( ELEMENT_EARTH ); break;
    case FIRE: this->card_element->set_frame ( ELEMENT_FIRE ); break;
    case HOLY: this->card_element->set_frame ( ELEMENT_HOLY ); break;
    case ICE: this->card_element->set_frame ( ELEMENT_ICE ); break;
    case POISON: this->card_element->set_frame ( ELEMENT_POISON ); break;
    case THUNDER: this->card_element->set_frame ( ELEMENT_THUNDER ); break;
    case WATER: this->card_element->set_frame ( ELEMENT_WATER ); break;
    case WIND: this->card_element->set_frame ( ELEMENT_WIND ); break;
  }

  this->card_element->set_position ( nom::Point2i ( x, y ) );
  this->card_element->draw ( target );
}

void CardView::draw_text  (
                            nom::IDrawable::RenderTarget& target, nom::int32 rank,
                            nom::int32 x, nom::int32 y
                          ) const
{
  if ( rank == 10 )
  {
    this->card_text->set_text ( "A" );
  }
  else
  {
    this->card_text->set_text ( std::to_string ( rank ) );
  }

  this->card_text->set_position ( nom::Point2i ( x, y ) );
  this->card_text->draw ( target );
}

void CardView::draw(  nom::IDrawable::RenderTarget& target, const Card& card,
                      nom::int32 x, nom::int32 y ) const
{
  if( card.face_down() == true ) {
    this->card_face->set_frame(NOFACE_ID);
    this->card_face->set_position( Point2i(x, y) );
    this->card_face->draw(target);
    return;
  }

  this->draw_background ( target, card.getPlayerID(), x, y );

  this->draw_face ( target, card.getID(), x, y );

  this->draw_element  (
                        target, card.getElement(),
                        ELEMENT_ORIGIN_X + x, ELEMENT_ORIGIN_Y + y
                      );

  this->draw_text (
                    target, card.getNorthRank(),
                    RANK_NORTH_ORIGIN_X + x, RANK_NORTH_ORIGIN_Y + y
                  );

  this->draw_text (
                    target, card.getEastRank(),
                    RANK_EAST_ORIGIN_X + x, RANK_EAST_ORIGIN_Y + y
                  );

  this->draw_text (
                    target, card.getWestRank(),
                    RANK_WEST_ORIGIN_X + x, RANK_WEST_ORIGIN_Y + y
                  );

  this->draw_text (
                    target, card.getSouthRank(),
                    RANK_SOUTH_ORIGIN_X + x, RANK_SOUTH_ORIGIN_Y + y
                  );

}

void CardView::setViewCard ( const Card& card )
{
  this->render_card = card;
}

void CardView::reposition ( const nom::Point2i& pos )
{
  this->position_ = pos;
}

void CardView::update ( void )
{
  //this->card_background->set_position ( this->position_ );
  //this->card_face->setPosition ( this->position_ );
  //this->card_element->setPosition ( this->position_ );
  //this->card_text->setPosition ( this->position_ );

  //for ( DrawableList::const_iterator it = card.begin(); it != card.end(); ++it )
  //{
    //std::shared_ptr<nom::IDrawable> obj = *it;
    //obj->update();
  //}
}

void CardView::draw ( nom::IDrawable::RenderTarget& target ) const
{
  //for ( DrawableList::const_iterator it = card.begin(); it != card.end(); ++it )
  //{
    //std::shared_ptr<nom::IDrawable> obj = *it;
    //this->draw_background ( target, render_card.getPlayerID(), this->position_.x, this->position_.y );
    //this->draw_face ( target, render_card.getID(), this->position_.x, this->position_.y );
    //this->draw_element ( target, render_card.getID(), this->position_.x, this->position_.y );
    this->draw ( target, render_card, this->position_.x, this->position_.y );
    //obj->draw ( target );
  //}
}

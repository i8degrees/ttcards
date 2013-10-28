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
#include "CardView.hpp"

CardView::CardView ( void )
{
NOM_LOG_TRACE ( TTCARDS );

  this->card_face_down = false;
  this->render_card = Card();

  this->card_background = std::shared_ptr<nom::Gradient> ( new nom::Gradient() );
  this->card_face = std::shared_ptr<nom::SpriteBatch> ( new nom::SpriteBatch ( "images/faces.json" ) );
  this->card_element = std::shared_ptr<nom::SpriteBatch> ( new nom::SpriteBatch ( "images/elements.json" ) );
  this->card_text = std::shared_ptr<nom::BitmapFont> ( new nom::BitmapFont() );

  card.push_back ( this->card_background );
  card.push_back ( this->card_face );
  card.push_back ( this->card_element );
  card.push_back ( this->card_text );

  this->card_background->setSize ( CARD_WIDTH - 4, CARD_HEIGHT - 4 );
  this->card_background->setMargins ( 4, 4 );
  this->card_background->setFillDirection ( nom::Gradient::FillDirection::Top );
}

CardView::CardView ( const nom::Coords& coords )
{
  // Stub
}

CardView::~CardView ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

bool CardView::load ( GameConfig* config )
{
  if ( config == nullptr ) return false;

  if ( this->card_text->load ( config->getString("CARD_FONTFACE"), nom::Color ( 110, 144, 190 ), true ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + config->getString("CARD_FONTFACE") );
    return false;
  }

  if ( this->card_face->load ( config->getString("CARD_FACES"), nom::Color ( 0, 0, 0 ), true ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + config->getString("CARD_FACES") );
    return false;
  }

  if ( this->card_element->load ( config->getString("CARD_ELEMENTS"), nom::Color ( 0, 0, 0 ), true ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + config->getString("CARD_ELEMENTS") );
    return false;
  }

  // Rescale our game resources if necessary.
  if ( config->getString("SCALE_ALGORITHM") == "scale2x" )
  {
    this->card_text->resize ( nom::ResizeAlgorithm::scale2x );
    this->card_face->resize ( nom::ResizeAlgorithm::scale2x );
    //this->card_background->resize ( nom::ResizeAlgorithm::scale2x );
    this->card_element->resize ( nom::ResizeAlgorithm::scale2x );
  }
  else if ( config->getString("SCALE_ALGORITHM") == "hqx" )
  {
    this->card_text->resize ( nom::ResizeAlgorithm::hq2x );
    this->card_face->resize ( nom::ResizeAlgorithm::hq2x );
    //this->card_background->resize ( nom::ResizeAlgorithm::hq2x );
    this->card_element->resize ( nom::ResizeAlgorithm::hq2x );
  }

  return true;
}

void CardView::draw_face_down ( SDL_Renderer* target, nom::int32 x, nom::int32 y ) const
{
  this->card_face->setSheetID ( NOFACE_ID );
  this->card_face->setPosition ( x, y );
  this->card_face->update();
  this->card_face->draw ( target );
}

void CardView::draw_background  (
                                  SDL_Renderer* target, nom::int32 player_id,
                                  nom::int32 x, nom::int32 y
                                ) const
{

  switch ( player_id )
  {
    case Card::PLAYER1:
    {
      this->card_background->setStartColor ( nom::Color ( 208, 223, 255 ) );
      this->card_background->setEndColor ( nom::Color ( 50, 59, 114 ) );
    }
    break;

    case Card::PLAYER2:
    {
      this->card_background->setStartColor ( nom::Color ( 251, 222, 232 ) );
      this->card_background->setEndColor ( nom::Color ( 114, 66, 66 ) );
    }
    break;

    case Card::NOPLAYER:
    default:
    {
      this->card_background->setStartColor ( nom::Color ( 197, 197, 197 ) );
      this->card_background->setEndColor ( nom::Color ( 84, 84, 84 ) );
    }
    break;
  }

  //this->card_background->setPosition ( this->position );
  this->card_background->setPosition ( x, y );
  this->card_background->update();
  this->card_background->draw ( target );
}

void CardView::draw_face  (
                            SDL_Renderer* target, nom::int32 face_id,
                            nom::int32 x, nom::int32 y
                          ) const
{
  // Our sprite sheets start at ID of zero now, not one
  this->card_face->setSheetID ( face_id - 1 );

  //this->card_face->setPosition ( this->position );
  this->card_face->setPosition ( nom::Coords ( x, y ) );
  this->card_face->update();
  this->card_face->draw ( target );
}

void CardView::draw_element (
                              SDL_Renderer* target, nom::int32 element_id,
                              nom::int32 x, nom::int32 y
                            ) const
{
  switch ( element_id )
  {
    default:
    case NONE: this->card_element->setSheetID ( ELEMENT_NONE ); break;
    case EARTH: this->card_element->setSheetID ( ELEMENT_EARTH ); break;
    case FIRE: this->card_element->setSheetID ( ELEMENT_FIRE ); break;
    case HOLY: this->card_element->setSheetID ( ELEMENT_HOLY ); break;
    case ICE: this->card_element->setSheetID ( ELEMENT_ICE ); break;
    case POISON: this->card_element->setSheetID ( ELEMENT_POISON ); break;
    case THUNDER: this->card_element->setSheetID ( ELEMENT_THUNDER ); break;
    case WATER: this->card_element->setSheetID ( ELEMENT_WATER ); break;
    case WIND: this->card_element->setSheetID ( ELEMENT_WIND ); break;
  }

  //this->card_element->setPosition ( this->position.x, this->position.y );
  this->card_element->setPosition ( nom::Coords( x, y ) );
  this->card_element->update();
  this->card_element->draw ( target );
}

void CardView::draw_text  (
                            SDL_Renderer* target, nom::int32 rank,
                            nom::int32 x, nom::int32 y
                          ) const
{
  if ( rank == 10 )
  {
    this->card_text->setText ( "A" );
  }
  else
  {
    this->card_text->setText ( std::to_string ( rank ) );
  }

  //this->card_text->setPosition ( this->position );
  this->card_text->setPosition ( nom::Coords ( x, y ) );
  this->card_text->update();
  this->card_text->draw ( target );
}

void CardView::draw (
                      SDL_Renderer* target, const Card& card,
                      nom::int32 x, nom::int32 y, bool face_down
                    ) const
{
  if ( this->card_face_down == true )
  {
    this->draw_face_down ( target, x, y );
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

void CardView::reposition ( const nom::Coords& coords )
{
  this->position = coords;
}

void CardView::face ( bool up )
{
  this->card_face_down = up;
}

void CardView::update ( void )
{
  //this->card_background->setPosition ( this->position );
  //this->card_face->setPosition ( this->position );
  //this->card_element->setPosition ( this->position );
  //this->card_text->setPosition ( this->position );

  //for ( DrawableList::const_iterator it = card.begin(); it != card.end(); ++it )
  //{
    //std::shared_ptr<nom::IDrawable> obj = *it;
    //obj->update();
  //}
}

void CardView::draw ( SDL_Renderer* target ) const
{
  //for ( DrawableList::const_iterator it = card.begin(); it != card.end(); ++it )
  //{
    //std::shared_ptr<nom::IDrawable> obj = *it;
    //this->draw_background ( target, render_card.getPlayerID(), this->position.x, this->position.y );
    //this->draw_face ( target, render_card.getID(), this->position.x, this->position.y );
    //this->draw_element ( target, render_card.getID(), this->position.x, this->position.y );
    this->draw ( target, render_card, this->position.x, this->position.y, this->card_face_down );
    //obj->draw ( target );
  //}
}

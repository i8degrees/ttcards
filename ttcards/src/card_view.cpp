/******************************************************************************
    card_view.cpp

    Card Graphics / Rendering

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_view.h"

using namespace nom;

CardView::CardView ( void )
{
  #ifdef DEBUG_CARD_VIEW_OBJ
    std::cout << "CardView::CardView (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->card_face = NULL;
  this->card_background = NULL;
  this->card_element = NULL;

  this->card_text.Load ( CARD_FONTFACE, nom::Color ( 110, 144, 190 ), 16, 16 );

  this->card_face = new nom::Sprite ( CARD_WIDTH, CARD_HEIGHT );
  this->card_background = new nom::Sprite ( CARD_WIDTH, CARD_HEIGHT );
  this->card_element = new nom::Sprite ( ELEMENT_WIDTH, ELEMENT_HEIGHT );

  this->card_face->setSheetDimensions ( 7104, 64, 0, 0 );
  this->card_background->setSheetDimensions ( 256, 64, 0, 0 );
  this->card_element->setSheetDimensions ( 144, 16, 0, 0 );

  this->card_face->Load ( CARD_FACES, nom::Color ( 0, 0, 0 ) );
  this->card_background->Load ( CARD_BACKGROUNDS, nom::Color ( 0, 0, 0 ) );
  this->card_element->Load ( CARD_ELEMENTS, nom::Color ( 0, 0, 0 ) );
}

CardView::~CardView ( void )
{
  #ifdef DEBUG_CARD_VIEW_OBJ
    std::cout << "CardView::~CardView (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->card_element != NULL )
  {
    delete this->card_element;
    this->card_element = NULL;
  }

  if ( this->card_background != NULL )
  {
    delete this->card_background;
    this->card_background = NULL;
  }

  if ( this->card_face != NULL )
  {
    delete this->card_face;
    this->card_face = NULL;
  }
}

// Helper method for drawing cards face down
bool CardView::drawFaceDown ( SDL_Surface *video_buffer, unsigned int x, unsigned int y )
{
  this->card_background->setSheetID ( NOFACE_ID );
  this->card_background->setXY ( BACKGROUND_ORIGIN_X + x, BACKGROUND_ORIGIN_Y + y );

  if ( this->card_background->Draw ( video_buffer ) == false )
    return false;

  return true;
}

bool CardView::DrawCard ( SDL_Surface *video_buffer, Card &card, unsigned int x, unsigned int y )
{
  if ( card.getID() != 0 )
  {
    switch ( card.getPlayerID() )
    {
      case Card::PLAYER1:
        card_background->setSheetID ( PLAYER1_BACKGROUND_ID );
        break;
      case Card::PLAYER2:
        card_background->setSheetID ( PLAYER2_BACKGROUND_ID );
        break;
      case Card::NOPLAYER:
      default:
        card_background->setSheetID ( NOPLAYER_BACKGROUND_ID );
        break;
    }

    card_background->setX ( BACKGROUND_ORIGIN_X + x );
    card_background->setY ( BACKGROUND_ORIGIN_Y + y );
    if ( card_background->Draw ( video_buffer ) == false )
      return false;

    card_face->setSheetID ( card.getID() );
    card_face->setX ( CARD_FACE_ORIGIN_X + x );
    card_face->setY ( CARD_FACE_ORIGIN_Y + y );

    if ( card_face->Draw ( video_buffer ) == false )
      return false;

    switch ( card.getElement() )
    {
      case NONE:
        card_element->setSheetID ( ELEMENT_NONE );
        break;
      case EARTH:
        card_element->setSheetID ( ELEMENT_EARTH );
        break;
      case FIRE:
        card_element->setSheetID ( ELEMENT_FIRE );
        break;
      case HOLY:
        card_element->setSheetID ( ELEMENT_HOLY );
        break;
      case ICE:
        card_element->setSheetID ( ELEMENT_ICE );
        break;
      case POISON:
        card_element->setSheetID ( ELEMENT_POISON );
        break;
      case THUNDER:
        card_element->setSheetID ( ELEMENT_THUNDER );
        break;
      case WATER:
        card_element->setSheetID ( ELEMENT_WATER );
        break;
      case WIND:
        card_element->setSheetID ( ELEMENT_WIND );
        break;
    }

    card_element->setX ( ELEMENT_ORIGIN_X + x );
    card_element->setY ( ELEMENT_ORIGIN_Y + y );
    if ( card_element->Draw ( video_buffer ) == false )
      return false;

    if ( card.getNorthRank() == 10 )
      this->card_text.setText ( "A" );
    else
      this->card_text.setText ( std::to_string ( card.getNorthRank() ) );

    this->card_text.setXY ( RANK_NORTH_ORIGIN_X + x, RANK_NORTH_ORIGIN_Y + y );
    this->card_text.Draw ( video_buffer );

    if ( card.getEastRank() == 10 )
      this->card_text.setText ( "A" );
    else
      this->card_text.setText ( std::to_string ( card.getEastRank() ) );

    this->card_text.setXY ( RANK_EAST_ORIGIN_X + x, RANK_EAST_ORIGIN_Y + y );
    this->card_text.Draw ( video_buffer );

    if ( card.getWestRank() == 10 )
      this->card_text.setText ( "A" );
    else
      this->card_text.setText ( std::to_string ( card.getWestRank() ) );

    this->card_text.setXY ( RANK_WEST_ORIGIN_X + x, RANK_WEST_ORIGIN_Y + y );
    this->card_text.Draw ( video_buffer );

    if ( card.getSouthRank() == 10 )
      this->card_text.setText ( "A" );
    else
      this->card_text.setText ( std::to_string ( card.getSouthRank() ) );

    this->card_text.setXY ( RANK_SOUTH_ORIGIN_X + x, RANK_SOUTH_ORIGIN_Y + y );
    this->card_text.Draw ( video_buffer );

    return true;
  } // if card.getID() != 0

  return false;
}

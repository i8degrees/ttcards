/******************************************************************************
    card_view.cpp

    Card Graphics / Rendering

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_view.h"

CardView::CardView ( void )
{
  #ifdef DEBUG_CARD_VIEW_OBJ
    std::cout << "CardView::CardView (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->card_face = NULL;
  this->card_background = NULL;
  this->card_element = NULL;

  this->card_text.Load ( CARD_FONTFACE, GColor ( 110, 144, 190 ), 16, 16 );

  this->card_face = new nom::Sprite ( CARD_WIDTH, CARD_HEIGHT );
  this->card_background = new nom::Sprite ( CARD_WIDTH, CARD_HEIGHT );
  this->card_element = new nom::Sprite ( ELEMENT_WIDTH, ELEMENT_HEIGHT );

  this->card_face->setSheetDimensions ( 7104, 64, 0, 0 );
  this->card_background->setSheetDimensions ( 256, 64, 0, 0 );
  this->card_element->setSheetDimensions ( 144, 16, 0, 0 );

  this->card_face->Load ( CARD_FACES, GColor ( 0, 0, 0 ) );
  this->card_background->Load ( CARD_BACKGROUNDS, GColor ( 0, 0, 0 ) );
  this->card_element->Load ( CARD_ELEMENTS, GColor ( 0, 0, 0 ) );
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
bool CardView::drawFaceDown ( Gfx *engine, unsigned int x, unsigned int y )
{
  this->card_background->setSheetID ( NOFACE_ID );
  this->card_background->setXY ( BACKGROUND_ORIGIN_X + x, BACKGROUND_ORIGIN_Y + y );

  if ( this->card_background->Draw ( engine ) == false )
    return false;

  return true;
}

bool CardView::DrawCard ( Gfx *engine, Card &card, unsigned int x, unsigned int y )
{
  if ( card.getID() != 0 )
  {
    switch ( card.getPlayerID() ) // player1 = 1, player2 = 2
    {
      case PLAYER1:
        card_background->setSheetID ( PLAYER1_BACKGROUND_ID );
        break;
      case PLAYER2:
        card_background->setSheetID ( PLAYER2_BACKGROUND_ID );
        break;
      case NOPLAYER:
      default:
        card_background->setSheetID ( NOPLAYER_BACKGROUND_ID );
        break;
    }

    card_background->setX ( BACKGROUND_ORIGIN_X + x );
    card_background->setY ( BACKGROUND_ORIGIN_Y + y );
    if ( card_background->Draw ( engine ) == false )
      return false;

    card_face->setSheetID ( card.getID() );
    card_face->setX ( CARD_FACE_ORIGIN_X + x );
    card_face->setY ( CARD_FACE_ORIGIN_Y + y );

    if ( card_face->Draw ( engine ) == false )
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
    if ( card_element->Draw ( engine ) == false )
      return false;

    if ( card.getNorthRank() == 10 )
      this->card_text.setTextBuffer ( "A" );
    else
      this->card_text.setTextBuffer ( std::to_string ( card.getNorthRank() ) );

    this->card_text.Draw ( engine, RANK_NORTH_ORIGIN_X + x, RANK_NORTH_ORIGIN_Y + y );

    if ( card.getEastRank() == 10 )
      this->card_text.setTextBuffer ( "A" );
    else
      this->card_text.setTextBuffer ( std::to_string ( card.getEastRank() ) );

    this->card_text.Draw ( engine, RANK_EAST_ORIGIN_X + x, RANK_EAST_ORIGIN_Y + y );

    if ( card.getWestRank() == 10 )
      this->card_text.setTextBuffer ( "A" );
    else
      this->card_text.setTextBuffer ( std::to_string ( card.getWestRank() ) );

    this->card_text.Draw ( engine, RANK_WEST_ORIGIN_X + x, RANK_WEST_ORIGIN_Y + y );

    if ( card.getSouthRank() == 10 )
      this->card_text.setTextBuffer ( "A" );
    else
      this->card_text.setTextBuffer ( std::to_string ( card.getSouthRank() ) );

    this->card_text.Draw ( engine, RANK_SOUTH_ORIGIN_X + x, RANK_SOUTH_ORIGIN_Y + y );

    return true;
  } // if card.getID() != 0

  return false;
}

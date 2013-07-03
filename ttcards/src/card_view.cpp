/******************************************************************************

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
bool CardView::drawFaceDown ( void* video_buffer, unsigned int x, unsigned int y )
{
  this->card_background->setSheetID ( NOFACE_ID );
  this->card_background->setPosition ( BACKGROUND_ORIGIN_X + x, BACKGROUND_ORIGIN_Y + y );
  this->card_background->Update();
  this->card_background->Draw ( video_buffer );

  return true;
}

bool CardView::DrawCard ( void* video_buffer, Card &card, unsigned int x, unsigned int y )
{
  if ( card.getID() != 0 )
  {
    switch ( card.getPlayerID() )
    {
      case Card::PLAYER1:
        this->card_background->setSheetID ( PLAYER1_BACKGROUND_ID );
        break;
      case Card::PLAYER2:
        this->card_background->setSheetID ( PLAYER2_BACKGROUND_ID );
        break;
      case Card::NOPLAYER:
      default:
        this->card_background->setSheetID ( NOPLAYER_BACKGROUND_ID );
        break;
    }

    this->card_background->setPosition ( BACKGROUND_ORIGIN_X + x, BACKGROUND_ORIGIN_Y + y );
    this->card_background->Update();
    this->card_background->Draw ( video_buffer );

    this->card_face->setSheetID ( card.getID() );
    this->card_face->setPosition ( CARD_FACE_ORIGIN_X + x, CARD_FACE_ORIGIN_Y + y );
    this->card_face->Update();

    this->card_face->Draw ( video_buffer );

    switch ( card.getElement() )
    {
      case NONE:
        this->card_element->setSheetID ( ELEMENT_NONE );
        break;
      case EARTH:
        this->card_element->setSheetID ( ELEMENT_EARTH );
        break;
      case FIRE:
        this->card_element->setSheetID ( ELEMENT_FIRE );
        break;
      case HOLY:
        this->card_element->setSheetID ( ELEMENT_HOLY );
        break;
      case ICE:
        this->card_element->setSheetID ( ELEMENT_ICE );
        break;
      case POISON:
        this->card_element->setSheetID ( ELEMENT_POISON );
        break;
      case THUNDER:
        this->card_element->setSheetID ( ELEMENT_THUNDER );
        break;
      case WATER:
        this->card_element->setSheetID ( ELEMENT_WATER );
        break;
      case WIND:
        this->card_element->setSheetID ( ELEMENT_WIND );
        break;
    }

    card_element->setPosition ( ELEMENT_ORIGIN_X + x, ELEMENT_ORIGIN_Y + y );
    card_element->Update();
    card_element->Draw ( video_buffer );

    if ( card.getNorthRank() == 10 )
      this->card_text.setText ( "A" );
    else
      this->card_text.setText ( std::to_string ( card.getNorthRank() ) );

    this->card_text.setPosition ( RANK_NORTH_ORIGIN_X + x, RANK_NORTH_ORIGIN_Y + y );
    this->card_text.Update();
    this->card_text.Draw ( video_buffer );

    if ( card.getEastRank() == 10 )
      this->card_text.setText ( "A" );
    else
      this->card_text.setText ( std::to_string ( card.getEastRank() ) );

    this->card_text.setPosition ( RANK_EAST_ORIGIN_X + x, RANK_EAST_ORIGIN_Y + y );
    this->card_text.Update();
    this->card_text.Draw ( video_buffer );

    if ( card.getWestRank() == 10 )
      this->card_text.setText ( "A" );
    else
      this->card_text.setText ( std::to_string ( card.getWestRank() ) );

    this->card_text.setPosition ( RANK_WEST_ORIGIN_X + x, RANK_WEST_ORIGIN_Y + y );
    this->card_text.Update();
    this->card_text.Draw ( video_buffer );

    if ( card.getSouthRank() == 10 )
      this->card_text.setText ( "A" );
    else
      this->card_text.setText ( std::to_string ( card.getSouthRank() ) );

    this->card_text.setPosition ( RANK_SOUTH_ORIGIN_X + x, RANK_SOUTH_ORIGIN_Y + y );
    this->card_text.Update();
    this->card_text.Draw ( video_buffer );

    return true;
  } // if card.getID() != 0

  return false;
}

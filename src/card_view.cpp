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

  this->card_text.LoadImage ( CARD_FONTFACE, GColor ( 110, 144, 190 ), 16, 16 );
  this->info_text.LoadImage ( INFO_FONTFACE, GColor ( 110, 144, 190 ), 16, 16 );

  this->card_face = new Sprite ( CARD_WIDTH, CARD_HEIGHT );
  this->card_background = new Sprite ( CARD_WIDTH, CARD_HEIGHT );
  this->card_element = new Sprite ( ELEMENT_WIDTH, ELEMENT_HEIGHT );

  this->card_face->SetSheetDimensions ( 7104, 64, 0, 0 );
  this->card_background->SetSheetDimensions ( 256, 64, 0, 0 );
  this->card_element->SetSheetDimensions ( 144, 16, 0, 0 );

  this->card_face->LoadImage ( CARD_FACES, GColor ( 0, 0, 0 ) );
  this->card_background->LoadImage ( CARD_BACKGROUNDS, GColor ( 0, 0, 0 ) );
  this->card_element->LoadImage ( CARD_ELEMENTS, GColor ( 0, 0, 0 ) );
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

void CardView::DrawName ( Gfx *engine, Card &card, unsigned int y )
{
  if ( card.getName() != "\0" )
  {
    this->info_text.setTextBuffer ( card.getName() );
    unsigned int text_width = this->info_text.getTextWidth ();
    this->info_text.DrawText ( engine, ( SCREEN_WIDTH - text_width ) / 2, y );
  }
}

bool CardView::DrawCard ( Gfx *engine, Card &card, unsigned int x, unsigned int y )
{
  if ( card.getID() != 0 )
  {
    switch ( card.getPlayerID() ) // player1 = 1, player2 = 2
    {
      case PLAYER1:
        card_background->SetSheetID ( PLAYER1_BACKGROUND_ID );
        break;
      case PLAYER2:
        card_background->SetSheetID ( PLAYER2_BACKGROUND_ID );
        break;
      case NOPLAYER:
      default:
        card_background->SetSheetID ( NOPLAYER_BACKGROUND_ID );
        break;
    }

    card_background->SetX ( BACKGROUND_ORIGIN_X + x );
    card_background->SetY ( BACKGROUND_ORIGIN_Y + y );
    if ( card_background->Draw ( engine ) == false )
      return false;

    card_face->SetSheetID ( card.getID() );
    card_face->SetX ( CARD_FACE_ORIGIN_X + x );
    card_face->SetY ( CARD_FACE_ORIGIN_Y + y );

    if ( card_face->Draw ( engine ) == false )
      return false;

    switch ( card.getElement() )
    {
      case NONE:
        card_element->SetSheetID ( ELEMENT_NONE );
        break;
      case EARTH:
        card_element->SetSheetID ( ELEMENT_EARTH );
        break;
      case FIRE:
        card_element->SetSheetID ( ELEMENT_FIRE );
        break;
      case HOLY:
        card_element->SetSheetID ( ELEMENT_HOLY );
        break;
      case ICE:
        card_element->SetSheetID ( ELEMENT_ICE );
        break;
      case POISON:
        card_element->SetSheetID ( ELEMENT_POISON );
        break;
      case THUNDER:
        card_element->SetSheetID ( ELEMENT_THUNDER );
        break;
      case WATER:
        card_element->SetSheetID ( ELEMENT_WATER );
        break;
      case WIND:
        card_element->SetSheetID ( ELEMENT_WIND );
        break;
    }

    card_element->SetX ( ELEMENT_ORIGIN_X + x );
    card_element->SetY ( ELEMENT_ORIGIN_Y + y );
    if ( card_element->Draw ( engine ) == false )
      return false;

    #ifdef DEBUG_CARD_VIEW
      this->card_text.setTextBuffer ( std::to_string ( card.getID() ) );
      this->card_text.DrawText ( engine, CARD_ID_ORIGIN_X + x, CARD_ID_ORIGIN_Y + y );
    #endif

    if ( card.getNorthRank() == 10 )
      this->card_text.setTextBuffer ( "A" );
    else
      this->card_text.setTextBuffer ( std::to_string ( card.getNorthRank() ) );

    this->card_text.DrawText ( engine, RANK_NORTH_ORIGIN_X + x, RANK_NORTH_ORIGIN_Y + y );

    if ( card.getEastRank() == 10 )
      this->card_text.setTextBuffer ( "A" );
    else
      this->card_text.setTextBuffer ( std::to_string ( card.getEastRank() ) );

    this->card_text.DrawText ( engine, RANK_EAST_ORIGIN_X + x, RANK_EAST_ORIGIN_Y + y );

    if ( card.getWestRank() == 10 )
      this->card_text.setTextBuffer ( "A" );
    else
      this->card_text.setTextBuffer ( std::to_string ( card.getWestRank() ) );

    this->card_text.DrawText ( engine, RANK_WEST_ORIGIN_X + x, RANK_WEST_ORIGIN_Y + y );

    if ( card.getSouthRank() == 10 )
      this->card_text.setTextBuffer ( "A" );
    else
      this->card_text.setTextBuffer ( std::to_string ( card.getSouthRank() ) );

    this->card_text.DrawText ( engine, RANK_SOUTH_ORIGIN_X + x, RANK_SOUTH_ORIGIN_Y + y );

    return true;
  } // if card.getID() != 0

  return false;
}

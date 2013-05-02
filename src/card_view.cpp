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

  this->text_buffer.LoadTTF ( CARD_FONTFACE, 12 ); // font: shitty temp
  this->text_buffer.SetTextColor ( 255, 255, 255 ); // color: white

  this->card_face = new Sprite ( CARD_WIDTH, CARD_HEIGHT );
  this->card_background = new Sprite ( CARD_WIDTH, CARD_HEIGHT );
  this->card_element = new Sprite ( ELEMENT_WIDTH, ELEMENT_HEIGHT );

  this->card_face->SetSheetDimensions ( 7104, 64, 0, 0 );
  this->card_background->SetSheetDimensions ( 256, 64, 0, 0 );
  this->card_element->SetSheetDimensions ( 144, 16, 0, 0 );

  this->card_face->LoadImage ( CARD_FACES );
  this->card_background->LoadImage ( CARD_BACKGROUNDS );
  this->card_element->LoadImage ( CARD_ELEMENTS );
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
  if ( card.name != "\0" )
  {
    this->text_buffer.SetTextBuffer ( card.name );
    unsigned int text_width = this->text_buffer.GetTextWidth ();
    this->text_buffer.DrawText ( engine, ( SCREEN_WIDTH - text_width ) / 2, y );
  }
}

bool CardView::DrawCard ( Gfx *engine, Card &card, unsigned int x, unsigned int y )
{
  if ( card.id != 0 )
  {
    switch ( card.player_id ) // player1 (0), player2 (1)
    {
      case 1:
        card_background->SetSheetID ( PLAYER1_BACKGROUND_ID );
        break;
      case 2:
        card_background->SetSheetID ( PLAYER2_BACKGROUND_ID );
        break;
      default:
        card_background->SetSheetID ( NOPLAYER_BACKGROUND_ID );
        break;
    }

    card_background->SetX ( BACKGROUND_ORIGIN_X + x );
    card_background->SetY ( BACKGROUND_ORIGIN_Y + y );
    if ( card_background->Draw ( engine ) == false )
      return false;

    card_face->SetSheetID ( card.id );
    card_face->SetX ( CARD_FACE_ORIGIN_X + x );
    card_face->SetY ( CARD_FACE_ORIGIN_Y + y );

    if ( card_face->Draw ( engine ) == false )
      return false;

    switch ( card.element )
    {
      case 0: // N/A
        card_element->SetSheetID ( ELEMENT_NONE );
        break;
      case 1: // EARTH
        card_element->SetSheetID ( ELEMENT_EARTH );
        break;
      case 2: // FIRE
        card_element->SetSheetID ( ELEMENT_FIRE );
        break;
      case 3: // HOLY
        card_element->SetSheetID ( ELEMENT_HOLY );
        break;
      case 4: // ICE
        card_element->SetSheetID ( ELEMENT_ICE );
        break;
      case 5: // POISON
        card_element->SetSheetID ( ELEMENT_POISON );
        break;
      case 6: // THUNDER
        card_element->SetSheetID ( ELEMENT_THUNDER );
        break;
      case 7: // WATER
        card_element->SetSheetID ( ELEMENT_WATER );
        break;
      case 8: // WIND
        card_element->SetSheetID ( ELEMENT_WIND );
        break;
    }

    card_element->SetX ( ELEMENT_ORIGIN_X + x );
    card_element->SetY ( ELEMENT_ORIGIN_Y + y );
    if ( card_element->Draw ( engine ) == false )
      return false;

    #ifdef DEBUG_CARD_VIEW
      this->text_buffer.SetTextBuffer ( std::to_string ( card.id ) );
      this->text_buffer.DrawText ( engine, CARD_ID_ORIGIN_X + x, CARD_ID_ORIGIN_Y + y );
    #endif

    this->text_buffer.SetTextBuffer ( std::to_string ( card.rank[0] ) );
    this->text_buffer.DrawText ( engine, RANK_NORTH_ORIGIN_X + x, RANK_NORTH_ORIGIN_Y + y );

    this->text_buffer.SetTextBuffer ( std::to_string ( card.rank[1] ) );
    this->text_buffer.DrawText ( engine, RANK_EAST_ORIGIN_X + x, RANK_EAST_ORIGIN_Y + y );

    this->text_buffer.SetTextBuffer ( std::to_string ( card.rank[2] ) );
    this->text_buffer.DrawText ( engine, RANK_SOUTH_ORIGIN_X + x, RANK_SOUTH_ORIGIN_Y + y );

    this->text_buffer.SetTextBuffer ( std::to_string ( card.rank[3] ) );
    this->text_buffer.DrawText ( engine, RANK_WEST_ORIGIN_X + x, RANK_WEST_ORIGIN_Y + y );

    return true;
  } // if card.id != 0

  return false;
}

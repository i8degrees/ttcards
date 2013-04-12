/******************************************************************************
    card_view.cpp

    Card graphics / drawing

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_view.h"

CardView::CardView ( void )
{
  #ifdef DEBUG_CARD_VIEW
    std::cout << "Hello, world! <From CardView::CardView>" << "\n" << std::endl;
  #endif

  this->card_face = NULL;
  this->card_background = NULL;
  this->card_element = NULL;

  this->text_card.LoadTTF ( CARD_FONTFACE, 12 ); // font: shitty temp
  this->text_card.SetTextColor ( 255, 255, 255 ); // color: white

  card_face = new Sprite ( CARD_WIDTH, CARD_HEIGHT );
  card_background = new Sprite ( CARD_WIDTH, CARD_HEIGHT );
  card_element = new Sprite ( ELEMENT_WIDTH, ELEMENT_HEIGHT );

}

CardView::~CardView ( void )
{
  #ifdef DEBUG_CARD_VIEW
    std::cout << "Goodbye cruel world! <From CardView::~CardView>" << "\n" << std::endl;
  #endif

  delete this->card_element;
  this->card_element = NULL;

  delete this->card_background;
  this->card_background = NULL;

  delete this->card_face;
  this->card_face = NULL;
}

bool CardView::EraseCard ( Gfx &engine, unsigned int x, unsigned int y )
{
  return true;
}

bool CardView::DrawCard ( Gfx &engine, Card &card, unsigned int x, unsigned int y, unsigned int player_state )
{
  card_background->LoadImage ( PLAYER1_CARDFACE );
  card_background->SetX ( x + 0 );
  card_background->SetY ( y + 0 );
  card_background->Draw ( engine );

  if ( card_face->LoadImage ( FACES_DIR + card.face ) == false )
  {
    std::cout << "ERR: " << card.face << "\n" << std::endl;
  }

  card_face->SetX ( x ); //card0.x = PLAYER1_ORIGIN_X;
  card_face->SetY ( y ); //cardFace.y = 0; //card0.y = PLAYER1_ORIGIN_Y;

  card_face->Draw ( engine );

  card_element->LoadImage ( ELEMENT_WATER );
  card_element->SetX ( x + 46 ); //cardElement.x = cardFace.x + 46;
  card_element->SetY ( y + 4 ); //cardElement.y = cardFace.y + 4;
  card_element->Draw ( engine );

  this->text_card.DrawText ( engine, std::to_string ( card.rank[0] ), x+8, y+0 ); //txt.DrawText ( card_buffer, "9", 26, 0, WHITE );
  this->text_card.DrawText ( engine, std::to_string ( card.rank[1] ), x+12, y+8 ); //txt.DrawText ( card_buffer, "6", 30, 8, WHITE );
  this->text_card.DrawText ( engine, std::to_string ( card.rank[2] ), x+8, y+16 ); //txt.DrawText ( card_buffer, "A", 26, 16, WHITE );
  this->text_card.DrawText ( engine, std::to_string ( card.rank[3] ), x+4, y+8 ); //txt.DrawText ( card_buffer, "2", 22, 8, WHITE );

  return true;
}

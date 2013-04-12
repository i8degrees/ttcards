/******************************************************************************
    player.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "player.h"

Player::Player ( void )
{
  //this->card_buffer = NULL;

  #ifdef DEBUG_PLAYER
    std::cout << "Hello, world! <From Player::Player>" << "\n" << std::endl;
  #endif

  //this->db.Load();
  //this->dealer.Init ( this->db.cards );

  //this->hand.Add ( this->dealer.selectCard ( 89 ) ); // Diablos
  //this->hand.Add ( this->dealer.selectCard ( 99 ) ); // Ward
  //this->hand.Add ( this->dealer.selectCard ( 0 ) ); // Geezard
  //this->hand.Add ( this->dealer.selectCard ( 109 ) ); // Squall
  //this->hand.Add ( this->dealer.selectCard ( 50 ) ); // Malboro

  //for ( int i = 0; i < MAX_HAND; i++ )
  //{
    //this->hand.Add ( this->dealer.Random() );
  //}


  //this->debug.ListCards ( this->hand.cards );
  //this->cdebug.ListCard ( this->hand.deck[0] );

  this->x = 0;
  this->y = 0;
  this->state = 0;
  this->turn = 0;
  this->score = 5;

  //this->card_buffer = SDL_CreateRGBSurface ( 0, CARD_WIDTH, CARD_HEIGHT,
                                      //SCREEN_BPP, 0, 0, 0, 0);
                                      //screen->format->BitsPerPixel,
                                      //screen->format->Rmask,
                                      //screen->format->Gmask,
                                      //screen->format->Bmask,
                                      //screen->format->Amask);

  //this->BuildCard();

  this->text_score.LoadTTF ( SCORE_FONTFACE, 32 );
  this->text_card.LoadTTF ( CARD_FONTFACE, 12 );

  this->text_score.SetTextColor ( 255, 255, 255 );
  this->text_card.SetTextColor ( 255, 255, 255 );
}

Player::~Player ( void )
{
  #ifdef DEBUG_PLAYER
    std::cout << "Goodbye cruel world! <From Player::~Player>" << "\n" << std::endl;
  #endif

  //SDL_FreeSurface ( this->card_buffer );
  //this->card_buffer = NULL;
}

/*
bool Player::BuildCard ( void )
{

  Sprite cardFace ( CARD_WIDTH, CARD_HEIGHT );
  Sprite cardBackground ( CARD_WIDTH, CARD_HEIGHT );
  Sprite cardElement ( ELEMENT_WIDTH, ELEMENT_HEIGHT );

  cardFace.LoadImage ( FACES_DIR + this->hand.cards[0].face );

  cardFace.x = 0; //card0.x = PLAYER1_ORIGIN_X;
  cardFace.y = 0; //card0.y = PLAYER1_ORIGIN_Y;

  cardBackground.LoadImage ( PLAYER1_CARDFACE );

  cardBackground.x = 0;
  cardBackground.y = 0;

  cardElement.LoadImage ( ELEMENT_WATER );

  cardElement.x = cardFace.x + 46;
  cardElement.y = cardFace.y + 4;

  cardBackground.Draw ( this->card_buffer );
  cardFace.Draw ( this->card_buffer );
  cardElement.Draw ( this->card_buffer );

  return true;

}
*/

SDL_Rect Player::GetXY ( void )
{
  SDL_Rect coords;

  this->x = coords.x;
  this->y = coords.y;

  return coords;
}

void Player::SetXY ( unsigned int x, unsigned int y )
{
  this->x = x;
  this->y = y;
}

unsigned int Player::GetState ( void )
{
  return this->state;
}

void Player::SetState ( unsigned int state )
{
  this->state = state;
}

unsigned int Player::GetScore ( void )
{
  return this->score;
}

void Player::SetScore ( unsigned int score )
{
  this->score = score;
}

bool Player::Draw ( Gfx &engine, unsigned int x, unsigned int y ) // card id
{

  this->text_card.DrawText ( engine, "9", x+8, y+0 ); //txt.DrawText ( card_buffer, "9", 26, 0, WHITE );
  this->text_card.DrawText ( engine, "6", x+12, y+8 ); //txt.DrawText ( card_buffer, "6", 30, 8, WHITE );
  this->text_card.DrawText ( engine, "A", x+8, y+16 ); //txt.DrawText ( card_buffer, "A", 26, 16, WHITE );
  this->text_card.DrawText ( engine, "2", x+4, y+8 ); //txt.DrawText ( card_buffer, "2", 22, 8, WHITE );

  //SDL_Rect offsets;
  //offsets.x = x;
  //offsets.y = y;

  //SDL_BlitSurface ( this->card_buffer, NULL, video_buffer, &offsets );

  return true;
}

bool Player::DrawScore ( Gfx &engine, unsigned int x, unsigned int y )
{
  //this->txtScore.DrawText ( video_buffer, "5", x, y, WHITE );
  this->text_score.DrawText ( engine, std::to_string ( this->score ), x, y );

  return true;
}

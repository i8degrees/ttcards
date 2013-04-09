/******************************************************************************
    player.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "player.h"

Player::Player ( void )
{
  this->card_buffer = NULL;

  #ifdef DEBUG_PLAYER
    std::cout << "Hello, world! <From Player::Player>" << "\n" << std::endl;
  #endif

  this->db.Load();
  this->dealer.Init ( this->db.cards );

  this->hand.Add ( this->dealer.selectCard ( 89 ) ); // Diablos
  this->hand.Add ( this->dealer.selectCard ( 99 ) ); // Ward
  this->hand.Add ( this->dealer.selectCard ( 0 ) ); // Geezard
  this->hand.Add ( this->dealer.selectCard ( 109 ) ); // Squall
  this->hand.Add ( this->dealer.selectCard ( 50 ) ); // Malboro

  for ( int i = 0; i < MAX_HAND; i++ )
  {
    //this->hand.Add ( this->dealer.Random() );
  }


  this->debug.ListCards ( this->hand.cards );
  //this->cdebug.ListCard ( this->hand.deck[0] );

  this->score = 5;

  //this->card_buffer = SDL_CreateRGBSurface ( 0, CARD_WIDTH, CARD_HEIGHT,
                                      //SCREEN_BPP, 0, 0, 0, 0);
                                      //screen->format->BitsPerPixel,
                                      //screen->format->Rmask,
                                      //screen->format->Gmask,
                                      //screen->format->Bmask,
                                      //screen->format->Amask);

  //this->BuildCard();

  this->txtScore.LoadTTF ( SCORE_FONTFACE, 32 );
  this->txtCard.LoadTTF ( CARD_FONTFACE, 12 );
}

Player::~Player ( void )
{
  #ifdef DEBUG_PLAYER
    std::cout << "Goodbye cruel world! <From Player::Player>" << "\n" << std::endl;
  #endif
  SDL_FreeSurface ( this->card_buffer );
  this->card_buffer = NULL;
}

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

unsigned int Player::GetScore ( void )
{
  return this->score;
}

bool Player::SetScore ( unsigned int value )
{
  this->score = value;
  return true;
}

bool Player::Draw ( SDL_Surface *video_buffer, int x, int y ) // card id
{

  this->txtCard.DrawText ( this->card_buffer, "9", 8, 0, WHITE ); //txt.DrawText ( card_buffer, "9", 26, 0, WHITE );
  this->txtCard.DrawText ( this->card_buffer, "6", 12, 8, WHITE ); //txt.DrawText ( card_buffer, "6", 30, 8, WHITE );
  this->txtCard.DrawText ( this->card_buffer, "A", 8, 16, WHITE ); //txt.DrawText ( card_buffer, "A", 26, 16, WHITE );
  this->txtCard.DrawText ( this->card_buffer, "2", 4, 8, WHITE ); //txt.DrawText ( card_buffer, "2", 22, 8, WHITE );

  SDL_Rect offsets;
  offsets.x = x;
  offsets.y = y;

  SDL_BlitSurface ( this->card_buffer, NULL, video_buffer, &offsets );

  return true;
}

bool Player::DrawScore ( SDL_Surface *video_buffer, int x, int y )
{
  //this->txtScore.DrawText ( video_buffer, "5", x, y, WHITE );
  this->txtScore.DrawText ( video_buffer, std::to_string(this->score), x, y, WHITE );

  return true;
}

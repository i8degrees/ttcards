/******************************************************************************
    player.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "player.h"

Player::Player ( void )
{

  //card = new Card ( card_type (  ) ); //deck = new Pile();

  Player::score = 5;



  card_buffer = SDL_CreateRGBSurface ( 0, CARD_WIDTH, CARD_HEIGHT,
                                      SCREEN_BPP, 0, 0, 0, 0);
                                      //screen->format->BitsPerPixel,
                                      //screen->format->Rmask,
                                      //screen->format->Gmask,
                                      //screen->format->Bmask,
                                      //screen->format->Amask);

  Player::BuildCard();

  txtScore.LoadTTF ( SCORE_FONTFACE, 32 );
  txtCard.LoadTTF ( CARD_FONTFACE, 12 );

}

Player::~Player ( void )
{
  SDL_FreeSurface ( card_buffer );
}

bool Player::BuildCard ( void )
{

  Sprite cardFace ( CARD_WIDTH, CARD_HEIGHT );
  Sprite cardBackground ( CARD_WIDTH, CARD_HEIGHT );
  Sprite cardElement ( ELEMENT_WIDTH, ELEMENT_HEIGHT );

  cardFace.LoadImage ( "./data/images/faces/89.bmp" );

  cardFace.x = 0; //card0.x = PLAYER1_ORIGIN_X;
  cardFace.y = 0; //card0.y = PLAYER1_ORIGIN_Y;

  cardBackground.LoadImage ( PLAYER1_CARDFACE );

  cardBackground.x = 0;
  cardBackground.y = 0;

  cardElement.LoadImage ( ELEMENT_WATER );

  cardElement.x = cardFace.x + 46;
  cardElement.y = cardFace.y + 4;

  cardBackground.Draw ( card_buffer );
  cardFace.Draw ( card_buffer );
  cardElement.Draw ( card_buffer );

  return true;

}

unsigned int Player::GetScore ( void )
{
  return Player::score;
}

bool Player::SetScore ( unsigned int value )
{
  Player::score = value;
  return true;
}

bool Player::Draw ( SDL_Surface *video_buffer, int x, int y ) // card id
{

  txtCard.DrawText ( card_buffer, "9", 8, 0, WHITE ); //txt.DrawText ( card_buffer, "9", 26, 0, WHITE );
  txtCard.DrawText ( card_buffer, "6", 12, 8, WHITE ); //txt.DrawText ( card_buffer, "6", 30, 8, WHITE );
  txtCard.DrawText ( card_buffer, "A", 8, 16, WHITE ); //txt.DrawText ( card_buffer, "A", 26, 16, WHITE );
  txtCard.DrawText ( card_buffer, "2", 4, 8, WHITE ); //txt.DrawText ( card_buffer, "2", 22, 8, WHITE );

  SDL_Rect offsets;
  offsets.x = x;
  offsets.y = y;

  SDL_BlitSurface ( card_buffer, NULL, video_buffer, &offsets );

  return true;
}

bool Player::DrawScore ( SDL_Surface *video_buffer, int x, int y )
{
  txtScore.DrawText ( video_buffer, std::to_string(Player::score), x, y, WHITE );

  return true;
}

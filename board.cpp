/******************************************************************************
    board.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "board.h"

/*
TODO: Add configurable board init, such as:

  Board ( unsigned int board_width, unsigned int board_height );

*/
Board::Board ( void )
{
  int x, y = 0;

  #ifdef DEBUG_BOARD_OBJ
    std::cout << "Board::Board (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
  {
    for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
    {
      this->grid[x][y] = 0;
    }
  }
}

Board::~Board ( void )
{
  #ifdef DEBUG_BOARD_OBJ
    std::cout << "Board::~Board (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  SDL_FreeSurface ( this->background );
  this->background = NULL;
}

void Board::Init ( CardHand &player1_cards, CardHand &player2_cards )
{
  this->player1_hand = &player1_cards;
  this->player2_hand = &player2_cards;
}

// TODO: Reconsider using an instance of Gfx solely for this method
bool Board::LoadBackground ( std::string filename )
{
  this->background = this->engine.LoadImage ( filename );

  return true;
}

bool Board::DrawBackground ( Gfx &engine )
{
  engine.DrawSurface ( this->background, 0, 0 );

  return true;
}

unsigned int Board::GetStatus ( unsigned int x, unsigned int y )
{
  return this->grid[x][y];
}

void Board::UpdateBoard ( unsigned int x, unsigned int y, unsigned int state )
{
  this->grid[x][y] = state;
}

void Board::Draw ( void )
{
  int x, y = 0;

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
  {
    for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
    {
      std::cout << this->grid[x][y] << " ";
    }
    std::cout << "\n" << std::endl;
  }
}

void Board::DrawBoard ( Gfx &engine )
{
  unsigned int hand_index = 0;
  unsigned int x, y = 0;

  for ( hand_index = 0; hand_index < this->player1_hand->cards.size(); hand_index++ )
  {
    if ( this->player1_hand->isValid ( this->player1_hand->cards[hand_index] ) == true )
    {
      for ( x = 0; x < 3; x++ )
      {
        for ( y = 0; y < 3; y++ )
        {
          if ( this->GetStatus ( x, y ) == this->player1_hand->cards[hand_index].id )
            this->card.DrawCard ( engine, this->player1_hand->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * y ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * x ), 0 );
          else
            this->card.DrawCard ( engine, this->player1_hand->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ), 0 );
        }
      }
    }
  }

  for ( hand_index = 0; hand_index < this->player2_hand->cards.size(); hand_index++ )
  {
    if ( this->player2_hand->isValid ( this->player2_hand->cards[hand_index] ) == true )
    {
      for ( x = 0; x < 3; x++ )
      {
        for ( y = 0; y < 3; y++ )
        {
          if ( this->GetStatus ( x, y ) == this->player2_hand->cards[hand_index].id )
            this->card.DrawCard ( engine, this->player2_hand->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * y ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * x ), 1 );
          else
            this->card.DrawCard ( engine, this->player2_hand->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ), 1 );
        }
      }
    }
  }
}

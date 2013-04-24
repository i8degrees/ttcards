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

  this->grid.resize ( BOARD_GRID_HEIGHT );

  for ( int i = 0; i < BOARD_GRID_WIDTH; i++ )
  {
    this->grid[i].resize ( BOARD_GRID_WIDTH );
  }

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
  unsigned int card_id = this->grid[x][y].id;
  return card_id;
}

void Board::UpdateBoard ( unsigned int x, unsigned int y, Card &card )
{
  this->grid[x][y] = card;
  this->board.cards.push_back ( card );
}

void Board::ListContents ( void )
{
  int x, y = 0;

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
  {
    for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
    {
      std::cout << this->grid[x][y].id << " ";
    }
    std::cout << "\n" << std::endl;
  }
}

void Board::DrawBoard ( Gfx &engine )
{
  unsigned int board_index = 0;
  unsigned int x, y = 0;

  for ( board_index = 0; board_index < this->board.cards.size(); board_index++ )
  {
    if ( this->board.isValid ( this->board.cards[board_index] ) == true )
    {
      for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
      {
        for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
        {
          if ( this->GetStatus ( x, y ) == this->board.cards[board_index].id )
            this->card.DrawCard ( engine, this->board.cards[board_index], BOARD_ORIGIN_X + ( CARD_WIDTH * y ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * x ) );
        }
      }
    }
  }
}

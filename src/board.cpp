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
  unsigned int x, y = 0;

  #ifdef DEBUG_BOARD_OBJ
    std::cout << "Board::Board (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->grid.resize ( BOARD_GRID_HEIGHT ); // y coords

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
  {
    this->grid[x].resize ( BOARD_GRID_WIDTH );
  }

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      this->grid[x][y] = 0;
    }
  }
}

void Board::Init ( CardView *card_gfx )
{
  this->card = card_gfx;
}

Board::~Board ( void )
{
  #ifdef DEBUG_BOARD_OBJ
    std::cout << "Board::~Board (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  if ( this->card != NULL )
  {
    this->card = NULL;
  }

  if ( this->background != NULL )
  {
    SDL_FreeSurface ( this->background );
    this->background = NULL;
  }
}

bool Board::LoadBackground ( std::string filename )
{
  this->background = Gfx::LoadImage ( filename );

  return true;
}

bool Board::DrawBackground ( Gfx *engine )
{
  engine->DrawSurface ( this->background, 0, 0 );

  return true;
}

void Board::checkBoard ( unsigned int x, unsigned int y, Card &card )
{
  if ( x == 0 && y == 0 ) // 0, 0
  {
    if ( this->grid[1][0].id != 0 )
    {
      if ( card.rank[1] > this->grid[1][0].rank[3] && card.player_id != this->grid[1][0].player_id )
      {
      #ifdef DEBUG_BOARD_CMP
        std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][0].id << std::endl;
        std::cout << std::endl;
      #endif

      this->grid[1][0].player_id = card.player_id;
      }
    }

    if ( this->grid[0][1].id != 0 )
    {
      if ( card.rank[2] > this->grid[0][1].rank[0] && card.player_id != this->grid[0][1].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][1].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[0][1].player_id = card.player_id;
      }
    }
  }

  if ( x == 1 && y == 0 ) // 1, 0
  {
    if ( this->grid[2][0].id != 0 )
    {
      if ( card.rank[1] > this->grid[2][0].rank[3] && card.player_id != this->grid[2][0].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][0].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[2][0].player_id = card.player_id;
      }
    }

    if ( this->grid[1][1].id != 0 )
    {
      if ( card.rank[2] > this->grid[1][1].rank[0] && card.player_id != this->grid[1][1].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][1].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[1][1].player_id = card.player_id;
      }
    }

    if ( this->grid[0][0].id != 0 )
    {
      if ( card.rank[3] > this->grid[0][0].rank[1] && card.player_id != this->grid[0][0].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][0].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[0][0].player_id = card.player_id;
      }
    }
  }

  if ( x == 2 && y == 0 ) // 2, 0
  {
    if ( this->grid[2][1].id != 0 )
    {
      if ( card.rank[2] > this->grid[2][1].rank[0] && card.player_id != this->grid[2][1].player_id )
      {
      #ifdef DEBUG_BOARD_CMP
        std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][1].id << std::endl;
        std::cout << std::endl;
      #endif

      this->grid[2][1].player_id = card.player_id;
      }
    }

    if ( this->grid[1][0].id != 0 )
    {
      if ( card.rank[3] > this->grid[1][0].rank[1] && card.player_id != this->grid[1][0].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][0].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[1][0].player_id = card.player_id;
      }
    }
  }

  if ( x == 0 && y == 1 ) // 0, 1
  {
    if ( this->grid[0][0].id != 0 )
    {
      if ( card.rank[0] > this->grid[0][0].rank[2] && card.player_id != this->grid[0][0].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][0].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[0][0].player_id = card.player_id;
      }
    }

    if ( this->grid[1][1].id != 0 )
    {
      if ( card.rank[1] > this->grid[1][1].rank[3] && card.player_id != this->grid[1][1].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][1].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[1][1].player_id = card.player_id;
      }
    }

    if ( this->grid[0][2].id != 0 )
    {
      if ( card.rank[2] > this->grid[0][2].rank[0] && card.player_id != this->grid[0][2].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][2].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[0][2].player_id = card.player_id;
      }
    }
  }

  if ( x == 1 && y == 1 ) // 1, 1
  {
    if ( this->grid[1][0].id != 0 )
    {
      if ( card.rank[0] > this->grid[1][0].rank[2] && card.player_id != this->grid[1][0].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][0].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[1][0].player_id = card.player_id;
      }
    }

    if ( this->grid[2][1].id != 0 )
    {
      if ( card.rank[1] > this->grid[2][1].rank[3] && card.player_id != this->grid[2][1].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][1].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[2][1].player_id = card.player_id;
      }
    }

    if ( this->grid[1][2].id != 0 )
    {
      if ( card.rank[2] > this->grid[1][2].rank[0] && card.player_id != this->grid[1][2].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][2].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[1][2].player_id = card.player_id;
      }
    }

    if ( this->grid[0][1].id != 0 )
    {
      if ( card.rank[3] > this->grid[0][1].rank[1] && card.player_id != this->grid[0][1].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][1].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[0][1].player_id = card.player_id;
      }
    }
  }

  if ( x == 2 && y == 1 ) // 2, 1
  {
    if ( this->grid[2][0].id != 0 )
    {
      if ( card.rank[0] > this->grid[2][0].rank[2] && card.player_id != this->grid[2][0].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][0].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[2][0].player_id = card.player_id;
      }
    }

    if ( this->grid[2][2].id != 0 )
    {
      if ( card.rank[2] > this->grid[2][2].rank[0] && card.player_id != this->grid[2][2].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][2].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[2][2].player_id = card.player_id;
      }
    }

    if ( this->grid[1][1].id != 0 )
    {
      if ( card.rank[3] > this->grid[1][1].rank[1] && card.player_id != this->grid[1][1].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][1].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[1][1].player_id = card.player_id;
      }
    }
  }

  if ( x == 0 && y == 2 ) // 0, 2
  {
    if ( this->grid[0][1].id != 0 )
    {
      if ( card.rank[0] > this->grid[0][1].rank[2] && card.player_id != this->grid[0][1].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][1].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[0][1].player_id = card.player_id;
      }
    }

    if ( this->grid[1][2].id != 0  )
    {
      if ( card.rank[1] > this->grid[1][2].rank[3] && card.player_id != this->grid[1][2].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][2].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[1][2].player_id = card.player_id;
      }
    }
  }

  if ( x == 1 && y == 2 ) // 1, 2
  {
    if ( this->grid[1][1].id != 0 )
    {
      if ( card.rank[0] > this->grid[1][1].rank[2] && card.player_id != this->grid[1][1].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][1].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[1][1].player_id = card.player_id;
      }
    }

    if ( this->grid[2][2].id != 0 )
    {
      if ( card.rank[1] > this->grid[2][2].rank[3] && card.player_id != this->grid[2][2].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][2].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[2][2].player_id = card.player_id;
      }
    }

    if ( this->grid[0][2].id != 0 )
    {
      if ( card.rank[3] > this->grid[0][2].rank[1] && card.player_id != this->grid[0][2].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][2].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[0][2].player_id = card.player_id;
      }
    }

  }

  if ( x == 2 && y == 2 ) // 2, 2
  {
    if ( this->grid[2][1].id != 0 )
    {
      if ( card.rank[0] > this->grid[2][1].rank[2] && card.player_id != this->grid[2][1].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][1].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[2][1].player_id = card.player_id;
      }
    }

    if ( this->grid[1][2].id != 0 )
    {
      if ( card.rank[3] > this->grid[1][2].rank[1] && card.player_id != this->grid[1][2].player_id )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][2].id << std::endl;
          std::cout << std::endl;
        #endif

        this->grid[1][2].player_id = card.player_id;
      }
    }
  }

  #ifdef DEBUG_BOARD_CMP
    std::cout << "\nEnd of comparison." << std::endl;
  #endif
}

unsigned int Board::GetPlayerCardCount ( unsigned int pid )
{
  unsigned int pid_count = 0;
  unsigned int x, y = 0;

  for ( y = 0; y < this->grid.size(); y++ )
  {
    for ( x = 0; x < this->grid.size(); x++ )
    {
      if ( this->grid[x][y].player_id != 0 && this->grid[x][y].player_id == pid )
      {
        pid_count += 1;
      }
    }
  }

  return pid_count;
}

unsigned int Board::GetStatus ( unsigned int x, unsigned int y )
{
  return this->grid[x][y].id;
}

void Board::UpdateBoard ( unsigned int x, unsigned int y, Card &card )
{
  this->grid[x][y] = card;

  this->checkBoard ( x, y, card );
}

void Board::ListContents ( void )
{
  unsigned int x, y = 0;

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      std::cout << this->grid[x][y].id << " " << this->grid[x][y].player_id << " ";
    }
    std::cout << std::endl << std::endl;
  }
}

void Board::DrawBoard ( Gfx *engine )
{
  unsigned int x, y = 0;

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      if ( this->GetStatus ( x, y ) == this->grid[x][y].id )
        this->card->DrawCard ( engine, this->grid[x][y], BOARD_ORIGIN_X + ( CARD_WIDTH * x ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * y ) );
    }
  }
}

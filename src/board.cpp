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

  this->rules = NULL;

  this->grid.resize ( BOARD_GRID_HEIGHT ); // y coords

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ ) // x coords
  {
    this->grid[x].resize ( BOARD_GRID_WIDTH );
  }

  /* Initialize our new 2D std::vector grid */
  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      this->grid[x][y] = 0;
    }
  }

}

void Board::Init ( CardView *card_gfx, CardRules *rules )
{
  this->card = card_gfx;
  this->rules = rules;
}

Board::~Board ( void )
{
  #ifdef DEBUG_BOARD_OBJ
    std::cout << "Board::~Board (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  if ( this->rules != NULL )
  {
    this->rules = NULL;
  }

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

std::vector<std::pair<int, int>> Board::checkBoard ( unsigned int x, unsigned int y )
{
  unsigned int cols, rows = 0;
  std::vector<std::pair<int, int>> coords;

  coords.clear();

  for ( cols = y; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( rows = x; x < BOARD_GRID_WIDTH; x++ )
    {
      if ( cols != 2 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols + 1 ) && GetStatus ( rows, cols + 1 ) != 0 )
        {
          if ( rules->CompareCards ( grid[rows][cols].rank[2], grid[rows][cols + 1].rank[0] ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << GetStatus ( rows, cols ) << " " << "wins against" << " " << GetStatus ( rows, cols + 1 ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows, cols + 1 ) );
          }
        }
      }

      if ( rows != 2 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows + 1, cols ) && GetStatus ( rows + 1, cols ) != 0 )
        {
          if ( rules->CompareCards ( grid[rows][cols].rank[1], grid[rows + 1][cols].rank[3] ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << GetStatus ( rows, cols ) << " " << "wins against" << " " << GetStatus ( rows + 1, cols ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows + 1, cols ) );
          }
        }
      }

      if ( cols != 0 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols - 1 ) && GetStatus ( rows, cols - 1 ) != 0 )
        {
          if ( rules->CompareCards ( grid[rows][cols].rank[0], grid[rows][cols - 1].rank[2] ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << GetStatus ( rows, cols ) << " " << "wins against" << " " << GetStatus ( rows, cols - 1 ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows, cols - 1 ) );
          }
        }
      }

      if ( rows != 0 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows - 1, cols ) && GetStatus ( rows - 1, cols ) != 0 )
        {
          if ( rules->CompareCards ( grid[rows][cols].rank[3], grid[rows - 1][cols].rank[1] ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << GetStatus ( rows, cols ) << " " << "wins against" << " " << GetStatus ( rows - 1, cols ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows - 1, cols ) );
          }
        }
      }
    } // rows for loop
  } // cols for loop

  return coords;

  #ifdef DEBUG_BOARD_CMP
    std::cout << "\nEnd of comparison." << std::endl;
  #endif
} // end Board::checkBoard()

unsigned int Board::GetPlayerCardCount ( unsigned int pid )
{
  unsigned int pid_count = 0;
  unsigned int x, y = 0;

  for ( y = 0; y < this->grid.size(); y++ )
  {
    for ( x = 0; x < this->grid.size(); x++ )
    {
      if ( this->getPlayerID ( x, y ) != 0 && this->getPlayerID ( x, y ) == pid )
      {
        pid_count += 1;
      }
    }
  }

  return pid_count;
}

unsigned int Board::GetTotalCount ( void )
{
  unsigned int total_count = 0;
  unsigned int x, y = 0;

  for ( y = 0; y < this->grid.size(); y++ )
  {
    for ( x = 0; x < this->grid.size(); x++ )
    {
      if ( this->GetStatus ( x, y ) != 0 )
      {
        total_count += 1;
      }
    }
  }

  return total_count;
}

unsigned int Board::GetStatus ( unsigned int x, unsigned int y )
{
  return this->grid[x][y].id;
}

void Board::flipCard ( unsigned int x, unsigned int y, unsigned int player_id )
{
  grid[x][y].player_id = player_id;
}

unsigned int Board::getPlayerID ( unsigned int x, unsigned int y )
{
  return this->grid[x][y].player_id;
}

void Board::updateStatus ( unsigned int x, unsigned int y, Card &card )
{
  grid[x][y] = card;
}

void Board::updateBoard ( unsigned int x, unsigned int y )
{
  //
}

void Board::ListContents ( void )
{
  unsigned int x, y = 0;

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      std::cout << GetStatus ( x, y ) << " " << getPlayerID ( x, y ) << " ";
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
      this->card->DrawCard ( engine, this->grid[x][y], BOARD_ORIGIN_X + ( CARD_WIDTH * x ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * y ) );
    }
  }
}

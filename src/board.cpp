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

  rules = NULL;

  grid.resize ( BOARD_GRID_HEIGHT ); // y coords

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ ) // x coords
  {
    grid[x].resize ( BOARD_GRID_WIDTH );
  }

  /* Initialize our new 2D std::vector grid */
  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      grid[x][y] = 0;
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

  if ( rules != NULL )
  {
    this->rules = NULL;
  }

  if ( card != NULL )
  {
    this->card = NULL;
  }

  if ( background != NULL )
  {
    SDL_FreeSurface ( background );
    background = NULL;
  }
}

bool Board::LoadBackground ( std::string filename )
{
  background = Gfx::LoadImage ( filename );

  return true;
}

// We check cards in a clockwise fashion; NORTH, EAST, SOUTH, WEST
std::vector<std::pair<int, int>> Board::checkBoard ( unsigned int x, unsigned int y )
{
  unsigned int cols, rows = 0;
  std::vector<std::pair<int, int>> coords;

  coords.clear(); // initialize a fresh new coords list

  for ( cols = y; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( rows = x; x < BOARD_GRID_WIDTH; x++ )
    {
      // Compare card's NORTH rank with opponent's SOUTH rank
      if ( cols != 0 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols - 1 ) && getStatus ( rows, cols - 1 ) != 0 )
        {
          if ( rules->compareCards ( grid[rows][cols].getNorthRank(), grid[rows][cols - 1].getSouthRank() ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << getStatus ( rows, cols ) << " " << "wins against" << " " << getStatus ( rows, cols - 1 ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows, cols - 1 ) );
          }
        }
      }

      // Compare card's EAST rank with opponent's WEST rank
      if ( rows != BOARD_GRID_WIDTH - 1 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows + 1, cols ) && getStatus ( rows + 1, cols ) != 0 )
        {
          if ( rules->compareCards ( grid[rows][cols].getEastRank(), grid[rows + 1][cols].getWestRank() ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << getStatus ( rows, cols ) << " " << "wins against" << " " << getStatus ( rows + 1, cols ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows + 1, cols ) );
          }
        }
      }

      // Compare card's SOUTH rank with opponent's NORTH rank
      if ( cols != BOARD_GRID_HEIGHT - 1 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols + 1 ) && getStatus ( rows, cols + 1 ) != 0 )
        {
          if ( rules->compareCards ( grid[rows][cols].getSouthRank(), grid[rows][cols + 1].getNorthRank() ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << getStatus ( rows, cols ) << " " << "wins against" << " " << getStatus ( rows, cols + 1 ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows, cols + 1 ) );
          }
        }
      }

      // Compare card's WEST rank with opponent's EAST rank
      if ( rows != 0 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows - 1, cols ) && getStatus ( rows - 1, cols ) != 0 )
        {
          if ( rules->compareCards ( grid[rows][cols].getWestRank(), grid[rows - 1][cols].getEastRank() ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << getStatus ( rows, cols ) << " " << "wins against" << " " << getStatus ( rows - 1, cols ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows - 1, cols ) );
          }
        }
      }

    } // rows for loop
  } // cols for loop

  return coords;
} // end Board::checkBoard()

// get card count by placed cards on board
unsigned int Board::getCount ( void )
{
  unsigned int total_count = 0;
  unsigned int x, y = 0;

  for ( y = 0; y < grid.size(); y++ )
  {
    for ( x = 0; x < grid.size(); x++ )
    {
      if ( getStatus ( x, y ) != 0 )
      {
        total_count += 1;
      }
    }
  }

  return total_count;
}

// get card count by player's cards on board
unsigned int Board::getPlayerCount ( unsigned int player_id )
{
  unsigned int pid_count = 0;
  unsigned int x, y = 0;

  for ( y = 0; y < grid.size(); y++ )
  {
    for ( x = 0; x < grid.size(); x++ )
    {
      if ( getPlayerID ( x, y ) != 0 && getPlayerID ( x, y ) == player_id )
      {
        pid_count += 1;
      }
    }
  }

  return pid_count;
}

unsigned int Board::getStatus ( unsigned int x, unsigned int y )
{
  return grid[x][y].getID();
}

void Board::updateStatus ( unsigned int x, unsigned int y, Card &card )
{
  grid[x][y] = card;
}

unsigned int Board::getPlayerID ( unsigned int x, unsigned int y )
{
  return grid[x][y].getPlayerID();
}

void Board::flipCard ( unsigned int x, unsigned int y, unsigned int player_id )
{
  grid[x][y].setPlayerID ( player_id );
}

void Board::Update ( unsigned int x, unsigned int y )
{
  // TODO
}

void Board::Draw ( Gfx *engine )
{
  unsigned int x, y = 0;

  engine->DrawSurface ( this->background, 0, 0 );

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      this->card->DrawCard ( engine, this->grid[x][y], BOARD_ORIGIN_X + ( CARD_WIDTH * x ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * y ) );
    }
  }
}

void Board::List ( void ) // debug
{
  unsigned int x, y = 0;

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      std::cout << getStatus ( x, y ) << " " << getPlayerID ( x, y ) << " ";
    }
    std::cout << std::endl << std::endl;
  }
}

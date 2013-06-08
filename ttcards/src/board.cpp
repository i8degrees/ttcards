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
    std::cout << "Board::Board(): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->rules = NULL;

  this->grid.resize ( BOARD_GRID_HEIGHT ); // initialize y coords

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ ) // initialize x coords
    this->grid[x].resize ( BOARD_GRID_WIDTH );

  /* Say hello to our brand spanking new 2D std::vector grid */
  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      this->grid[x][y] = Card::Card();
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
    this->rules = NULL;

  if ( this->card != NULL )
    this->card = NULL;
}

std::vector<std::pair<int, int>> Board::checkBoard ( unsigned int x, unsigned int y )
{
  unsigned int cols, rows = 0;
  unsigned int same_count = 0;
  std::vector<std::pair<int, int>> coords;

  coords.clear(); // initialize a fresh new coords list

  for ( cols = y; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( rows = x; x < BOARD_GRID_WIDTH; x++ )
    {
      // Compare card's WEST rank with opponent's EAST rank
      if ( rows != 0 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows - 1, cols ) && getStatus ( rows - 1, cols ) != 0 )
        {
          if ( rules->getRules() == 2 )
          {
            if ( grid[rows][cols].getWestRank() == grid[rows - 1][cols].getEastRank() )
            {
              same_count += 1;
              if ( same_count < 2 && coords.size() < 2 )
                coords.push_back ( std::make_pair ( rows - 1, cols ) );
            }
          }
          if ( grid[rows][cols].getWestRank() > grid[rows - 1][cols].getEastRank() == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << getStatus ( rows, cols ) << " " << "wins against" << " " << getStatus ( rows - 1, cols ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows - 1, cols ) );
          }
        }
      }

      // Compare card's SOUTH rank with opponent's NORTH rank
      if ( cols != BOARD_GRID_HEIGHT - 1 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols + 1 ) && getStatus ( rows, cols + 1 ) != 0 )
        {
          if ( rules->getRules() == 2 )
          {
            if ( grid[rows][cols].getSouthRank() == grid[rows][cols + 1].getNorthRank() )
            {
              same_count += 1;
              if ( same_count < 2 && coords.size() < 2 )
                coords.push_back ( std::make_pair ( rows, cols + 1 ) );
            }
          }
          if ( grid[rows][cols].getSouthRank() > grid[rows][cols + 1].getNorthRank() == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << getStatus ( rows, cols ) << " " << "wins against" << " " << getStatus ( rows, cols + 1 ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows, cols + 1 ) );
          }
        }
      }

      // Compare card's EAST rank with opponent's WEST rank
      if ( rows != BOARD_GRID_WIDTH - 1 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows + 1, cols ) && getStatus ( rows + 1, cols ) != 0 )
        {
          if ( rules->getRules() == 2 )
          {
            if ( grid[rows][cols].getEastRank() == grid[rows + 1][cols].getWestRank() )
            {
              same_count += 1;
              if ( same_count < 2 && coords.size() < 2 )
                coords.push_back ( std::make_pair ( rows + 1, cols ) );
            }
          }
          if ( grid[rows][cols].getEastRank() > grid[rows + 1][cols].getWestRank() == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << getStatus ( rows, cols ) << " " << "wins against" << " " << getStatus ( rows + 1, cols ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows + 1, cols ) );
          }
        }
      }

      // Compare card's NORTH rank with opponent's SOUTH rank
      if ( cols != 0 )
      {
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols - 1 ) && getStatus ( rows, cols - 1 ) != 0 )
        {
          if ( rules->getRules() == 2 )
          {
            if ( grid[rows][cols].getNorthRank() == grid[rows][cols - 1].getSouthRank() )
            {
              same_count += 1;
              if ( same_count < 2 && coords.size() < 2 )
                coords.push_back ( std::make_pair ( rows, cols - 1 ) );
            }
          }
          if ( grid[rows][cols].getNorthRank() > grid[rows][cols - 1].getSouthRank() == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << getStatus ( rows, cols ) << " " << "wins against" << " " << getStatus ( rows, cols - 1 ) << std::endl << std::endl;
            #endif

            coords.push_back ( std::make_pair ( rows, cols - 1 ) );
          }
        }
      }

    } // rows for loop
  } // cols for loop

  return coords;
} // end Board::checkBoard()

// Getter helper method for obtaining total count of placed cards on board
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

// Getter helper method for obtaining card ID at x, y coords
unsigned int Board::getStatus ( unsigned int x, unsigned int y )
{
  return grid[x][y].getID();
}

// Setter helper method for placing a card on the board at x, y coords
void Board::updateStatus ( unsigned int x, unsigned int y, Card &card )
{
  grid[x][y] = card;
}

// Getter helper method for obtaining player owner tag / ID on a card at x, y
// coords
unsigned int Board::getPlayerID ( unsigned int x, unsigned int y )
{
  return grid[x][y].getPlayerID();
}

// Setter helper method for swapping player owner tags / IDs on a card at x, y
// coords
void Board::flipCard ( unsigned int x, unsigned int y, unsigned int player_id )
{
  grid[x][y].setPlayerID ( player_id );
}

// Getter helper method for obtaining card name at x, y coords
std::string Board::getName ( unsigned int x, unsigned int y )
{
  return this->grid[x][y].getName();
}

// (Private) Getter helper method for obtaining card placed at x, y coords;
// Used within Board::Draw(), TTcards::showCardInfoBox() method calls
//
Card &Board::getCard ( unsigned int x, unsigned int y )
{
  return this->grid[x][y];
}

void Board::Update ( unsigned int x, unsigned int y )
{
  // TODO
}

// Draws our active board grid based on their values (card IDs)
void Board::Draw ( void* video_buffer )
{
  unsigned int x, y = 0;

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      this->card->DrawCard ( video_buffer, this->getCard ( x, y ), BOARD_ORIGIN_X + ( CARD_WIDTH * x ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * y ) );
    }
  }
}

// Debug helper method
void Board::List ( void )
{
  unsigned int x, y = 0;

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      std::cout << getStatus ( x, y ) << "[" << getPlayerID ( x, y ) << "]" << "\t";
    }
    std::cout << std::endl << std::endl;
  }
}

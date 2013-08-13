/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "Board.hpp"

/*
TODO: Add configurable board init, such as:

  Board ( unsigned int board_width, unsigned int board_height );

*/
Board::Board ( CardRules& ruleset, CardView* view )
{
  this->rules = ruleset;
  this->card = view;

  this->initialize();
}

Board::Board ( void )
{
  this->initialize();
}

Board::~Board ( void )
{
TTCARDS_LOG_CLASSINFO;
}

void Board::initialize ( void )
{
TTCARDS_LOG_CLASSINFO;

  nom::int32 x, y = 0; // iterator

  this->grid.resize ( BOARD_GRID_HEIGHT ); // initialize y coords

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ ) // initialize x coords
    this->grid[x].resize ( BOARD_GRID_WIDTH );

  /* Say hello to our brand spanking new 2D std::vector grid */
  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      this->grid[x][y] = Card();
    }
  }
}

std::vector<std::pair<nom::int32, nom::int32>> Board::checkBoard ( nom::int32 x, nom::int32 y )
{
  nom::int32 cols, rows = 0; // iterator
  nom::int32 same_count = 0;
  std::vector<std::pair<nom::int32, nom::int32>> coords;

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
          if ( this->rules.getRules() == 2 )
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
          if ( this->rules.getRules() == 2 )
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
          if ( this->rules.getRules() == 2 )
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
          if ( this->rules.getRules() == 2 )
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
  nom::ulong x, y = 0; // iterator

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
nom::int32 Board::getStatus ( nom::int32 x, nom::int32 y )
{
  if ( ( x >= 0 && x <= BOARD_GRID_WIDTH ) && ( y >= 0 && y <= BOARD_GRID_HEIGHT ) )
    return grid[x][y].getID();
  else
    return -1;
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
// Used within Board::Draw(), Game::showCardInfoBox() method calls
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
  nom::int32 x, y = 0; // iterator

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
  nom::int32 x, y = 0; // iterator

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      std::cout << getStatus ( x, y ) << "[" << getPlayerID ( x, y ) << "]" << "\t";
    }
    std::cout << std::endl << std::endl;
  }
}

const nom::int32 Board::operator() ( const nom::int32 x, const nom::int32 y )
{
  return this->getStatus ( x, y );
}

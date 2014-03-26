/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
NOM_LOG_TRACE ( TTCARDS );
}

void Board::clear ( void )
{
  this->initialize();
}

void Board::initialize ( void )
{
NOM_LOG_TRACE ( TTCARDS );

  // Calculate the global screen coordinates for local positions on the board
  this->board_map[0] = nom::IntRect ( 0, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );
  this->board_map[1] = nom::IntRect ( 1, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );
  this->board_map[2] = nom::IntRect ( 2, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );

  this->board_map[3] = nom::IntRect ( 0, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );
  this->board_map[4] = nom::IntRect ( 1, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );
  this->board_map[5] = nom::IntRect ( 2, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );

  this->board_map[6] = nom::IntRect ( 0, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );
  this->board_map[7] = nom::IntRect ( 1, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );
  this->board_map[8] = nom::IntRect ( 2, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );

  this->grid.resize ( BOARD_GRID_HEIGHT );

  for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    this->grid[x].resize ( BOARD_GRID_WIDTH );

  nom::int32 idx = 0;
  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      nom::uint32 random_element = nom::rand ( 0, MAX_ELEMENT );
      this->grid[x][y] = BoardTile ( Card(), this->board_map[idx], random_element );
      idx++; // this->board_map[0..8]
    }
  }
}

const nom::IntRect Board::getGlobalBounds ( nom::int32 x, nom::int32 y ) const
{
  for ( nom::int32 idx = 0; idx < ( BOARD_GRID_WIDTH * BOARD_GRID_HEIGHT ); idx++ )
  {
    if ( x <= this->board_map[idx].w && x >= BOARD_ORIGIN_X && y <= this->board_map[idx].h && y >= BOARD_ORIGIN_Y )
      return this->board_map[idx];
  }

  return nom::IntRect::null;
}

const std::vector<BoardTile> Board::find_adjacent ( nom::int32 x, nom::int32 y ) const
{
  std::vector<BoardTile> adjacent_cards;

  nom::int32 cols = y;
  nom::int32 rows = x;

  // Compare card's WEST rank with opponent's EAST rank
  if ( rows != 0 )
  {
    if ( this->getStatus ( rows - 1, cols ) != BAD_CARD_ID )
    {
      adjacent_cards.push_back ( this->tile ( rows - 1, cols ) );
    }
  }

  // Compare card's EAST rank with opponent's WEST rank
  if ( rows != BOARD_GRID_WIDTH - 1 )
  {
    if ( this->getStatus ( rows + 1, cols ) != BAD_CARD_ID )
    {
      adjacent_cards.push_back ( this->tile ( rows + 1, cols ) );
    }
  }

  // Compare card's NORTH rank with opponent's SOUTH rank
  if ( cols != 0 )
  {
    if ( this->getStatus ( rows, cols - 1 ) != BAD_CARD_ID )
    {
      adjacent_cards.push_back ( this->tile ( rows, cols - 1 ) );
    }
  }

  // Compare card's SOUTH rank with opponent's NORTH rank
  if ( cols != BOARD_GRID_HEIGHT - 1 )
  {
    if ( this->getStatus ( rows, cols + 1 ) != BAD_CARD_ID )
    {
      adjacent_cards.push_back ( this->tile ( rows, cols + 1 ) );
    }
  }

  return adjacent_cards;
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
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows - 1, cols ) && getStatus ( rows - 1, cols ) != BAD_CARD_ID )
        {
          if ( this->rules.getRules() == 2 )
          {
            if ( this->grid[rows][cols].tile_card.getWestRank() == grid[rows - 1][cols].tile_card.getEastRank() )
            {
              same_count += 1;
              if ( same_count < 2 && coords.size() < 2 )
                coords.push_back ( std::make_pair ( rows - 1, cols ) );
            }
          }
          if ( this->grid[rows][cols].tile_card.getWestRank() > grid[rows - 1][cols].tile_card.getEastRank() == true )
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
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols + 1 ) && getStatus ( rows, cols + 1 ) != BAD_CARD_ID )
        {
          if ( this->rules.getRules() == 2 )
          {
            if ( this->grid[rows][cols].tile_card.getSouthRank() == grid[rows][cols + 1].tile_card.getNorthRank() )
            {
              same_count += 1;
              if ( same_count < 2 && coords.size() < 2 )
                coords.push_back ( std::make_pair ( rows, cols + 1 ) );
            }
          }
          if ( this->grid[rows][cols].tile_card.getSouthRank() > grid[rows][cols + 1].tile_card.getNorthRank() == true )
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
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows + 1, cols ) && getStatus ( rows + 1, cols ) != BAD_CARD_ID )
        {
          if ( this->rules.getRules() == 2 )
          {
            if ( this->grid[rows][cols].tile_card.getEastRank() == this->grid[rows + 1][cols].tile_card.getWestRank() )
            {
              same_count += 1;
              if ( same_count < 2 && coords.size() < 2 )
                coords.push_back ( std::make_pair ( rows + 1, cols ) );
            }
          }
          if ( this->grid[rows][cols].tile_card.getEastRank() > grid[rows + 1][cols].tile_card.getWestRank() == true )
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
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols - 1 ) && getStatus ( rows, cols - 1 ) != BAD_CARD_ID )
        {
          if ( this->rules.getRules() == 2 )
          {
            if ( this->grid[rows][cols].tile_card.getNorthRank() == grid[rows][cols - 1].tile_card.getSouthRank() )
            {
              same_count += 1;
              if ( same_count < 2 && coords.size() < 2 )
                coords.push_back ( std::make_pair ( rows, cols - 1 ) );
            }
          }
          if ( this->grid[rows][cols].tile_card.getNorthRank() > grid[rows][cols - 1].tile_card.getSouthRank() == true )
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

nom::uint32 Board::getCount ( void )
{
  nom::uint32 total_count = 0;

  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      if ( this->getStatus ( x, y ) != BAD_CARD_ID )
      {
        total_count += 1;
      }
    }
  }
  return total_count;
}

nom::uint32 Board::getPlayerCount ( nom::int32 player_id )
{
  nom::uint32 pid_count = 0;

  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      if ( this->getPlayerID ( x, y ) != 0 && this->getPlayerID ( x, y ) == player_id )
      {
        pid_count += 1;
      }
    }
  }
  return pid_count;
}

const nom::int32 Board::getStatus ( nom::int32 x, nom::int32 y ) const
{
  if ( ( x >= 0 && x <= BOARD_GRID_WIDTH ) && ( y >= 0 && y <= BOARD_GRID_HEIGHT ) )
  {
    return this->grid[x][y].tile_card.getID(); // FIXME
  }

  return BAD_CARD_ID;
}

void Board::updateStatus ( nom::int32 x, nom::int32 y, const Card& card )
{
  this->grid[x][y].update ( nom::Point2i( x, y ), card );
/*
  if ( this->grid[x][y].element() != 0 )
  {
    this->grid[x][y].tile_card.increaseNorthRank();
    this->grid[x][y].tile_card.increaseEastRank();
    this->grid[x][y].tile_card.increaseSouthRank();
    this->grid[x][y].tile_card.increaseWestRank();
  }
  else
  {
    this->grid[x][y].tile_card.decreaseNorthRank();
    this->grid[x][y].tile_card.decreaseEastRank();
    this->grid[x][y].tile_card.decreaseSouthRank();
    this->grid[x][y].tile_card.decreaseWestRank();
  }
*/
}

const nom::int32 Board::getPlayerID ( nom::int32 x, nom::int32 y ) const
{
  return this->grid[x][y].tile_card.getPlayerID(); // FIXME
}

void Board::flipCard ( nom::int32 x, nom::int32 y, nom::int32 player_id )
{
  this->grid[x][y].tile_card.setPlayerID ( player_id ); // FIXME
}

const std::string Board::getName ( nom::int32 x, nom::int32 y ) const
{
  Card selected = this->grid[x][y].tile();
  return selected.getName();
}

const Card& Board::get ( nom::int32 x, nom::int32 y ) const
{
  return this->grid[x][y].tile();
}

const BoardTile& Board::tile ( nom::int32 x, nom::int32 y ) const
{
  return this->grid[x][y];
}

void Board::update ( void )
{
  this->card->update();
}

void Board::draw ( nom::IDrawable::RenderTarget& target )
{
  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      if ( this->get ( x, y ).getID() == BAD_CARD_ID ) continue;

      // Positions of the cards on the game board
      nom::Point2i board_pos  (
                                BOARD_ORIGIN_X + ( CARD_WIDTH * x ),
                                BOARD_ORIGIN_Y + ( CARD_HEIGHT * y )
                              );

      this->card->reposition ( board_pos );

      this->card->setViewCard ( this->grid[x][y].tile() );

      this->card->draw ( target );

/*
      if ( this->grid[x][y].element() != 0 )
      {
        nom::IntRect element_pos (
                                  board_pos.x + ELEMENT_WIDTH,
                                  board_pos.y + ELEMENT_HEIGHT
                                );

        this->card->draw_element  (
                                    video_buffer, this->grid[x][y].element(),
                                    element_pos.x, element_pos.y
                                  );
      }
*/
    } // end for loop rows
  } // end for loop cols
}

bool Board::save ( const std::string& filename )
{
  nom::JsonCppValue fp;
  nom::JsonCppValue card;

  for ( nom::int32 y = 0; y != BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x != BOARD_GRID_WIDTH; x++ )
    {
      // Serialize each card's attributes
      card = this->grid[x][y].tile_card.serialize();
      fp.insert ( card );

      // Additional card attributes
      fp.insert ( "player_id", this->grid[x][y].tile_card.getPlayerID() );
      fp.insert ( "owner", this->grid[x][y].tile_card.getPlayerOwner() );
      fp.endl();
    }
  }

  if ( fp.serialize( fp, filename ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    return false;
  }

  return true;
}

bool Board::load ( const std::string& filename )
{
  nom::JsonCppValue fp; // JSON interface (deprecated)
  nom::JsonCppValue object;

  // The card attributes we are loading in will be stored in here temporarily.
  // This will become the data to load onto the board if all goes well..!
  Card card;
  Cards cards_buffer;

  if ( fp.unserialize( filename, fp ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

  for ( nom::uint32 idx = 0; idx != fp.size(); ++idx )
  {
    card.setID ( fp.get_int( "id" ) );
    card.setName ( fp.get_string( "name" ) );
    card.setLevel ( fp.get_int( "level" ) );
    card.setType ( fp.get_int( "type" ) );
    card.setElement ( fp.get_int( "element" ) );
    card.setType ( fp.get_int( "type" ) );

    std::vector<int> ranks = fp.get_ints ( "ranks" );
    card.set_ranks ( ranks );

    card.setPlayerID ( fp.get_int( "player_id" ) );
    card.setPlayerOwner ( fp.get_int( "owner" ) );

    fp.endl();
    // Commit contents to our buffer if all goes well
    cards_buffer.push_back ( card );
  } // end for loop

  // Sanity check
  if ( cards_buffer.size() < 8 ) // - 1 padding; we start at zero, not one
  {
NOM_LOG_ERR ( TTCARDS, "Board data is invalid from file: " + filename );
    return false;
  }

  this->clear(); // otherwise we may exceed our board size limit

  // All is well, let us make it permanent!
  // Load each card object onto the board grid at the proper X, Y coordinates.
  nom::uint32 idx = 0;
  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_HEIGHT; x++ )
    {
      this->grid[x][y].update ( nom::Point2i( x, y ), cards_buffer[idx] );
      idx++;
    }
  }

  return true;
}

const nom::int32 Board::operator() ( const nom::int32 x, const nom::int32 y )
{
  return this->getStatus ( x, y );
}

void Board::list ( void )
{
  nom::uint32 line_number = 1;

  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      Card tile = this->grid[x][y].tile();
      nom::IntRect pos = this->grid[x][y].bounds();
      nom::uint32 element = this->grid[x][y].element();

      std::cout << line_number
                << ". "
                << tile.getName()
                << " is at pos "
                << pos.x
                << ", "
                << pos.y
                << " ("
                << pos.w
                << "x"
                << pos.h
                << ") "
                << " with an element ID of "
                << element
                << "."
                << "\n";

    line_number++;
    }
  }
  std::cout << "\n";
}

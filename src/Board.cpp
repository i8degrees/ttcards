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
  this->board_map[0] = nom::Coords ( 0, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );
  this->board_map[1] = nom::Coords ( 1, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );
  this->board_map[2] = nom::Coords ( 2, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );

  this->board_map[3] = nom::Coords ( 0, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );
  this->board_map[4] = nom::Coords ( 1, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );
  this->board_map[5] = nom::Coords ( 2, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );

  this->board_map[6] = nom::Coords ( 0, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );
  this->board_map[7] = nom::Coords ( 1, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );
  this->board_map[8] = nom::Coords ( 2, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );

  this->grid.resize ( BOARD_GRID_HEIGHT );

  for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    this->grid[x].resize ( BOARD_GRID_WIDTH );

  nom::int32 idx = 0;
  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      nom::uint32 random_element = nom::randomInteger ( 0, MAX_ELEMENT );
      this->grid[x][y] = BoardTile ( Card(), this->board_map[idx], random_element );
      idx++; // this->board_map[0..8]
    }
  }
}

const nom::Coords Board::getGlobalBounds ( nom::int32 x, nom::int32 y ) const
{
  nom::Coords pos ( -1, -1 ); // ...when all else fails, default to undefined

  for ( nom::int32 idx = 0; idx < ( BOARD_GRID_WIDTH * BOARD_GRID_HEIGHT ); idx++ )
  {
    if ( x <= this->board_map[idx].width && x >= BOARD_ORIGIN_X && y <= this->board_map[idx].height && y >= BOARD_ORIGIN_Y )
      return this->board_map[idx];
  }

  return pos;
}

const std::vector<Card> Board::find_adjacent ( nom::int32 x, nom::int32 y ) const
{
  std::vector<Card> adjacent_cards;

  nom::int32 cols = y;
  nom::int32 rows = x;

  // Compare card's WEST rank with opponent's EAST rank
  if ( rows != 0 )
  {
    if ( this->getStatus ( rows - 1, cols ) != 0 )
    {
      adjacent_cards.push_back ( this->get ( rows - 1, cols ) );
    }
  }

  // Compare card's EAST rank with opponent's WEST rank
  if ( rows != BOARD_GRID_WIDTH - 1 )
  {
    if ( this->getStatus ( rows + 1, cols ) != 0 )
    {
      adjacent_cards.push_back ( this->get ( rows + 1, cols ) );
    }
  }

  // Compare card's NORTH rank with opponent's SOUTH rank
  if ( cols != 0 )
  {
    if ( this->getStatus ( rows, cols - 1 ) != 0 )
    {
      adjacent_cards.push_back ( this->get ( rows, cols - 1 ) );
    }
  }

  // Compare card's SOUTH rank with opponent's NORTH rank
  if ( cols != BOARD_GRID_HEIGHT - 1 )
  {
    if ( this->getStatus ( rows, cols + 1 ) != 0 )
    {
      adjacent_cards.push_back ( this->get ( rows, cols + 1 ) );
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
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows - 1, cols ) && getStatus ( rows - 1, cols ) != 0 )
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
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols + 1 ) && getStatus ( rows, cols + 1 ) != 0 )
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
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows + 1, cols ) && getStatus ( rows + 1, cols ) != 0 )
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
        if ( getPlayerID ( rows, cols ) != getPlayerID ( rows, cols - 1 ) && getStatus ( rows, cols - 1 ) != 0 )
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
      if ( this->getStatus ( x, y ) != 0 )
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
    return this->grid[x][y].tile_card.getID(); // FIXME
  else
    return -1;
}

void Board::updateStatus ( nom::int32 x, nom::int32 y, const Card& card )
{
  this->grid[x][y].update ( nom::Coords ( x, y ), card );
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

void Board::update ( nom::int32 x, nom::int32 y ) {}

void Board::draw ( void* video_buffer )
{
  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      this->card->DrawCard ( video_buffer, this->get ( x, y ), BOARD_ORIGIN_X + ( CARD_WIDTH * x ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * y ) );
    }
  }
}

bool Board::save ( const std::string& filename )
{
  nom::JSON::FileWriter fp; // json_spirit wrapper for file output
  json_spirit::Array game; // Overall container; this is the parent
  json_spirit::Object node; // JSON object record; the child

  for ( nom::int32 y = 0; y != BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x != BOARD_GRID_WIDTH; x++ )
    {
      // Primary card attributes
      node = this->grid[x][y].tile_card.serialize();

      // Additional card attributes to append onto
      node.push_back ( json_spirit::Pair ( "PlayerID", this->grid[x][y].tile_card.getPlayerID() ) );
      node.push_back ( json_spirit::Pair ( "Owner", this->grid[x][y].tile_card.getPlayerOwner() ) );

      // Push current node to our overall game data to be written
      game.push_back ( node );

      // Get ready for the next inbound row
      node.clear();
    }
  }

  if ( fp.save ( filename, game, nom::JSON::CompactArrays ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    return false;
  }

  return true;
}

bool Board::load ( const std::string& filename )
{
  nom::JSON::FileReader fp; // json_spirit wrapper for file input
  json_spirit::Object node;
  json_spirit::Value value;
  json_spirit::Array game;
  // Iterators
  json_spirit::Array::size_type i;
  json_spirit::Object::size_type o;

  // The card attributes we are loading in will be stored in here temporarily.
  // This will become the data to load onto the board if all goes well..!
  Card card;
  std::vector<Card> input_cards;

  if ( fp.load ( filename, value ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

NOM_ASSERT ( value.type() == json_spirit::array_type );
  game = value.get_array();

  for ( i = 0; i != game.size(); i++ )
  {
NOM_ASSERT ( game[i].type() == json_spirit::obj_type );
    node = game[i].get_obj();

    for ( o = 0; o != node.size(); o++ )
    {
      const json_spirit::Pair& pair = node[o];
      const std::string& path = pair.name_;
      const json_spirit::Value& value = pair.value_;

      if ( path == "ID" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setID ( value.get_int() );
      }
      else if ( path == "Name" )
      {
NOM_ASSERT ( value.type() == json_spirit::str_type );
        card.setName ( value.get_str() );
      }
      else if ( path == "Level" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setLevel ( value.get_int() );
      }
      else if ( path == "Type" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setType ( value.get_int() );
      }
      else if ( path == "Element" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setElement ( value.get_int() );
      }
      else if ( path == "Ranks" )
      {
NOM_ASSERT ( value.type() == json_spirit::array_type );
        const json_spirit::Array &ranks = value.get_array();

NOM_ASSERT ( ranks.size() == 4 );
        card.setNorthRank ( ranks[NORTH].get_int() );
        card.setEastRank ( ranks[EAST].get_int() );
        card.setSouthRank ( ranks[SOUTH].get_int() );
        card.setWestRank ( ranks[WEST].get_int() );
      }
      else if ( path == "PlayerID" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setPlayerID ( value.get_int() );
      }
      else if ( path == "Owner" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setPlayerOwner ( value.get_int() );

        // The next line *MUST* be at the end of the current node object being
        // read in -- or else epic failure will result!
        input_cards.push_back ( card );
      }
    } // end current node loop
  } // end current array node

  // Sanity check
  if ( input_cards.size() < 8 ) // + 1 padding; we start at zero, not one
  {
NOM_LOG_ERR ( TTCARDS, "Board data is invalid at file: " + filename );
    return false;
  }

  // All is well, let us make our freshly loaded data permanent;
  //
  // Load each card object onto the board grid at the proper X, Y coordinates.
  nom::int32 idx = 0;
  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_HEIGHT; x++ )
    {
      this->grid[x][y].update ( nom::Coords ( x, y ), input_cards[idx] );
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
      nom::Coords pos = this->grid[x][y].bounds();
      nom::uint32 element = this->grid[x][y].element();

      std::cout << line_number
                << ". "
                << tile.getName()
                << " is at pos "
                << pos.x
                << ", "
                << pos.y
                << " ("
                << pos.width
                << "x"
                << pos.height
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

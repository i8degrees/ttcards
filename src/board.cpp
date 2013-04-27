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
      this->grid[x][y].id = 0;
    }
  }
}

Board::~Board ( void )
{
  #ifdef DEBUG_BOARD_OBJ
    std::cout << "Board::~Board (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  if ( this->background != NULL )
  {
    SDL_FreeSurface ( this->background );
    this->background = NULL;
  }
}

void Board::Init ( CardHand &player1_cards, CardHand &player2_cards )
{
  this->player1_hand = &player1_cards;
  this->player2_hand = &player2_cards;
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

Card & Board::CompareCards ( unsigned int x, unsigned int y, Card &card )
{
  #ifdef DEBUG_BOARD_CMP
    std::cout << std::endl << std::endl;
    std::cout << "Comparing:" << std::endl;
  #endif

  if ( x == 0 && y == 0 ) // 0, 0
  {
    if ( this->grid[1][0].id != 0 && card.player_id != this->grid[1][0].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][0].id << ' ';
      #endif

      if ( card.rank[1] > this->grid[1][0].rank[3] )
      {
      #ifdef DEBUG_BOARD_CMP
        std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][0].id << std::endl;
      #endif

      return this->grid[1][0];
      }
    }

    std::cout << std::endl;

    if ( this->grid[0][1].id != 0 && card.player_id != this->grid[0][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[0][1].id << ' ';
      #endif

      if ( card.rank[2] > this->grid[0][1].rank[0] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][1].id << std::endl;
        #endif

        return this->grid[0][1];
      }
    }
    std::cout << std::endl;
  }

  if ( x == 1 && y == 0 ) // 1, 0
  {
    if ( this->grid[2][0].id != 0 && card.player_id != this->grid[2][0].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[2][0].id << ' ';
      #endif

      if ( card.rank[1] > this->grid[2][0].rank[3] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][0].id << std::endl;
        #endif

        return this->grid[2][0];
      }
    }
    std::cout << std::endl;

    if ( this->grid[1][1].id != 0 && card.player_id != this->grid[1][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][1].id << ' ';
      #endif

      if ( card.rank[2] > this->grid[1][1].rank[0] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][1].id << std::endl;
        #endif

        return this->grid[1][1];
      }
    }
    std::cout << std::endl;

    if ( this->grid[0][0].id != 0 && card.player_id != this->grid[0][0].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[0][0].id << ' ';
      #endif

      if ( card.rank[3] > this->grid[0][0].rank[1] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][0].id << std::endl;
        #endif

        return this->grid[0][0];
      }
    }
    std::cout << std::endl;
  }

  if ( x == 2 && y == 0 ) // 2, 0
  {
    if ( this->grid[2][1].id != 0 && card.player_id != this->grid[2][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[2][1].id << ' ';
      #endif

      if ( card.rank[2] > this->grid[2][1].rank[0] )
      {
      #ifdef DEBUG_BOARD_CMP
        std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][1].id << std::endl;
      #endif

      return this->grid[2][1];
      }
    }
    std::cout << std::endl;
    if ( this->grid[1][0].id != 0 && card.player_id != this->grid[1][0].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][0].id << ' ';
      #endif

      if ( card.rank[3] > this->grid[1][0].rank[1] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][0].id << std::endl;
        #endif

        return this->grid[1][0];
      }
    }
    std::cout << std::endl;
  }

  if ( x == 0 && y == 1 ) // 0, 1
  {
    if ( this->grid[0][0].id != 0 && card.player_id != this->grid[0][0].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[0][0].id << ' ';
      #endif
      if ( card.rank[0] > this->grid[0][0].rank[2] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][0].id << std::endl;
        #endif

        return this->grid[0][0];
      }
    }
    std::cout << std::endl;
    if ( this->grid[1][1].id != 0 && card.player_id != this->grid[1][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][1].id << ' ';
      #endif
      if ( card.rank[1] > this->grid[1][1].rank[3] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][1].id << std::endl;
        #endif

        return this->grid[1][1];
      }
    }
    std::cout << std::endl;
    if ( this->grid[0][2].id != 0 && card.player_id != this->grid[0][2].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[0][2].id << ' ';
      #endif
      if ( card.rank[2] > this->grid[0][2].rank[0] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][2].id << std::endl;
        #endif

        return this->grid[0][2];
      }
    }
    std::cout << std::endl;
  }

  if ( x == 1 && y == 1 ) // 1, 1
  {
    if ( this->grid[1][0].id != 0 && card.player_id != this->grid[1][0].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][0].id << ' ';
      #endif
      if ( card.rank[0] > this->grid[1][0].rank[2] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][0].id << std::endl;
        #endif

        return this->grid[1][0];
      }
    }
    std::cout << std::endl;
    if ( this->grid[2][1].id != 0 && card.player_id != this->grid[2][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[2][1].id << ' ';
      #endif
      if ( card.rank[1] > this->grid[2][1].rank[3] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][1].id << std::endl;
        #endif

        return this->grid[2][1];
      }
    }
    std::cout << std::endl;
    if ( this->grid[1][2].id != 0 && card.player_id != this->grid[1][2].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][2].id << ' ';
      #endif
      if ( card.rank[2] > this->grid[1][2].rank[0] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][2].id << std::endl;
        #endif

        return this->grid[1][2];
      }
    }
    std::cout << std::endl;
    if ( this->grid[0][1].id != 0 && card.player_id != this->grid[0][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[0][1].id << ' ';
      #endif
      if ( card.rank[3] > this->grid[0][1].rank[1] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][1].id << std::endl;
        #endif

        return this->grid[0][1];
      }
    }
    std::cout << std::endl;
  }

  if ( x == 2 && y == 1 ) // 2, 1
  {
    if ( this->grid[2][0].id != 0 && card.player_id != this->grid[2][0].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[2][0].id << ' ';
      #endif
      if ( card.rank[0] > this->grid[2][0].rank[2] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][0].id << std::endl;
        #endif

        return this->grid[2][0];
      }
    }
    std::cout << std::endl;
    if ( this->grid[2][2].id != 0 && card.player_id != this->grid[2][2].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[2][2].id << ' ';
      #endif
      if ( card.rank[2] > this->grid[2][2].rank[0] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][2].id << std::endl;
        #endif

        return this->grid[2][2];
      }
    }
    std::cout << std::endl;
    if ( this->grid[1][1].id != 0 && card.player_id != this->grid[1][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][1].id << ' ';
      #endif
      if ( card.rank[3] > this->grid[1][1].rank[1] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][1].id << std::endl;
        #endif

        return this->grid[1][1];
      }
    }
    std::cout << std::endl;
  }

  if ( x == 0 && y == 2 ) // 0, 2
  {
    if ( this->grid[0][1].id != 0 && card.player_id != this->grid[0][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[0][1].id << ' ';
      #endif
      if ( card.rank[0] > this->grid[0][1].rank[2] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][1].id << std::endl;
        #endif

        return this->grid[0][1];
      }
    }
    std::cout << std::endl;
    if ( this->grid[1][2].id != 0 && card.player_id != this->grid[1][2].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][2].id << ' ';
      #endif
      if ( card.rank[1] > this->grid[1][2].rank[3] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][2].id << std::endl;
        #endif

        return this->grid[1][2];
      }
    }
    std::cout << std::endl;
  }

  if ( x == 1 && y == 2 ) // 1, 2
  {
    if ( this->grid[1][1].id != 0 && card.player_id != this->grid[1][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][1].id << ' ';
      #endif
      if ( card.rank[0] > this->grid[1][1].rank[2] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][1].id << std::endl;
        #endif

        return this->grid[1][1];
      }
    }
    std::cout << std::endl;
    if ( this->grid[2][2].id != 0 && card.player_id != this->grid[2][2].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[2][2].id << ' ';
      #endif
      if ( card.rank[1] > this->grid[2][2].rank[3] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][2].id << std::endl;
        #endif

        return this->grid[2][2];
      }
    }
    std::cout << std::endl;
    if ( this->grid[0][2].id != 0 && card.player_id != this->grid[0][2].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[0][2].id << ' ';
      #endif
      if ( card.rank[3] > this->grid[0][2].rank[1] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[0][2].id << std::endl;
        #endif

        return this->grid[0][2];
      }
    }
    std::cout << std::endl;
  }

  if ( x == 2 && y == 2 ) // 2, 2
  {
    if ( this->grid[2][1].id != 0 && card.player_id != this->grid[2][1].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[2][1].id << ' ';
      #endif
      if ( card.rank[0] > this->grid[2][1].rank[2] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[2][1].id << std::endl;
        #endif

        return this->grid[2][1];
      }
    }
    std::cout << std::endl;
    if ( this->grid[1][2].id != 0 && card.player_id != this->grid[1][2].player_id )
    {
      #ifdef DEBUG_BOARD_CMP
        std::cout << "adj " << this->grid[1][2].id << ' ';
      #endif
      if ( card.rank[3] > this->grid[1][2].rank[1] )
      {
        #ifdef DEBUG_BOARD_CMP
          std::cout << std::endl << card.id << ' ' << "wins against" << ' ' << this->grid[1][2].id << std::endl;
        #endif

        return this->grid[1][2];
      }
    }
    std::cout << std::endl;
  }

  #ifdef DEBUG_BOARD_CMP
    std::cout << "\nEnd of comparison." << std::endl;
  #endif

  return card;
}

unsigned int Board::GetCount ( unsigned int pid )
{
  unsigned int pid_count = 0;

  for ( int idx = 0; idx < this->board_hand.cards.size(); idx++ )
  {
    if ( this->board_hand.cards[idx].player_id == pid )
      pid_count += 1;
  }
  return pid_count;
}

unsigned int Board::GetStatus ( unsigned int x, unsigned int y )
{
  return this->grid[x][y].id;
}

void Board::UpdateBoard ( unsigned int x, unsigned int y, Card &card )
{
  Card opponent;
  signed int pos = -1;

  this->grid[x][y] = card;
  this->board_hand.cards.push_back ( card );

  opponent = this->CompareCards ( x, y, card );

  if ( card.player_id != opponent.player_id )
  {
    pos = this->board_hand.CardPosition ( opponent );

    if ( pos != -1 )
    {
      if ( this->board_hand.cards[pos].player_id == 0 )
      {
        this->board_hand.cards[pos].player_id = 1;
      }
      else // opponent.player_id == 1
      {
        this->board_hand.cards[pos].player_id = 0;
      }
    }
  }
}

void Board::ListContents ( void )
{
  int x, y = 0;

  for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      std::cout << this->grid[x][y].id << " " << this->grid[x][y].player_id << " ";
    }
    std::cout << std::endl << std::endl;
  }
  std::cout << "Board Hand:" << std::endl;
  for ( int idx = 0; idx < this->board_hand.cards.size(); idx++ )
  {
    std::cout << this->board_hand.cards[idx].id << " " << this->board_hand.cards[idx].name << " " << this->board_hand.cards[idx].player_id << " ";
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void Board::DrawBoard ( Gfx *engine )
{
  unsigned int board_index = 0;
  unsigned int x, y = 0;

  for ( board_index = 0; board_index < this->board_hand.cards.size(); board_index++ )
  {
    if ( this->board_hand.isValid ( this->board_hand.cards[board_index] ) == true )
    {
      for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
      {
        for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
        {
          if ( this->GetStatus ( x, y ) == this->board_hand.cards[board_index].id )
            this->card.DrawCard ( engine, this->board_hand.cards[board_index], BOARD_ORIGIN_X + ( CARD_WIDTH * x ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * y ) );
        }
      }
    }
  }
}

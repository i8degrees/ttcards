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

void Board::checkBoard ( unsigned int x, unsigned int y, Card &card )
{
  unsigned int cols, rows = 0;

  for ( cols = y; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( rows = x; x < BOARD_GRID_WIDTH; x++ )
    {
      if ( cols != 2 )
      {
        if ( card.player_id != this->GetPlayerID ( rows, cols + 1 ) && this->GetStatus ( rows, cols + 1 ) !=0 )
        {
          if ( this->rules->CompareCards ( card.rank[2], this->grid[rows][cols+1].rank[0] ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << card.id << " " << "wins against" << " " << this->GetStatus ( rows, cols + 1 ) << std::endl << std::endl;
            #endif

            this->UpdatePlayerID ( rows, cols + 1, card.player_id );

            if ( this->rules->GetRules() == 0 )
              return;
            else
              continue;
          }
        }
      }

      if ( rows != 2 )
      {
        if ( card.player_id != this->GetPlayerID ( rows + 1, cols ) && this->GetStatus ( rows + 1, cols ) !=0 )
        {
          if ( this->rules->CompareCards ( card.rank[1], this->grid[rows+1][cols].rank[3] ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << card.id << " " << "wins against" << " " << this->GetStatus ( rows + 1, cols ) << std::endl << std::endl;
            #endif

            this->UpdatePlayerID ( rows + 1, cols, card.player_id );

            if ( this->rules->GetRules() == 0 )
              return;
            else
              continue;
          }
        }
      }

      if ( cols != 0 )
      {
        if ( card.player_id != this->GetPlayerID ( rows, cols - 1 ) && this->GetStatus ( rows, cols - 1 ) !=0 )
        {
          if ( this->rules->CompareCards ( card.rank[0], this->grid[rows][cols-1].rank[2] ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << card.id << " " << "wins against" << " " << this->GetStatus ( rows, cols - 1 ) << std::endl << std::endl;
            #endif

            this->UpdatePlayerID ( rows, cols - 1, card.player_id );

            if ( this->rules->GetRules() == 0 )
              return;
            else
              continue;
          }
        }
      }

      if ( rows != 0 )
      {
        if ( card.player_id != this->GetPlayerID ( rows - 1, cols ) && this->GetStatus ( rows - 1, cols ) !=0 )
        {
          if ( this->rules->CompareCards ( card.rank[3], this->grid[rows-1][cols].rank[1] ) == true )
          {
            #ifdef DEBUG_BOARD_CMP
              std::cout << std::endl << card.id << " " << "wins against" << " " << this->GetStatus ( rows - 1, cols ) << std::endl << std::endl;
            #endif

            this->UpdatePlayerID ( rows - 1, cols, card.player_id );

            if ( this->rules->GetRules() == 0 )
              return;
            else
              continue;
          }
        }
      }
    } // rows for loop
  } // cols for loop

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
      if ( this->GetPlayerID ( x, y ) != 0 && this->GetPlayerID ( x, y ) == pid )
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

unsigned int Board::GetPlayerID ( unsigned int x, unsigned int y )
{
  return this->grid[x][y].player_id;
}

void Board::UpdatePlayerID ( unsigned int x, unsigned int y, unsigned int player )
{
  this->grid[x][y].player_id = player;
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
      std::cout << this->GetStatus ( x, y ) << " " << this->GetPlayerID ( x, y ) << " ";
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

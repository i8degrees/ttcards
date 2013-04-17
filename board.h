/******************************************************************************
    board.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef BOARD_HEADERS
#define BOARD_HEADERS

#include "SDL/SDL.h"
#include <iostream>
#include <string>

#include "cfg.h"
#include "gfx.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"

#define DEBUG_BOARD
#define DEBUG_BOARD_OBJ

class Board
{
  public:
    Board ( void );
    ~Board ( void );

    void Init ( CardHand &player1_cards, CardHand &player2_cards );

    bool LoadBackground ( std::string filename );
    bool DrawBackground ( Gfx &engine );

    unsigned int GetStatus ( unsigned int x, unsigned int y );

    void UpdateBoard ( unsigned int x, unsigned int y, unsigned int state );
    void ListContents ( void );
    void DrawBoard ( Gfx &engine );

  private:
    SDL_Surface *background;
    Gfx engine;
    CardDebug debug;
    CardView card;
    CardHand *player1_hand;
    CardHand *player2_hand;
    unsigned int grid[3][3];
};

#endif // BOARD_HEADERS defined

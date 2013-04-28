/******************************************************************************
    board.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_BOARD_HEADERS
#define GAMEAPP_BOARD_HEADERS

#include <iostream>
#include <string>

#include "gamelib/gamelib.h"

#include "cfg.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"

class Board
{
  public:
    Board ( void );
    ~Board ( void );

    bool LoadBackground ( std::string filename );
    bool DrawBackground ( Gfx *engine );

    void checkBoard ( unsigned int x, unsigned int y, Card &card );

    // TODO: Consider branching this into Score class
    unsigned int GetCount ( unsigned int player_id );

    unsigned int GetStatus ( unsigned int x, unsigned int y );
    void UpdateBoard ( unsigned int x, unsigned int y, Card &card );
    void ListContents ( void );
    void DrawBoard ( Gfx *engine );

  private:
    SDL_Surface *background;
    CardDebug debug;
    CardView card;
    std::vector<std::vector<Card>> grid;
};

#endif // BOARD_HEADERS defined

/******************************************************************************
    board.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_BOARD_HEADERS
#define GAMEAPP_BOARD_HEADERS

#include <iostream>
#include <string>
#include <utility>

#include "gamelib.h"

#include "cfg.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"
#include "card_rules.h"

class Board
{
  public:
    Board ( void );
    ~Board ( void );

    void Init ( CardView *card_gfx, CardRules *rules );

    bool LoadBackground ( std::string filename );
    bool DrawBackground ( Gfx *engine );

    std::vector<std::pair<int, int>> checkBoard ( unsigned int x, unsigned int y );

    // TODO: Consider branching this into Score class
    unsigned int GetPlayerCardCount ( unsigned int player_id );

    unsigned int GetTotalCount ( void );

    unsigned int GetStatus ( unsigned int x, unsigned int y );
    void flipCard ( unsigned int x, unsigned int y, unsigned int player_id );
    unsigned int getPlayerID ( unsigned int x, unsigned int y );

    void updateStatus ( unsigned int x, unsigned int y, Card &card );
    void updateBoard ( unsigned int x, unsigned int y );
    void ListContents ( void );
    void DrawBoard ( Gfx *engine );

  private:
    SDL_Surface *background;
    CardDebug debug;
    CardView *card;
    CardRules *rules;
    std::vector<std::vector<Card>> grid;
};

#endif // BOARD_HEADERS defined

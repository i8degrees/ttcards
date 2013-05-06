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

    std::pair <int, int> checkBoard ( unsigned int x, unsigned int y, Card &card ); //bool checkBoard ( unsigned int x, unsigned int y, Card &card );

    // TODO: Consider branching this into Score class
    unsigned int GetPlayerCardCount ( unsigned int player_id );

    unsigned int GetTotalCount ( void );

    unsigned int GetStatus ( unsigned int x, unsigned int y );

    void UpdateBoard ( unsigned int x, unsigned int y, Card &card );
    void ListContents ( void );
    void DrawBoard ( Gfx *engine );

  private:
    SDL_Surface *background;
    CardDebug debug;
    CardView *card;
    CardRules *rules;
    std::vector<std::vector<Card>> grid;

    unsigned int GetPlayerID ( unsigned int x, unsigned int y );
    void UpdatePlayerID ( unsigned int x, unsigned int y, unsigned int player_id );
};

#endif // BOARD_HEADERS defined

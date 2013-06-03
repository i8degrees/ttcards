/******************************************************************************
    board.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_BOARD_HEADERS
#define GAMEAPP_BOARD_HEADERS

#include <iostream>
#include <string>
#include <utility>

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

    std::vector<std::pair<int, int>> checkBoard ( unsigned int x, unsigned int y );

    // TODO: Consider branching this into Score class
    unsigned int getCount ( void );
    unsigned int getPlayerCount ( unsigned int player_id );

    unsigned int getStatus ( unsigned int x, unsigned int y );
    void updateStatus ( unsigned int x, unsigned int y, Card &card );
    unsigned int getPlayerID ( unsigned int x, unsigned int y );
    void flipCard ( unsigned int x, unsigned int y, unsigned int player_id );
    std::string getName ( unsigned int x, unsigned int y );
    Card &getCard ( unsigned int x, unsigned int y );

    void Update ( unsigned int x, unsigned int y ); // TODO
    void Draw ( SDL_Surface *video_buffer );
    void List ( void );

  private:
    CardDebug debug; // debug support for card attributes
    CardView *card; // pointer referencing our card rendering class
    CardRules *rules;
    std::vector<std::vector<Card>> grid; // 2D vector of Card data containers
};

#endif // BOARD_HEADERS defined
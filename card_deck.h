/******************************************************************************
    card_deck.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_DECK_HEADERS
#define CARD_DECK_HEADERS

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "SDL/SDL.h"

#include "cfg.h"

#include "card.h"

//#define DEBUG_CARD_DECK

class Deck
{
  public:
    Deck ( void );
    ~Deck ( void );

    bool Load ( std::string filename = CARDS_DB );

    std::vector<Card> cards;

  private:
    // ...
};

#endif // CARD_DECK_HEADERS defined

/******************************************************************************
    card_collection.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_COLLECTION_HEADERS
#define CARD_COLLECTION_HEADERS

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

//#define DEBUG_DECK
//#define DEBUG_DEALER

class Deck
{
  public:
    Deck ( void );
    ~Deck ( void );

    bool Load ( std::string filename = CARDS_DB );
    bool List ( void ); // debug

    std::vector<Card> cards;

  private:
    // ...
};

class Dealer
{
  public:
    Dealer ( void );
    ~Dealer ( void );

    bool Init ( std::vector<Card> &card );

    Card Random ( void );
    Card Deal ( void );
    Card selectCard ( int id );

    std::vector<Card> cards;

  private:
    // ...
};

#endif // CARD_COLLECTION_HEADERS defined

/******************************************************************************
    card_dealer.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_DEALER_HEADERS
#define CARD_DEALER_HEADERS

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "SDL.h"

#include "cfg.h"

#include "card.h"

//#define DEBUG_CARD_DEALER

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

#endif // CARD_DEALER_HEADERS defined

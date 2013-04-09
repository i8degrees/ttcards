/******************************************************************************
    card_hand.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_HAND_HEADERS
#define CARD_HAND_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "card.h"
#include "card_collection.h"

#define DEBUG_CARDHAND

class CardHand
{
  public:
    CardHand ( void );
    ~CardHand ( void );

    bool Add ( Card card );

    std::vector<Card> deck;
  private:
    // ...
};

#endif // CARD_HAND_HEADERS defined

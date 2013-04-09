/******************************************************************************
    card_debug.h

    Debugging Class for Card objects ( including <Card> vectors )

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_DEBUG_HEADERS
#define CARD_DEBUG_HEADERS

#include <iostream>
#include <string>

#include "card.h"

//#define DEBUG_CARD_DEBUG

class CardDebug: public Card
{
  public:
    CardDebug();
    ~CardDebug();

    void ListCard ( Card card );
    void ListCards ( std::vector<Card> &cards );

  private:
    // ...
};

#endif // CARD_DEBUG_HEADERS defined

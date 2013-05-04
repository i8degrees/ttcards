/******************************************************************************
    card_debug.h

    Debugging Class for Card objects ( including <Card> vectors )

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_DEBUG_HEADERS
#define GAMEAPP_CARD_DEBUG_HEADERS

#include <iostream>
#include <string>

#include "card.h"

class CardDebug
{
  public:
    CardDebug();
    ~CardDebug();

    void ListCard ( Card &card );
    void ListCards ( std::vector<Card> &cards );

  private:
    // ...
};

#endif // CARD_DEBUG_HEADERS defined

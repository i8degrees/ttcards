/******************************************************************************

    Debugging Class for Card deriving objects

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_DEBUG_HEADERS
#define GAMEAPP_CARD_DEBUG_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "card.h"
#include "cfg.h"

class CardDebug
{
  public:
    CardDebug();
    ~CardDebug();

    void ListCard ( Card& card );
    void ListCards ( std::vector<Card>& cards );

  private:
    // ...
};

#endif // GAMEAPP_CARD_DEBUG_HEADERS defined

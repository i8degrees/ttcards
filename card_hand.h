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

#define DEBUG_CARD_HAND
//#define DEBUG_CARD_HAND_OBJ

class CardHand
{
  public:
    CardHand ( void );
    ~CardHand ( void );

    bool AddCard ( Card &card );
    //bool RemoveCard ( Card &card );
    bool SelectedCard ( Card &card );

    std::vector<Card> cards;
  private:
    // ...
    Card *selectedCard;
};

#endif // CARD_HAND_HEADERS defined

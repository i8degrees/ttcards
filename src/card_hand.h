/******************************************************************************
    card_hand.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_HAND_HEADERS
#define GAMEAPP_CARD_HAND_HEADERS

#include <iostream>
#include <string>

#include "card.h"

class CardHand
{
  public:
    CardHand ( void );
    ~CardHand ( void );

    bool AddCard ( Card &card );
    bool RemoveCard ( Card &card );
    bool isValid ( Card &card );
    bool isEmpty ( void );
    void ClearSelected ( void );
    Card & GetSelectedCard ( void );
    bool SelectCard ( Card &card );
    unsigned int CardCount ( void );
    signed int CardPosition ( Card &card );

    std::vector<Card> cards;

    unsigned int card_pos; // keeps track of card index cursor is at
  private:
    Card selectedCard;
};

#endif // CARD_HAND_HEADERS defined

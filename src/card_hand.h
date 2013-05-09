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

    void ClearSelected ( void );
    Card & GetSelectedCard ( void );
    bool SelectCard ( Card &card );

    signed int getPrevCardIndex ( void );
    signed int getNextCardIndex ( void );
    unsigned int getCardIndex ( void );
    void setCardIndex ( signed int pos );

    bool isValid ( Card &card );
    bool isEmpty ( void );
    unsigned int getCount ( void );
    signed int CardPosition ( Card &card );

    std::vector<Card> cards;

  private:
    Card selectedCard; // holds player's selected card
    unsigned int card_pos; // keeps track of card index cursor is at
};

#endif // CARD_HAND_HEADERS defined

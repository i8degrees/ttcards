/******************************************************************************
    card_hand.h

  Helper Class for player defined cards

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

    void clearSelectedCard ( void );
    Card & getSelectedCard ( void );
    bool selectCard ( Card &card );

    bool isValid ( Card &card );
    bool isEmpty ( void );
    unsigned int getCount ( void );
    signed int cardPosition ( Card &card );


    std::vector<Card> cards; // TODO: declare in private scope

  private:
    Card selectedCard; // holds player's selected (think: ready to place) card
};

#endif // CARD_HAND_HEADERS defined

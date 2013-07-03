/******************************************************************************

    Helper Class for player defined cards

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_HAND_HEADERS
#define GAMEAPP_CARD_HAND_HEADERS

#include <iostream>
#include <string>

#include "card.h"
#include "cfg.h"

class CardHand
{
  public:
    CardHand ( void );
    ~CardHand ( void );

    bool addCard ( Card& card );
    bool removeCard ( Card& card );

    void clearSelectedCard ( void );
    Card& getSelectedCard ( void );
    bool selectCard ( Card& card );

    bool isValid ( Card& card );

    void clear ( void );
    bool empty ( void );
    unsigned int size ( void );
    signed int pos ( Card& card );

    /// \internal
    ///
    /// TODO: declare in private scope
    /// \endinternal
    std::vector<Card> cards;

  private:
    /// holds player's selected (think: ready to place) card
    Card selectedCard;
};

#endif // GAMEAPP_CARD_HAND_HEADERS defined

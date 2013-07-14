/******************************************************************************

    Helper Class for player defined cards

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_HAND_HEADERS
#define GAMEAPP_CARD_HAND_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include <nomlib/types.hpp>

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
    nom::int32 size ( void );
    nom::int32 pos ( Card& card );

    /// \todo Declare in private scope
    std::vector<Card> cards;

  private:
    /// holds player's selected (think: ready to place) card
    Card selectedCard;
};

#endif // GAMEAPP_CARD_HAND_HEADERS defined

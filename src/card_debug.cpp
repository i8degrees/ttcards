/******************************************************************************
    card_debug.cpp

  Debugging for Card objects ( including <Card> vectors )

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_debug.h"

CardDebug::CardDebug ( void )
{
  #ifdef DEBUG_CARD_DEBUG_OBJ
    std::cout << "CardDebug::CardDebug(): " << "Hello, world!" << "\n" << std::endl;
  #endif
}

CardDebug::~CardDebug ( void )
{
  #ifdef DEBUG_CARD_DEBUG_OBJ
    std::cout << "CardDebug::~CardDebug(): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

void CardDebug::ListCard ( Card &card )
{
  std::array<int, 4> ranks = { { 0, 0, 0, 0 } };

  if ( card.getID() == 0 )
  {
    #ifdef DEBUG_CARD_DEBUG
      std::cout << "CardDebug::ListCard(): " << "Card is empty." << "\n" << std::endl;
    #endif
  }

  std::cout << card.getID();
  std::cout << " ";
  std::cout << card.getLevel();
  std::cout << " ";
  std::cout << card.getType();
  std::cout << " ";
  std::cout << card.getElement();
  std::cout << " ";

  for ( int rank = 0; rank < ranks.size(); rank++ )
  {
    std::cout << ranks[rank];
    std::cout << " ";
  }

  std::cout << card.getName();
  std::cout << " ";
  std::cout << card.getPlayerID();

  std::cout << std::endl;
}

void CardDebug::ListCards ( std::vector<Card> &cards )
{
  #ifdef DEBUG_CARD_DEBUG
    if ( cards.empty() == true )
    {
      std::cout << "CardDebug::ListCards(): " << "Cards vector is empty." << "\n" << std::endl;
    }
  #endif

  std::cout << "\nCardDebug::ListCards";
  std::cout << "\n====================";
  std::cout << "\n";

  if ( cards.empty() == true )
    std::cout << "N/A";

  for ( int i = 0; i < cards.size(); i++ )
  {
    std::cout << cards[i].getID();
    std::cout << " ";
    std::cout << cards[i].getLevel();
    std::cout << " ";
    std::cout << cards[i].getType();
    std::cout << " ";
    std::cout << cards[i].getElement();
    std::cout << " ";

    for ( int rank = 0; rank < 4; rank++ )
    {
      std::cout << cards[i].getNorthRank();
      std::cout << " ";

      std::cout << cards[i].getEastRank();
      std::cout << " ";

      std::cout << cards[i].getSouthRank();
      std::cout << " ";

      std::cout << cards[i].getWestRank();
      std::cout << " ";
    }

    std::cout << cards[i].getName();
    std::cout << " ";
    std::cout << cards[i].getPlayerID();

    std::cout << std::endl;
  }

  std::cout << "\n";
}

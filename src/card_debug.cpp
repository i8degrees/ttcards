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
  if ( card.id == 0 )
  {
    #ifdef DEBUG_CARD_DEBUG
      std::cout << "CardDebug::ListCard(): " << "Card is empty." << "\n" << std::endl;
    #endif
  }

  std::cout << card.id;
  std::cout << " ";
  std::cout << card.level;
  std::cout << " ";
  std::cout << card.type;
  std::cout << " ";
  std::cout << card.element;
  std::cout << " ";

  for ( int rdx = 0; rdx < card.rank.size(); rdx++ )
  {
    std::cout << card.rank[rdx];
    std::cout << " ";
  }

  std::cout << card.name;
  std::cout << " ";
  std::cout << card.player_id;

  std::cout << std::endl;
}

void CardDebug::ListCards ( std::vector<Card> &cards )
{
  #ifdef DEBUG_CARD_DEBUG
    if ( cards.empty() == true )
    {
      std::cout << "CardDebug::ListCards(): " << "Cards are empty." << "\n" << std::endl;
    }
  #endif

  for ( int i = 0; i < cards.size(); i++ )
  {
    std::cout << cards[i].id;
    std::cout << " ";
    std::cout << cards[i].level;
    std::cout << " ";
    std::cout << cards[i].type;
    std::cout << " ";
    std::cout << cards[i].element;
    std::cout << " ";

    for ( int rdx = 0; rdx < cards[i].rank.size(); rdx++ )
    {
      std::cout << cards[i].rank[rdx];
      std::cout << " ";
    }

    std::cout << cards[i].name;
    std::cout << " ";
    std::cout << cards[i].player_id;

    std::cout << std::endl;
  }
}

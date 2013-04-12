/******************************************************************************
    card_debug.cpp

  Debugging for Card objects ( including <Card> vectors )

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_debug.h"

CardDebug::CardDebug ( void )
{
  #ifdef DEBUG_CARD_DEBUG
    std::cout << "Hello, world! <From CardDebug::CardDebug>" << "\n" << std::endl;
  #endif
}

CardDebug::~CardDebug ( void )
{
  #ifdef DEBUG_CARD_DEBUG
    std::cout << "Goodbye cruel world! <From CardDebug::~CardDebug>" << "\n" << std::endl;
  #endif
}

void CardDebug::ListCard ( Card &card )
{
  std::cout << card.id;
  std::cout << " ";
  std::cout << card.level;
  std::cout << " ";
  std::cout << card.type;
  std::cout << " ";
  std::cout << card.element;
  std::cout << " ";

  for ( int rdx = 0; rdx < 4; rdx++ )
  {
    std::cout << card.rank[rdx];
    std::cout << " ";
  }

  std::cout << card.name;
  std::cout << " ";
  std::cout << card.face;

  std::cout << std::endl;
}

void CardDebug::ListCards ( std::vector<Card> &cards )
{
  if ( cards.empty() == true )
  {
    std::cout << "Card vector is empty. <From CardDebug::ListCards>" << "\n" << std::endl;
  }

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

    for ( int rdx = 0; rdx < 4; rdx++ )
    {
      std::cout << cards[i].rank[rdx];
      std::cout << " ";
    }

    std::cout << cards[i].name;
    std::cout << " ";
    std::cout << cards[i].face;

    std::cout << std::endl;
  }
}

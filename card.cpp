/******************************************************************************
    card.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card.h"

Card::Card (  unsigned int id, unsigned int level, unsigned int type,
              unsigned int element, std::array<int, 4> rank,
              std::string name, std::string face )

{
  #ifdef DEBUG_CARDS
    std::cout << "Hello, world! <From Card::Card>\n" << std::endl;
  #endif

  this->id = id;
  this->level = level;
  this->type = type;
  this->element = element;
  this->rank = rank;
  this->name = name;
  this->face = face;
}

Card::~Card ( void )
{
  #ifdef DEBUG_CARDS
    std::cout << "Good cruel world! <From Card::~Card>\n" << std::endl;
  #endif

  this->id = 0;
  this->level = 0;
  this->type = 0;
  this->element = 0;
  this->rank = { { 0, 0, 0, 0 } };
  this->name = "\0";
  this->face = "\0";
}

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

void CardDebug::ListCard ( Card card )
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
    #ifdef DEBUG
      std::cout << "Card vector is empty. <From CardDebug::ListCards>" << "\n" << std::endl;
    #endif
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

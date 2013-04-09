/******************************************************************************
    card_hand.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_hand.h"

CardHand::CardHand ( void )
{
  // ...
}

bool CardHand::Add ( std::vector<Card> &card )
{
  //std::cout << "Hello, world! <From CardHand::CardHand\n" << std::endl;

  //deck.push_back ( card );
  //for ( int i = 5; i < 10; i++ )
  //{
    //deck.push_back ( Card ( card[i].id, card.level, card[i].type, card[i].element,
                    //card[i].power[0], card[i].power[1], card[i].power[2], card[i].power[3],
                    //card[i].name ) );
  //}

  return true;

}

CardHand::~CardHand ( void )
{
  //std::cout << "Goodbye, cruel world! <From CardHand::~CardHand\n" << std::endl;
}

bool CardHand::List ( void )
{

  for ( int i = 0; i < deck.size(); i++ )
  {
    std::cout << deck[i].id << " ";
    std::cout << deck[i].level << " ";
    std::cout << deck[i].type << " ";
    std::cout << deck[i].element << " ";
    std::cout << deck[i].power[0] << " ";
    std::cout << deck[i].power[1] << " ";
    std::cout << deck[i].power[2] << " ";
    std::cout << deck[i].power[3] << " ";
    std::cout << deck[i].name;

    std::cout << std::endl;
  }

  return true;
}

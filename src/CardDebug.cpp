/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "CardDebug.hpp"

CardDebug::CardDebug ( void )
{
}

CardDebug::~CardDebug ( void )
{
}

void CardDebug::ListCard ( Card& card )
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

  for ( nom::ulong rank = 0; rank < ranks.size(); rank++ )
  {
    std::cout << ranks[rank];
    std::cout << " ";
  }

  std::cout << card.getName();
  std::cout << " ";
  std::cout << card.getPlayerID();

  std::cout << std::endl;
}

void CardDebug::ListCards ( std::vector<Card>& cards )
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

  for ( nom::ulong i = 0; i < cards.size(); i++ )
  {
    std::cout << cards[i].getID();
    std::cout << " ";
    std::cout << cards[i].getLevel();
    std::cout << " ";
    std::cout << cards[i].getType();
    std::cout << " ";
    std::cout << cards[i].getElement();
    std::cout << " ";

    std::cout << cards[i].getNorthRank();
    std::cout << ", ";

    std::cout << cards[i].getEastRank();
    std::cout << ", ";

    std::cout << cards[i].getSouthRank();
    std::cout << ", ";

    std::cout << cards[i].getWestRank();
    std::cout << " ";

    std::cout << cards[i].getName();
    std::cout << " ";
    std::cout << cards[i].getPlayerID();

    std::cout << std::endl;
  }

  std::cout << "\n";
}

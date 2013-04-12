/******************************************************************************
    card_collection.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_collection.h"

Collection::Collection ( void )
{
  #ifdef DEBUG_CARD_COLLECTION
    std::cout << "Hello, world! <From Collection::Collection>" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

Collection::~Collection ( void )
{
  #ifdef DEBUG_CARD_COLLECTION
    std::cout << "Goodbye cruel world! <From Collection::~Collection>" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

bool Collection::Load ( std::string filename )
{
  unsigned int id, level, type, element = 0;
  std::array<int, 4> rank = { { 0 } };
  std::string name, face = "\0";

  std::ifstream load ( filename );

  if ( ! load )
  {
    std::cout << "ERR in Collection::Load () at: " << filename << std::endl;
    return false;
  }

  #ifdef DEBUG_CARD_COLLECTION
    std::cout << filename << " " << "<From Collection::Load>" << "\n" << std::endl;
  #endif

  for ( int i = 0; i < ( MAX_DECKSET ); i++ ) // 110
  {
    load >> id;
    load >> level;
    load >> type;
    load >> element;
    load >> rank[0];
    load >> rank[1];
    load >> rank[2];
    load >> rank[3];
    load >> name;
    load >> face;

    #ifdef DEBUG_CARD_COLLECTION
      std::cout << id;
      std::cout << " ";
      std::cout << level;
      std::cout << " ";
      std::cout << type;
      std::cout << " ";
      std::cout << element;
      std::cout << " ";
      for ( int rdx = 0; rdx < 4; rdx++ )
      {
        std::cout << rank[rdx];
        std::cout << " ";
      }
      std::cout << name;
      std::cout << " ";
      std::cout << face;

      std::cout << std::endl;
    #endif // defined DEBUG_CARD_COLLECTION

    this->cards.push_back ( Card ( id, level, type, element, { { rank[0], rank[1], rank[2], rank[3] } }, name, face ) );
  }

  #ifdef DEBUG_CARD_COLLECTION
    std::cout << "EOF: " << filename << " " << "<From Collection::Load>" << "\n" << std::endl;
  #endif

  load.close();

  return true;
}

/******************************************************************************
    card_collection.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_collection.h"

Collection::Collection ( void )
{
  #ifdef DEBUG_CARD_COLLECTION_OBJ
    std::cout << "Hello, world! <From Collection::Collection>" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

Collection::~Collection ( void )
{
  #ifdef DEBUG_CARD_COLLECTION_OBJ
    std::cout << "Goodbye cruel world! <From Collection::~Collection>" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

bool Collection::Load ( std::string filename )
{
  int index = 0;
  unsigned int id, level, type, element = 0;
  std::array<int, 4> rank = { { 0 } };
  std::string name = "\0";

  std::ifstream load ( filename );

  if ( ! load )
  {
    #ifdef DEBUG_CARD_COLLECTION
      std::cout << "ERR in Collection::Load () at: " << filename << std::endl;
    #endif
    return false;
  }

  #ifdef DEBUG_CARD_COLLECTION
    std::cout << filename << " " << "<From Collection::Load>" << "\n" << std::endl;
  #endif

  for ( index = 0; index < ( MAX_COLLECTION ); index++ )
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

    #ifdef DEBUG_CARD_COLLECTION
      std::cout << id;
      std::cout << " ";
      std::cout << level;
      std::cout << " ";
      std::cout << type;
      std::cout << " ";
      std::cout << element;
      std::cout << " ";
      for ( int rank_index = 0; rank_index < 4; rank_index++ )
      {
        std::cout << rank[rank_index];
        std::cout << " ";
      }
      std::cout << name;

      std::cout << std::endl;
    #endif // defined DEBUG_CARD_COLLECTION

    this->cards.push_back ( Card ( id, level, type, element, { { rank[0], rank[1], rank[2], rank[3] } }, name, 0 ) );
  }

  #ifdef DEBUG_CARD_COLLECTION
    std::cout << "EOF: " << filename << " " << "<From Collection::Load>" << "\n" << std::endl;
  #endif

  load.close();

  return true;
}

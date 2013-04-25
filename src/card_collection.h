/******************************************************************************
    card_collection.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_COLLECTION_HEADERS
#define CARD_COLLECTION_HEADERS

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "SDL/SDL.h"

#include "cfg.h"
#include "card.h"

//#define DEBUG_CARD_COLLECTION
//#define DEBUG_CARD_COLLECTION_OBJ

class Collection
{
  public:
    Collection ( void );
    ~Collection ( void );

    bool Load ( std::string filename );

    std::vector<Card> cards;

  private:
    // ...
};

#endif // CARD_COLLECTION_HEADERS defined

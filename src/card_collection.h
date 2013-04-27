/******************************************************************************
    card_collection.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_COLLECTION_HEADERS
#define GAMEAPP_CARD_COLLECTION_HEADERS

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

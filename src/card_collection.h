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

#include "cfg.h"
#include "card.h"

class Collection
{
  public:
    Collection ( void );
    ~Collection ( void );

    bool Load ( std::string filename );

    // TODO: a) ERR handling; b) reconsider how we pass
    Card &getCards ( unsigned int idx );
    std::vector<Card> getCards ( void );

    std::vector<Card> cards; // TODO: redeclare private scope
  private:
    // ...
};

#endif // CARD_COLLECTION_HEADERS defined

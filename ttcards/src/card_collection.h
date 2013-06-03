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
#include <cassert>

#include "cfg.h"
#include "card.h"
#include "card_debug.h"

#include "json_spirit.h"
#include "json_spirit_stream_reader.h"
#include "json_spirit_writer_template.h" // FIXME

#ifndef JSON_SPIRIT_VALUE_ENABLED
  #define JSON_SPIRIT_VALUE_ENABLED
#endif

class Collection
{
  public:
    Collection ( void );
    ~Collection ( void );

    bool LoadJSON ( std::string filename );
    bool LoadASCII ( std::string filename );

    bool ExportASCII ( std::string filename ); // NOT IMPLEMENTED
    bool ExportJSON ( std::string filename );

    // TODO: a) ERR handling; b) reconsider how we pass
    Card &getCards ( unsigned int idx );
    std::vector<Card> getCards ( void );

    void clear ( void );

    std::vector<Card> cards; // TODO: redeclare private scope
  private:
    // ...
};

#endif // CARD_COLLECTION_HEADERS defined

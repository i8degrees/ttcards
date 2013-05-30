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

  this->reset();
}

Collection::~Collection ( void )
{
  #ifdef DEBUG_CARD_COLLECTION_OBJ
    std::cout << "Goodbye cruel world! <From Collection::~Collection>" << "\n" << std::endl;
  #endif

  this->reset();
}

bool Collection::Load ( std::string filename )
{
  int index = 0;
  unsigned int id, level, type, element = 0;
  std::array<int, 4> rank = { { 0 } };
  std::string name = "\0";

  std::ifstream fp;
  fp.open ( filename );

  if ( ! fp )
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
    fp >> id;
    fp >> level;
    fp >> type;
    fp >> element;
    fp >> rank[NORTH];
    fp >> rank[EAST];
    fp >> rank[SOUTH];
    fp >> rank[WEST];
    fp >> name;

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

    this->cards.push_back ( Card ( id, level, type, element, { { rank[NORTH], rank[EAST], rank[SOUTH], rank[WEST] } }, name, 0 ) );
  }

  #ifdef DEBUG_CARD_COLLECTION
    std::cout << "EOF: " << filename << " " << "<From Collection::Load>" << "\n" << std::endl;
  #endif

  fp.close();

  return true;
}

/*
bool Collection::LoadJSON ( std::string filename )
{
  //int index = 0;
  //unsigned int id, level, type, element = 0;
  //std::array<int, 4> rank = { { 0 } };
  //std::string name = "\0";

  json_spirit::Object obj;

  nom::JSON_FileReader parser;

  parser.Parse ( "cards.json" );

  return true;
}
*/

bool Collection::ExportASCII ( std::string filename )
{
  // NOT IMPLEMENTED (Stub )
  return false;
}

bool Collection::ExportJSON ( std::string filename )
{
  nom::JSON_FileWriter writer;
  nom::JSON_Object root;
  json_spirit::Array ranks; // FIXME

  if ( this->cards.empty() )
  {
    this->Load ( CARDS_DB );
  }

  for ( int i = 0; i < this->cards.size(); i++ )
  {
    ranks.clear(); // FIXME

    nom::JSON_Value id ( "ID", (int)this->cards[i].getID() );
    nom::JSON_Value name ( "Name", this->cards[i].getName() );
    nom::JSON_Value level ( "Level", (int)this->cards[i].getLevel() );
    nom::JSON_Value type ( "Type", (int)this->cards[i].getType() );
    nom::JSON_Value element ( "Element", (int)this->cards[i].getElement() );

    root.Add ( id );
    root.Add ( name );
    root.Add ( level );
    root.Add ( type );
    root.Add ( element );

    ranks.push_back ( json_spirit::Value ( (int)this->cards[i].getNorthRank() ) );
    ranks.push_back ( json_spirit::Value ( (int)this->cards[i].getEastRank() ) );
    ranks.push_back ( json_spirit::Value ( (int)this->cards[i].getSouthRank() ) );
    ranks.push_back ( json_spirit::Value ( (int)this->cards[i].getWestRank() ) );

    root.obj.push_back ( json_spirit::Pair ( "Ranks", ranks ) ); // FIXME

    root.endl();
  }

  writer.Write ( filename, root );

  return true;
}

Card &Collection::getCards ( unsigned int idx )
{
  return this->cards[idx];
}

std::vector<Card> Collection::getCards ( void )
{
  unsigned int idx = 0;
  std::vector<Card> temp_cards; // temp var for return passing
  temp_cards.clear();

  for ( idx = 0; idx < this->cards.size(); idx++ )
    temp_cards.push_back ( this->cards[idx] );

  return temp_cards;
}

void Collection::reset ( void )
{
  this->cards.clear();
}

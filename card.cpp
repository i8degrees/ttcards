/******************************************************************************
    card.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card.h"

using namespace std;

Card::Card (  unsigned int cid, unsigned int level, unsigned int ctype,
              unsigned int element, int p0, int p1, int p2, int p3, std::string name)
{
  this->id = cid;
  this->level = level;
  this->type = ctype;
  this->element = element;
  Card::power[0] = p0;
  Card::power[1] = p1;
  Card::power[2] = p2;
  Card::power[3] = p3;
  Card::name = name;
}

Card::~Card ( void )
{
}

Deck::Deck ( string filename )
{
  unsigned int id, level, type, element, power[4];
  string name;

  ifstream load ( filename );

  if ( ! load )
  {
    std::cout << "ERR: Input file " << filename << endl;
    //cerr << "ERR: Input file " << filename << endl;
    exit ( EXIT_FAILURE );
  }

  for ( int i = 0; i < MAX_DECKSET; i++ )
  {
    load >> id;
    load >> level;
    load >> type;
    load >> element;
    load >> power[0];
    load >> power[1];
    load >> power[2];
    load >> power[3];
    load >> name;

    cards.push_back ( Card (id, level, type, element, power[0], power[1], power[2], power[3], name) );
    //cout << id << " " << level << " " << type << " " << element << " " << power[0] << " " << power[1] << " " << power[2] << " " << power[3] << " " << name << endl;
  }

  load.close();
}

Deck::~Deck ( void )
{
}

void Deck::Draw ( void )
{
  for ( int i = 0; i < MAX_DECKSET; i++ )
  {

    std::cout << cards[i].id << " " << cards[i].level << " " << cards[i].type << " " << cards[i].element << " " << cards[i].power[0] << " " << cards[i].power[1] << " " << cards[i].power[2] << " " << cards[i].power[3] << " " << cards[i].name << std::endl;
  }
}

void Deck::Shuffle ( void )
{
  std::srand ( unsigned ( std::time(0) ) );

  random_shuffle ( cards.begin(), cards.end() );
  //random_shuffle ( cards.begin(), cards.end(), randomize(MAX_CARDSET) );
  //return (std::rand() % MAX_CARDSET+1);
}

/*
void Deck::Deal ( Pile *card )
{
    card[0].id = cards[0].name;
    // return cards vector array?
}
*/

Pile::Pile ( Deck *deck )
{

  for ( int i = 0; i < MAX_CARDSET; i++ )
  {
    Pile::cards.push_back ( Card (deck->cards[i].id, deck->cards[i].level, deck->cards[i].type, deck->cards[i].element, deck->cards[i].power[0], deck->cards[i].power[1], deck->cards[i].power[2], deck->cards[i].power[3], deck->cards[i].name));
    //Pile::cards[i].id = deck->cards[i].id;
    //std::cout << Pile::cards[i].name << endl;
    //std::cout << deck->cards[i].id << " " << deck->cards[i].name << endl;
  }

}

Pile::~Pile ( void )
{
}

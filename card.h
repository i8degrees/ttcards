/******************************************************************************
    card.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_HEADERS
#define CARD_HEADERS

#include <iostream>
#include <fstream> // file streams
#include <vector>
#include <string>

#include "SDL/SDL.h"

#include "cfg.h"

/*
const typedef struct card_typ {
  unsigned int id;
  unsigned int level;
  unsigned int type;
  unsigned int element;
  std::array<int, 4> power;
  std::string name;
} card_type;
*/

int randomize ( int rval );

class Card
{
public:
  Card ( int id, int level, int ctype, int element, int p1, int p2, int p3, int p4, std::string name);
  ~Card ( void );

  void Draw ( void );
  unsigned int id;
  unsigned int level; // 1..10
  unsigned int type;
  unsigned int element;
  unsigned int power[4]; // clockwise; top, right, down, left
  std::string name;
private:

};

class Deck
{
public:
  Deck( std::string filename );
  ~Deck ( void );

  //bool Load ( string filename );
  void Shuffle ( void );
  void Draw ( void );
  //void Deal ( Pile *card );
  std::vector<Card> cards;
private:
  //std::vector<Card> hand;
};

class Pile
{
public:
  Pile ( Deck *deck );
  ~Pile ( void );

  void Add ( void );
  void List ( void );
  std::vector<Card> cards;
};

typedef enum element_type {
  NONE=0, // type 0; reserved / debug
  EARTH=1, // type 1 element
  FIRE=2, // type 2 element
  HOLY=3, // type 3 element
  ICE=4, // type 4 element
  POISON=5, // type 5 element
  THUNDER=6, // type 6 element
  WATER=7, // type 7 element
  WIND=8 // type 8 element
  // TODO: cElement etype = { ICE }; std::cout << etype << endl;
} cElement, cEleType;

typedef enum category_type {
  MONSTER=1,
  BOSS=2,
  GF=3,
  PLAYER=4,
  LEVEL_1=1,
  LEVEL_2=2,
  LEVEL_3=3,
  LEVEL_4=4,
  LEVEL_5=5,
  LEVEL_6=6,
  LEVEL_7=7,
  LEVEL_8=8,
  LEVEL_9=9,
  LEVEL_10=10
  // TODO: cType cType = { LEVEL_4 }; std::cout << cType << "\n";
} cType, cat_type;

#endif // CARD_HEADERS defined

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

class Card
{
  public:
    Card (  unsigned int cid, unsigned int level, unsigned int ctype,
            unsigned int element, int p0, int p1, int p2, int p3,
            std::string name);

    ~Card ( void );

    unsigned int id;
    unsigned int level;
    unsigned int type;
    unsigned int element;
    unsigned int power[4];
    std::string name;

    // Player's Card Elemental type
    enum CardElement {
      NONE=0, // type 0; reserved / debug
      EARTH=1, // type 1 element
      FIRE=2, // type 2 element
      HOLY=3, // type 3 element
      ICE=4, // type 4 element
      POISON=5, // type 5 element
      THUNDER=6, // type 6 element
      WATER=7, // type 7 element
      WIND=8 // type 8 element
    };

    // Player's  Level Category Bracket
    enum CardLType {
      MONSTER=1,
      BOSS=2,
      GF=3,
      PLAYER=4
    };

    // Card ( id, level, type, element, power[], name );
    // getID();
    // getLevel();
    // getType();
    // getElement();
    // getRank();
    // getName();
    // isFaceUp();
    // Flip();
    // Color();
    std::vector<Card>cards;
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


#endif // CARD_HEADERS defined

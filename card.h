/******************************************************************************
    card.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/

#include <SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#define CARD_WIDTH 64
#define CARD_HEIGHT 64
#define MAX_DECKSET 30
#define MAX_CARDSET 4

int randomize ( int rval );

class Card
{
public:
  Card ( int id, int level, int ctype, int element, int p1, int p2, int p3, int p4, string name);
  void Show ( void );
  unsigned int id;
  unsigned int level; // 1..10
  unsigned int type;
  unsigned int element;
  unsigned int power[4]; // clockwise; top, right, down, left
  string name;
private:
};

class Deck
{
public:
  Deck();
  bool Load ( string filename );
  void Shuffle ( void );
  void Draw ( void );
  std::vector<Card> cards;
private:
};

/*
  enum {
    MONSTER, // type 1; lvl 1..5
    BOSS, // type 2; lvl 6..7
    GF, // type 3; lvl 8..9
    PLAYER // type 4; lvl 10
  } class;
*/

/*
  enum {
    EARTH, // type 1
    FIRE, // type 2
    HOLY, // type 3
    POISON, // type 4
    THUNDER, // type 5
    WATER, // type 6
    WIND // type 7
  } elemental;
*/

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

int init_card (void);
int load_card (void);

struct Card {

  //int magic;
  //int ver;

  unsigned int id;
  unsigned int level; // 1..10
  unsigned int type;
  unsigned int element;
  unsigned int power[4]; // clockwise; top, right, down, left
  char name[255];
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

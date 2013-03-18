#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CARD_WIDTH 64
#define CARD_HEIGHT 64
#define MAX_DECKSET 30
#define MAX_CARDSET 5

int randomize(void);
int init_card(struct Card *card_ptr, struct Deck *deck_ptr);
//int init_cards(struct Deck *deck_ptr, struct Card *card_ptr);
int load_deck(struct Deck *deck_ptr, char deck_datafile[255]);
//int load_card(struct Card *card_ptr, char cards_datafile[255]);

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

struct Deck {
    struct Card cards[MAX_DECKSET];
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

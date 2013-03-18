#include "card.h"

int randomize(void)
{
  int rvalue = NULL;

  rvalue = (rand() % MAX_DECKSET);

  return rvalue;
}

int init_card ( struct Card *card_ptr, struct Deck *deck_ptr )
//int init_card(struct Deck *deck_ptr, struct Card *card_ptr)
{
  int rval = NULL;

  for ( int i = 0; i < MAX_CARDSET; i++ )
  {
    rval = randomize();
    card_ptr[i].id = deck_ptr->cards[rval].id;
    card_ptr[i].level = deck_ptr->cards[rval].level;
    card_ptr[i].type = deck_ptr->cards[rval].type;
    card_ptr[i].element = deck_ptr->cards[rval].element;
    card_ptr[i].power[0] = deck_ptr->cards[rval].power[0];
    card_ptr[i].power[1] = deck_ptr->cards[rval].power[1];
    card_ptr[i].power[2] = deck_ptr->cards[rval].power[2];
    card_ptr[i].power[3] = deck_ptr->cards[rval].power[3];
    //card_ptr[i].name = deck_ptr->cards[rval].name;
    //card_ptr[i].name = deck_ptr->cards[rval].name;
    //printf("%d %s\n", deck_ptr->cards[rval].id, deck_ptr->cards[rval].name);
  }


/*
  printf("%d %d %d %d %d %d %d %d %s\n", card_ptr->id, card_ptr->level, card_ptr->type,
          card_ptr->element, card_ptr->power[0], card_ptr->power[1], card_ptr->power[2],
          card_ptr->power[3], card_ptr->name);
*/
  return rval;
}

int load_deck ( struct Deck *deck_ptr, char deck_datafile[255] )
{
  FILE *fp = NULL;

  fp = fopen ( deck_datafile, "r" );

  if ( fp == NULL)
  {
    printf("Error opening file: %s\n", deck_datafile);
    return -1;
  }

  for ( int y = 0; y < MAX_DECKSET; y++ )
  //for ( int y = 0; ! feof(fp); y++ )
  {
    // 0 1 1 0 1 4 5 1 Geezard
    fscanf ( fp, "%i %i %i %i %i %i %i %i %[A-Za-z0-9- ]",
                  &deck_ptr->cards[y].id, &deck_ptr->cards[y].level, &deck_ptr->cards[y].type, &deck_ptr->cards[y].element,
                  &deck_ptr->cards[y].power[0], &deck_ptr->cards[y].power[1], &deck_ptr->cards[y].power[2], &deck_ptr->cards[y].power[3],
                  deck_ptr->cards[y].name);

    //fscanf ( fp, "\n");
/*
    printf("%i %i %i %i %i %i %i %i %s\n",
            card.id, card.level, card.type, card.element,
            card.power[0], card.power[1], card.power[2], card.power[3],
            card.name);
*/

  }

  fclose(fp);

  return 0;
}

/*
void show_deck(void)
{

  return 0;
}
*/

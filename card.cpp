#include "card.h"

int init_card(void)
{
  char data_file[255] = "cards.txt";
  FILE *fp = NULL;
  struct Card card;

  fp = fopen ( data_file, "r" );

  if ( fp == NULL)
  {
    printf("Error opening file: %s\n", data_file);
    return -1;
  }

  for ( int y = 0; ! feof(fp); y++ )
  {
    // 0 1 1 0 1 4 5 1 Geezard
    fscanf ( fp, "%i %i %i %i %i %i %i %i %[A-Za-z0-9- ]",
                  &card.id, &card.level, &card.type, &card.element,
                  &card.power[0], &card.power[1], &card.power[2], &card.power[3],
                  card.name);
    fscanf ( fp, "\n");

    printf("%i %i %i %i %i %i %i %i %s\n",
            card.id, card.level, card.type, card.element,
            card.power[0], card.power[1], card.power[2], card.power[3],
            card.name);
  }

  fclose(fp);

  return 0;
}

int load_card(void)
{
    return 0;
}

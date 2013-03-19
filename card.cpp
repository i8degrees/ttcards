#include "card.h"

int randomize ( int rval )
{
    return 0;
    //return (std::rand() % MAX_CARDSET+1);
}

Card::Card ( int cid, int level, int ctype, int element, int p0, int p1, int p2, int p3, string name)
{
  Card::id = cid;
  Card::level = level;
  Card::type = ctype;
  Card::element = element;
  Card::power[0] = p0;
  Card::power[1] = p1;
  Card::power[2] = p2;
  Card::power[3] = p3;
  Card::name = name;
}

void Card::Show ( void )
{
  cout << Card::id << " " << Card::name << "\n";
}

Deck::Deck ( void )
{
}

bool Deck::Load ( string filename )
{
    unsigned int id, level, type, element, power[4];
    string name;

    ifstream in ( filename );

    if ( ! in )
    {
        cerr << "ERR: Input file " << filename << endl;
        return false;
    }


    for ( int i = 0; i < MAX_DECKSET; i++ )
    {
        in >> id;
        in >> level;
        in >> type;
        in >> element;
        in >> power[0];
        in >> power[1];
        in >> power[2];
        in >> power[3];
        in >> name;

        cards.push_back ( Card (id, level, type, element, power[0], power[1], power[2], power[3], name) );
        //cout << id << " " << level << " " << type << " " << element << " " << power[0] << " " << power[1] << " " << power[2] << " " << power[3] << " " << name << endl;
    }

    in.close();

    return true;
}

void Deck::Draw ( void )
{
  for ( int i = 0; i < MAX_DECKSET; i++ )
  {
    std::cout << cards[i].id << " " << cards[i].name << endl;
  }
}

void Deck::Shuffle ( void )
{
    //random_shuffle ( cards.begin(), cards.end(), randomize );
}

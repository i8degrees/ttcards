/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef GAMEAPP_CARD_HEADERS
#define GAMEAPP_CARD_HEADERS

#include <string>
#include <array>
#include <algorithm>

#include <nomlib/config.hpp>

#include "config.hpp"

namespace tt {

// Forward declarations
class CardRenderer;

/// Maximum element a card can contain -- starting at 1
const nom::int32 MAX_ELEMENT = 8;

/// Minimum rank a card can contain
const nom::int32 MIN_RANK = 1;

/// Maximum rank a card can contain
const nom::int32 MAX_RANK = 10;

/// Maximum number of rank attributes
const nom::int32 MAX_RANKS = 4;

/// Maximum name length a card can contain -- "Chubby Chocobo" (without quotes)
const nom::int32 MAX_NAME = 14; // +1 padding

/// \brief The minimum number of cards of a type you can collect.
const int MIN_NUM = 0;

/// \brief The maximum number of cards of a type you can collect.
const int MAX_NUM = 99;

class Card
{
  public:
    static Card null;

    Card();
    ~Card();

    const nom::int32 getID ( void ) const;
    const std::string get_id_string( void ) const;
    const nom::int32 getLevel ( void ) const;
    const nom::int32 getType ( void ) const;
    const nom::int32 getElement ( void ) const;
    const std::array<nom::int32, MAX_RANKS> getRanks ( void ) const;
    const std::vector<int> ranks_as_vector ( void ) const;
    const nom::int32 getNorthRank ( void ) const;
    const nom::int32 getEastRank ( void ) const;
    const nom::int32 getSouthRank ( void ) const;
    const nom::int32 getWestRank ( void ) const;
    const std::string& getName ( void ) const;

    const nom::int32 getPlayerID ( void ) const;
    const nom::int32 getPlayerOwner ( void ) const;

    int num() const;
    bool face_down() const;

    /// The total strength value of any given card is determined by adding the
    /// sum of all the card rank values together.
    ///
    /// Note that this does not take into account game rules that may be in
    /// effect!
    nom::int32 strength ( void );

    std::shared_ptr<CardRenderer>& card_renderer();

    void setID ( nom::int32 id_ );

    void setLevel ( nom::int32 level_ );

    void setType ( nom::int32 type_ );

    void setElement ( nom::int32 element_ );

    void setRanks ( std::array<nom::int32, MAX_RANKS> ranks );

    void set_ranks ( std::vector<nom::int32> ranks );

    void setNorthRank ( nom::int32 rank );

    void setEastRank ( nom::int32 rank );

    void setSouthRank ( nom::int32 rank );

    void setWestRank ( nom::int32 rank );

    void setName ( std::string name_ );

    void setPlayerID ( nom::int32 player_id_ );

    void setPlayerOwner ( nom::int32 player_owner_ );

    /// \brief Set the number of cards of this type has been collected.
    void set_num(int num_cards);

    void set_face_down(bool state);

    void increaseNorthRank ( void );
    void increaseEastRank ( void );
    void increaseSouthRank ( void );
    void increaseWestRank ( void );

    void decreaseNorthRank ( void );
    void decreaseEastRank ( void );
    void decreaseSouthRank ( void );
    void decreaseWestRank ( void );

    void set_card_renderer(CardRenderer* renderer);

    /// card.player_id AKA owner tag
    enum
    {
      NO_PLAYER=0,
      PLAYER1=1,
      PLAYER2=2
    };

    nom::Value serialize( void ) const;

    void unserialize( nom::Value& obj );

  private:
    nom::int32 id;
    nom::int32 level;
    nom::int32 type;
    /// NONE is no element
    nom::int32 element;
    /// NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3
    std::array<nom::int32, MAX_RANKS> rank;
    std::string name;

    /// Additional field; used to distinguish card background and also used to
    // track player in board in order to do card flipping, among other things
    // like score tallying
    nom::int32 player_id;

    /// Additional field; original owner of the card
    nom::int32 player_owner;

    /// \brief The number of cards of this instance type that has been
    /// collected.
    int num_;

    /// \brief Internal state flag for whether or not the card face is to be
    /// shown to the player.
    bool face_down_;

    std::shared_ptr<CardRenderer> card_renderer_;
};

/// \todo Rename to CardList..?
typedef std::vector<Card> Cards;

/// TODO: Rename to Card::iterator
typedef Cards::iterator CardsIterator;

/// TODO: Rename to Card::const_iterator
typedef Cards::const_iterator ConstCardsIterator;

/// Pretty print the the card attributes.
std::ostream& operator <<(std::ostream& os, const Card& rhs);

/// Compare two cards for equality
bool operator == ( const Card& lhs, const Card& rhs );

/// Compare two cards for in-equality
bool operator != ( const Card& lhs, const Card& rhs );

/// Compare two cards for less-than equality
bool operator < ( const Card& lhs, const Card& rhs );

/// Compare two cards for greater-than equality
bool operator > ( const Card& lhs, const Card& rhs );

/// Compare two cards for less-than or equal to equality
bool operator <= ( const Card& lhs, const Card& rhs );

/// Compare two cards for greater-than or equal to equality
bool operator >= ( const Card& lhs, const Card& rhs );

/// \brief Card strength greater-than comparison.
///
/// \see CardHand
bool strongest_card(const Card& lhs, const Card& rhs);

/// \brief Card strength lesser-than comparison.
///
/// \see CardHand
bool weakest_card(const Card& lhs, const Card& rhs);

} // namespace tt

#endif // GAMEAPP_CARD_HEADERS defined

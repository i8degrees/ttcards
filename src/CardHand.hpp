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
#ifndef GAMEAPP_CARD_HAND_HEADERS
#define GAMEAPP_CARD_HAND_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <functional>
#include <memory>

#include <nomlib/config.hpp>
#include <nomlib/serializers.hpp>

#include "Card.hpp"
#include "CardCollection.hpp"
#include "config.hpp"

class CardHand
{
  public:
    typedef std::shared_ptr<CardHand> SharedPtr;

    CardHand ( void );
    ~CardHand ( void );

    bool push_back ( const Card& card );
    bool erase ( Card& card );

    void clearSelectedCard ( void );
    Card& getSelectedCard ( void );

    /// \deprecated Use the provided iterators ::previous, ::next,
    /// ::set_position, etc.
    void selectCard ( Card& card );

    bool exists ( const Card& card ) const;

    /// Make selected card the first element in stack
    ///
    /// The current card position is reset to zero.
    void front();

    /// Obtain the current position we are at in the cards vector
    ///
    /// Returns a number between zero and the maximum card element in the
    /// vector.
    nom::size_type position();

    /// Increment the position we are at in the cards vector by one.
    ///
    /// If we are at the end of the vector of cards, we reset the position to
    /// zero.
    ///
    /// We always update the selected card to this calculated position.
    void next();

    /// Decrement the position we are at in the cards vector by one.
    ///
    /// If we are at the beginning or the end of the vector of cards, we reset
    /// the position to zero.
    ///
    /// We always update the selected card to this calculated position.
    void previous();

    /// Sets the position we are at in the cards vector
    void set_position(nom::size_type pos);

    void clear ( void );
    bool empty() const;
    nom::uint32 size ( void ) const;
    nom::int32 at ( Card& card );

    /// Creates a randomized player hand with the preferred minimum & maximum
    /// level ranges in mind, ~~with no duplicate cards present~~.
    ///
    /// Do not forget to set the proper player ID on your new card objects
    /// before heading off into battle!
    void shuffle( nom::int32 level_min, nom::int32 level_max, const CardCollection& db);

    /// Save the current player's hand to a file as a series of RFC 4627 JSON
    /// compliant objects.
    bool save ( const std::string& filename );

    /// Load saved player hand from a file encoded as RFC 4627 compliant JSON
    /// objects.
    bool load ( const std::string& filename );

    /// Modify card rank values.
    ///
    /// \fixme This method breaks when we use ::set_position && friends
    void modifyCardRank ( bool modifier, nom::uint32 direction );

    /// Getter for obtaining the strongest card in the player's hand.
    ///
    /// Note that this does not take into account game rules that may be in
    /// effect!
    const Card strongest ( void );

    /// Getter for obtaining the weakest card in the player's hand.
    ///
    /// Note that this does not take into account game rules that may be in
    /// effect!
    const Card weakest ( void );

    /// \brief Toggle the rendering state of the card face for all the cards in
    /// the player's hand.
    void set_face_down(bool state);

    /// \todo Declare in private scope
    Cards cards;

  private:
    /// Track the current position we are at in the cards vector
    nom::size_type position_;

    /// Player's active card
    Card selectedCard;
};

/// Pretty print the the card attributes.
///
std::ostream& operator << ( std::ostream& os, const CardHand& rhs );

#endif // GAMEAPP_CARD_HAND_HEADERS defined

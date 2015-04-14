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
#ifndef TTCARDS_CARD_COLLECTION_HPP
#define TTCARDS_CARD_COLLECTION_HPP

#include <string>

#include "config.hpp"
#include "Card.hpp"

namespace tt {

class CardCollection
{
  public:
    CardCollection();
    ~CardCollection();

    void clear();
    nom::size_type size() const;

    /// \returns The first card in the container on success, or Card::null on
    /// failure, such as when the container is empty.
    const Card& front() const;

    /// Save the current board grid data to a file as a series of RFC 4627
    /// compliant JSON objects.
    bool save(const std::string& filename);

    /// Load saved board grid data from a file encoded as RFC 4627 compliant
    /// JSON objects.
    bool load(const std::string& filename);

    /// \returns The resulting card by its name on success, or Card::null on
    /// failure to find a card by the given name.
    const Card& find(const std::string& card_name) const;

    /// \returns The resulting card by its ID on success, or Card::null on
    /// failure to find a card by the given ID.
    const Card& find(nom::int32 card_id) const;

    ConstCardsIterator begin() const;
    ConstCardsIterator end() const;

    CardsIterator begin();
    CardsIterator end();

  private:
    // TODO: Rename to cards_
    Cards cards;
};

} // namespace tt

#endif // GAMEAPP_CARD_COLLECTION_HEADERS defined

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
#ifndef TTCARDS_CARD_DEALER_HPP
#define TTCARDS_CARD_DEALER_HPP

#include <memory>
#include <string>
#include <map>

#include "config.hpp"
#include "Card.hpp"

// Forward declarations (engine)
namespace nom {

class Font;

}

namespace tt {

// Forward declarations
class CardResourceLoader;
class CardCollection;
class CardHand;
// class IPlayer;
struct Player;

class CardDealer
{
  public:
    CardDealer();
    ~CardDealer();

    // CardHand* begin();
    // CardHand* end();
    CardHand* player_hand(PlayerIndex pid) const;
    CardCollection* player_deck(PlayerIndex pid) const;

    nom::size_type num_players() const;
    nom::size_type num_cards(PlayerIndex pid) const;

    std::string player_name(PlayerIndex pid) const;
    nom::uint32 player_score(PlayerIndex pid) const;

    void clear_hand(PlayerIndex pid);
    void clear_hands();

    /// \brief Initialize the player's hand.
    ///
    /// \param pid One of the enumeration values of tt::PlayerIndex
    ///
    /// \remarks The player's index controls the ownership and rendering color
    /// of their card's texture.
    nom::size_type
    register_player(PlayerIndex pid, CardResourceLoader* card_res,
                    const nom::Font& scoreboard_font);

    void deregister_player(PlayerIndex pid);

    void set_player_name(PlayerIndex pid, const std::string& name);

    bool add_player_card(PlayerIndex pid, const Card& card);
    bool add_player_card(PlayerIndex pid, const Cards& cards);
    bool remove_player_card(PlayerIndex pid, const Card& card);

    /// \brief Create a randomized player hand with a set criteria.
    // TODO: Rename to shuffle_random_card ..?
    void
    add_random_card(  PlayerIndex pid, nom::uint32 min_level,
                      nom::uint32 max_level, bool face_up = true );

    void add_deck_card(PlayerIndex pid, const Card& card);
    void add_deck_card(PlayerIndex pid, const Cards& cards);

    void remove_deck_card(PlayerIndex pid, const Card& card);

    bool save_deck(PlayerIndex pid, const std::string& filename);
    bool load_new_deck(PlayerIndex pid, const std::string& filename);
    bool load_deck(PlayerIndex pid, const std::string& filename,
                   bool load_new_deck = false);

    // nom::Value
    // save_player_hand(PlayerIndex pid, const std::string& filename);
    bool
    load_player_hand(PlayerIndex pid, const std::string& filename,
                     nom::Value& game);

    /// \brief Re-render the player's hand.
    ///
    /// \note This method call is necessary when render-able card attributes
    /// are modified outside of this class interface.
    bool generate_card_texture(PlayerIndex pid, Card& card);

    // TODO: Add ::set_card_position(PlayerIndex pid, Point2i& pos)..?

    bool set_player_card_faces(PlayerIndex pid, bool face_up);
    void set_player_score(PlayerIndex pid, nom::uint32 score);

    /// \brief Set the rendering coordinates of the player's cards in their
    /// hand.
    ///
    /// \returns The number of successfully modified cards.
    ///
    /// \param origin The card rendering position to base the spacing
    /// calculation of the player's hand from.
    nom::uint32
    update_player_hand(PlayerIndex pid, const nom::Point2i& origin);

    /// \brief Render the player's hand.
    void
    render_player_hand(PlayerIndex pid, const nom::RenderWindow& target);

    void
    render_player_score(PlayerIndex pid,
                        const nom::RenderWindow& target);

    void dump_deck() const;
    void dump_deck(PlayerIndex pid) const;

    void dump_hand() const;
    void dump_hand(PlayerIndex pid) const;

    // TODO: Consider adding ::player_hand_position..? (i.e.: game cursor)
    // TODO: Consider adding ::player_deck_position..? (i.e.: game cursor)

  private:
    std::map<PlayerIndex, std::unique_ptr<Player>> players_;

    /// \brief Our reference to the cards resource loader.
    ///
    /// \remarks We do not own this pointer, so we **must not** free it!
    CardResourceLoader* card_res_ = nullptr;

};

} // namespace tt

#endif // GAMEAPP_CARD_COLLECTION_HEADERS defined

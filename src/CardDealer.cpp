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
#include "CardDealer.hpp"

// Private headers
#include "helpers.hpp"
#include <nomlib/core/helpers.hpp>
#include <nomlib/serializers.hpp>
#include <nomlib/graphics.hpp>

// Forward declarations
#include "CardResourceLoader.hpp"
#include "CardCollection.hpp"
#include "CardHand.hpp"
#include "CardRenderer.hpp"

using namespace nom;

namespace tt {

struct Player
{
  std::unique_ptr<CardCollection> deck;
  std::unique_ptr<CardHand> hand;

  nom::Text scoreboard_text;

  /// \brief The player's unique identifier.
  PlayerID id = PLAYER_ID_INVALID;

  /// \brief The name of the player.
  std::string name;

  /// \brief The player's score.
  nom::uint32 score = 5;
};

CardDealer::CardDealer()
{
  //
}

CardDealer::~CardDealer()
{
  //
}

CardHand* CardDealer::player_hand(PlayerIndex pid) const
{
  CardHand* result = nullptr;

  auto res = this->players_.find(pid);
  if( res != this->players_.end() ) {
    // Success; player found
    NOM_ASSERT(res->second != nullptr);
    if( res->second != nullptr ) {
      result = res->second->hand.get();
    }
  }

  return result;
}

CardCollection* CardDealer::player_deck(PlayerIndex pid) const
{
  CardCollection* result = nullptr;

  auto res = this->players_.find(pid);
  if( res != this->players_.end() ) {
    // Success; player found
    NOM_ASSERT(res->second != nullptr);
    if( res->second != nullptr ) {
      result = res->second->deck.get();
    }
  }

  return result;
}

nom::size_type CardDealer::num_players() const
{
  return this->players_.size();
}

nom::size_type CardDealer::num_cards(PlayerIndex pid) const
{
  nom::size_type result = 0;
  auto phand = this->player_hand(pid);

  NOM_ASSERT(phand != nullptr);
  if( phand != nullptr ) {
    result = phand->size();
  }

  return result;
}

std::string CardDealer::player_name(PlayerIndex pid) const
{
  std::string result = "\0";

  auto res = this->players_.find(pid);
  if( res != this->players_.end() ) {

    NOM_ASSERT(res->second != nullptr);
    if( res->second != nullptr ) {
      result = res->second->name;
    }
  }

  return result;
}

nom::uint32 CardDealer::player_score(PlayerIndex pid) const
{
  nom::uint32 result = 5; // Default scoreboard for a player

  auto res = this->players_.find(pid);
  if( res != this->players_.end() ) {

    NOM_ASSERT(res->second != nullptr);
    if( res->second != nullptr ) {
      result = res->second->score;
    }
  }

  return result;
}

void CardDealer::clear_hand(PlayerIndex pid)
{
  auto phand = this->player_hand(pid);

  NOM_ASSERT(phand != nullptr);
  if( phand != nullptr ) {
    phand->clear();
  }
}

void CardDealer::clear_hands()
{
  auto num_players = this->players_.size();

  // Found; exec clear action
  for( auto pindex = 0; pindex != num_players; ++pindex ) {
    auto player_index = NOM_SCAST(PlayerIndex, pindex);
    this->clear_hand(player_index);
  }
}

nom::size_type
CardDealer::register_player(PlayerIndex pid, CardResourceLoader* card_res,
                            const nom::Font& scoreboard_font)
{
  std::unique_ptr<Player> player;
  nom::size_type player_pos = this->players_.size();  // Insertion index

  if( card_res != nullptr ) {
    this->card_res_ = card_res;
  }

  NOM_ASSERT(this->card_res_ != nullptr);

  player = nom::make_unique<Player>();
  if( player != nullptr ) {
    player->deck = nom::make_unique<CardCollection>();
    player->hand = nom::make_unique<CardHand>();
    player->id = tt::player_id(pid);

    if( scoreboard_font->valid() ) {
      player->scoreboard_text.set_font(scoreboard_font);
    }

    auto p = std::make_pair(pid, std::move(player) );
    this->players_.emplace( std::move(p) );
  }

  return player_pos;
}

void CardDealer::deregister_player(PlayerIndex pid)
{
  auto res = this->players_.find(pid);
  if( res == this->players_.end() ) {
    // Not found
  } else {
    // Found; clear the player's hand
    this->clear_hand(pid);
    // this->players_.erase(res);
  }
}

void CardDealer::set_player_name(PlayerIndex pid, const std::string& name)
{
  auto res = this->players_.find(pid);
  if( res != this->players_.end() ) {

    NOM_ASSERT(res->second != nullptr);
    if( res->second != nullptr ) {
      res->second->name = name;
    }
  }
}

bool CardDealer::add_player_card(PlayerIndex pid, const Card& card)
{
  bool result = false;
  auto phand = this->player_hand(pid);

  NOM_ASSERT(phand != nullptr);
  if( phand != nullptr ) {
    result = phand->push_back(card);
  }

  return result;
}

bool CardDealer::add_player_card(PlayerIndex pid, const Cards& cards)
{
  auto phand = this->player_hand(pid);

  NOM_ASSERT(phand != nullptr);
  if( phand != nullptr ) {

    for( auto itr = cards.begin(); itr != cards.end(); ++itr ) {
      phand->push_back(*itr);
    }

    return true;
  }

  return false;
}

bool CardDealer::remove_player_card(PlayerIndex pid, const Card& card)
{
  auto phand = this->player_hand(pid);

  NOM_ASSERT(phand != nullptr);
  if( phand != nullptr ) {

    if( card != Card::null ) {
      phand->erase(card);
    }

    return true;
  }

  return false;
}

void
CardDealer::add_random_card(  PlayerIndex pid, nom::uint32 min_level,
                              nom::uint32 max_level, bool face_up )
{
  nom::size_type num_cards = 0;
  int32 random_card_id = 0;
  Card card(Card::null);

  auto deck = this->player_deck(pid);

  NOM_ASSERT(deck != nullptr);
  if( deck != nullptr ) {

    num_cards = (deck->size() - 1);

    if( num_cards < 1 ) {
      return;
    }

    random_card_id = nom::uniform_int_rand<nom::uint32>(0, num_cards);

    card = deck->find(random_card_id);
    if( card != Card::null && card.num > 0 && card.level <= max_level &&
        card.level >= min_level )
    {
      card.face_down = !(face_up);
      this->add_player_card(pid, card);
    }
  }
}

void CardDealer::add_deck_card(PlayerIndex pid, const Card& card)
{
  auto deck = this->player_deck(pid);

  NOM_ASSERT(deck != nullptr);
  if( deck != nullptr ) {
    if( card != Card::null ) {
      deck->append_card(card);
    }
  }
}

void CardDealer::add_deck_card(PlayerIndex pid, const Cards& cards)
{
  auto deck = this->player_deck(pid);

  NOM_ASSERT(deck != nullptr);
  if( deck != nullptr ) {
    deck->append_cards(cards);
  }
}

void CardDealer::remove_deck_card(PlayerIndex pid, const Card& card)
{
  auto deck = this->player_deck(pid);

  NOM_ASSERT(deck != nullptr);
  if( deck != nullptr && card != Card::null ) {
    deck->erase_card(card);
  } // end if deck != NULL
}

bool CardDealer::save_deck(PlayerIndex pid, const std::string& filename)
{
  auto deck = this->player_deck(pid);

  NOM_ASSERT(deck != nullptr);
  if( deck != nullptr ) {
    nom::Value game;

    auto fp = nom::make_unique_json_serializer();
    if( fp == nullptr ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not open input file:", "failure to allocate memory!" );
      return false;
    }

    // ...Arrays enclosed in an object...
    game["cards"] = tt::serialize_deck(deck);

    if( fp->save(game, filename) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Unable to save game at:", filename );
      return false;
    }

    // Success!
    NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Game saved at:", filename );

    return true;
  }

  return false;
}

bool
CardDealer::load_deck(PlayerIndex pid, const std::string& filename,
                      bool load_new_deck)
{
  auto deck = this->player_deck(pid);

  NOM_ASSERT(deck != nullptr);
  if( deck != nullptr ) {

    Cards cards;
    nom::Value game;

    auto fp = nom::make_unique_json_deserializer();
    if( fp == nullptr ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not open input file: failure to allocate memory!" );
      return false;
    }

    if( fp->load(filename, game) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Unable to load game from:", filename );
      return false;
    }

#if 0
    if( game["stats"]["name"].null_type() == false ) {
      this->set_player_name(PlayerIndex::PLAYER_1,
                                     game["stats"]["name"].get_string() );
      NOM_DUMP( this->player_name(PlayerIndex::PLAYER_1) );
    }
#endif

    NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Game loaded from:", filename );

    // ...Arrays enclosed in an object...
    if( load_new_deck == true ) {
      cards = tt::deserialize_deck(game["basic"]);
    } else {
      cards = tt::deserialize_deck(game["cards"]);
    }

    deck->clear();
    deck->append_cards(cards);

    // Success!
    return true;
  }

  return false;
}

#if 0
nom::Value
CardDealer::save_player_hand(PlayerIndex pid, const std::string& filename)
{
  nom::Value game;
  auto phand = this->player_hand(pid);

  auto fp = nom::make_unique_json_serializer();
  if( fp == nullptr ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not open input file: failure to allocate memory!" );
    return game;
  }

  if( phand != nullptr ) {
    // ...Arrays enclosed in an object...
    game = tt::serialize_hand(phand);
  }

  // Success!
  return game;
}
#endif

bool
CardDealer::load_player_hand(PlayerIndex pid, const std::string& filename,
                             nom::Value& game)
{
  Cards phand_cards;
  // nom::Value game;
  auto phand = this->player_hand(pid);

  auto fp = nom::make_unique_json_deserializer();
  if( fp == nullptr ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not open input file: failure to allocate memory!" );
    return false;
  }

  if( fp->load(filename, game) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Unable to load game from:", filename );
    return false;
  }

  NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Game loaded from:", filename );

  if( phand != nullptr ) {
    if( pid == PlayerIndex::PLAYER_1 ) {
      // ...Arrays enclosed in an object...
      phand_cards =
        tt::deserialize_hand(game["player"]["hand"]);
    } else if( pid == PlayerIndex::PLAYER_2 ) {
      // ...Arrays enclosed in an object...
      phand_cards =
        tt::deserialize_hand(game["opponent"]["hand"]);
    }

    phand->clear();
    this->add_player_card(pid, phand_cards);

    // Success!
    return true;
  }

  return false;
}

// FIXME: Graphics related

// TODO: Restructure into ::add_player_hand && check in ::update call for when
// to recreate card texture
bool CardDealer::generate_card_texture(PlayerIndex pid, Card& card)
{
  NOM_ASSERT(this->card_res_ != nullptr);
  if( this->card_res_ == nullptr) {
    // TODO: logging && err handling
    return false;
  }

  // IMPORTANT: To minimize rendering updates, the card's initial
  // attributes should be set before the rendering of the hand occurs
  tt::set_card_id(tt::player_id(pid), card);

  if( card.face_down == true ) {
    // No card face
    auto renderer = tt::create_card_renderer(this->card_res_, Card::null);
    card.card_renderer.reset(renderer);
  } else {
    auto renderer = tt::create_card_renderer(this->card_res_, card);
    card.card_renderer.reset(renderer);
  }

  NOM_ASSERT(card.card_renderer != nullptr);
  NOM_ASSERT(card.card_renderer->valid() == true);
  if( card.card_renderer == nullptr ) {
    // TODO: logging && err handling
    return false;
  }

  if( card.card_renderer->valid() == false ) {
    // TODO: logging && err handling
    return false;
  }

  return true;
}

bool CardDealer::set_player_card_faces(PlayerIndex pid, bool face_up)
{
  bool result = false;
  auto phand = this->player_hand(pid);

  NOM_ASSERT(phand != nullptr);
  if( phand != nullptr ) {

    for( auto itr = phand->begin(); itr != phand->end(); ++itr ) {
      itr->face_down = (!face_up);
    }

    result = true;
  }

  return result;
}

// TODO: Relocate scoreboard functionality to its own file..?
void CardDealer::set_player_score(PlayerIndex pid, nom::uint32 score)
{
  Point2i score_origin_pos(Point2i::zero);

  auto res = this->players_.find(pid);
  if( res != this->players_.end() ) {
    NOM_ASSERT(res->second != nullptr);
    res->second->score = score;

    // TODO: Use scoreboard var instead of score for storage of this..?
    auto score_str = std::to_string( this->player_score(pid) );
    res->second->scoreboard_text.set_text(score_str);

    if( pid == PlayerIndex::PLAYER_2 ) {
      score_origin_pos = PLAYER2_SCORE_ORIGIN;
    } else if( pid == PlayerIndex::PLAYER_1 ) {
      score_origin_pos = PLAYER1_SCORE_ORIGIN;
    }

    res->second->scoreboard_text.set_position(score_origin_pos);
  }
}

// FIXME: Graphics related
uint32 CardDealer::update_player_hand(PlayerIndex pid,
                                      const nom::Point2i& origin)
{
  uint32 num_updates = 0;
  uint32 hand_index = 0;

  auto phand = this->player_hand(pid);

  NOM_ASSERT(phand != nullptr);
  if( phand != nullptr ) {

    // Success; player hand is valid
    for( auto itr = phand->begin(); itr != phand->end(); ++itr ) {

      // FIXME (?): Create a new card rendering only when necessary, i.e.: when
      // the card renderer is NULL -- otherwise, we ought to be able to leave
      // it alone 99% of the time.
      // if( itr->card_renderer == nullptr ) {
        if( this->generate_card_texture(pid, *itr) == false ) {
          NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                        "Failed to update the player's hand:",
                        "could not create card renderer." );
          return false;
        }
      // }
    } // end for loop
  }

  // NOTE: Update player's card rendering positions in hand
  Point2i card_pos(Point2i::zero);
  for( auto itr = phand->begin(); itr != phand->end(); ++itr ) {

    card_pos.x = origin.x;
    card_pos.y = origin.y + (CARD_DIMS.h / 2) * hand_index;
    // if( origin.y > 0 ) {
    //   card_pos.x = origin.x;
    //   card_pos.y = origin.y + (CARD_DIMS.h / 2) * hand_index;
    // } else if( origin.x > 0 ) {
    //   card_pos.x = origin.x + (CARD_DIMS.w / 2) * hand_index;
    //   card_pos.y = origin.y;
    // }

    auto card_sp = (itr)->card_renderer;
    if( card_sp != nullptr ) {
      card_sp->set_position(card_pos);
      ++num_updates;
    } else {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION, "Card ID,", (itr)->id,
                    "was not valid; card renderer invalid." );
    }

    ++hand_index;
  }

  return num_updates;
}

// FIXME: Graphics related
void
CardDealer::render_player_hand(PlayerIndex pid,
                               const nom::RenderWindow& target)
{
  auto phand = this->player_hand(pid);
  if( phand != nullptr ) {
    for( auto itr = phand->begin(); itr != phand->end(); ++itr ) {

      auto card_renderer = (itr)->card_renderer;
      if( card_renderer != nullptr && card_renderer->valid() == true ) {
        card_renderer->render(target);
      }
    }
  }
}

// FIXME: Graphics related
void
CardDealer::render_player_score(PlayerIndex pid,
                                const nom::RenderWindow& target)
{
  auto res = this->players_.find(pid);
  if( res != this->players_.end() && res->second != nullptr ) {
    res->second->scoreboard_text.draw(target);
  }
}

void CardDealer::dump_deck() const
{
  uint32 player_index = 0;
  for( auto itr = this->players_.begin(); itr != this->players_.end(); ++itr ) {

    PlayerIndex pid = NOM_SCAST(PlayerIndex, player_index);
    this->dump_deck(pid);

    ++player_index;
  }
}

void CardDealer::dump_deck(PlayerIndex pid) const
{
  auto deck = this->player_deck(pid);

  NOM_ASSERT(deck != nullptr);
  if( deck != nullptr ) {

    NOM_LOG_INFO(TTCARDS, "player:", pid);

    for( auto itr = deck->begin(); itr != deck->end(); ++itr ) {
      NOM_LOG_INFO( TTCARDS, "card:", *itr);
    }
  }
}

void CardDealer::dump_hand() const
{
  uint32 player_index = 0;
  for( auto itr = this->players_.begin(); itr != this->players_.end(); ++itr ) {

    PlayerIndex pid = NOM_SCAST(PlayerIndex, player_index);
    this->dump_hand(pid);

    ++player_index;
  }
}

void CardDealer::dump_hand(PlayerIndex pid) const
{
  auto phand = this->player_hand(pid);

  NOM_ASSERT(phand != nullptr);
  if( phand != nullptr ) {
    NOM_LOG_INFO( TTCARDS, "player:", pid);

    for( auto itr = phand->begin(); itr != phand->end(); ++itr ) {
      NOM_LOG_INFO( TTCARDS, "card:", *itr);
    }
  }
}

} // namespace tt

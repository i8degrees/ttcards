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
#include "helpers.hpp"

// Private headers
#include "version.hpp"
#include "resources.hpp"
#include "CardRenderer.hpp"

// Forward declarations
#include "Card.hpp"
#include "Board.hpp"
#include "CardHand.hpp"
#include "CardResourceLoader.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/Gradient.hpp"
#include "nomlib/graphics/RenderWindow.hpp"
#include "nomlib/graphics/Text.hpp"
#include "nomlib/graphics/sprite/SpriteBatch.hpp"

using namespace nom;

namespace ttcards {

bool
save_game(Board* game_board, CardHand* player_hand)
{
  if( player_hand[PLAYER1].save(TTCARDS_PLAYER1_FILENAME) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Unable to save player 1 hand at:",
                  TTCARDS_PLAYER1_FILENAME );
    return false;
  }

  NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_APPLICATION, "Player 1 hand saved at:",
                  TTCARDS_PLAYER1_FILENAME );

  if( player_hand[PLAYER2].save(TTCARDS_PLAYER2_FILENAME) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Unable to save player 2 hand at:",
                  TTCARDS_PLAYER2_FILENAME );
    return false;
  }

  NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_APPLICATION, "Player 2 hand saved at:",
                  TTCARDS_PLAYER2_FILENAME );

  if( game_board->save(TTCARDS_BOARD_FILENAME) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Unable to save game board at:", TTCARDS_BOARD_FILENAME );
    return false;
  }

  NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_APPLICATION, "Game board saved at:",
                  TTCARDS_BOARD_FILENAME );

  // Success!
  return true;
}

bool
load_game(Board* game_board, CardHand* player_hand)
{
  if( player_hand[PLAYER1].load(TTCARDS_PLAYER1_FILENAME) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Unable to load player 1 hand from:",
                  TTCARDS_PLAYER1_FILENAME );
    return false;
  }

  NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Player 1 hand loaded from:", TTCARDS_PLAYER1_FILENAME );

  if( player_hand[PLAYER2].load(TTCARDS_PLAYER2_FILENAME) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Unable to load player 2 hand from:", TTCARDS_PLAYER2_FILENAME );
    return false;
  }

  NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Player 2 hand loaded from:", TTCARDS_PLAYER2_FILENAME );

  if( game_board->load(TTCARDS_BOARD_FILENAME) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Unable to load game board from:", TTCARDS_BOARD_FILENAME );
    return false;
  }

  NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_APPLICATION, "Game board loaded from:",
                  TTCARDS_BOARD_FILENAME );

  // Success!
  return true;
}

bool
render_card_background( nom::int32 player_id, const nom::Point2i& pos,
                        nom::Gradient* card_background,
                        nom::RenderTarget& target,
                        const nom::Texture* render_target )
{

  if( card_background == nullptr ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not render card background: NULL pointer." );
    return false;
  }

  if( card_background->valid() == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not render card background: invalid object." );
    return false;
  }

  // IMPORTANT: This must be done **after** the internals of nom::Gradient has
  // been updated with the final texture!
  if( render_target != nullptr ) {
    if( target.set_render_target(render_target) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not render card: render targets not supported." );
      return false;
    }
  }

  card_background->draw(target);

  if( render_target != nullptr ) {
    if( target.reset_render_target() == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not render card:",
                    "failed to reset the rendering target." );
      return false;
    }
  }

  return true;
}

bool
render_card_face( nom::int32 face_id, const nom::Point2i& pos,
                  nom::SpriteBatch* card_face,
                  nom::RenderTarget& target,
                  const nom::Texture* render_target )
{
  if( card_face == nullptr ) {
    return false;
  }

  if( card_face->valid() == false ) {
    return false;
  }

  card_face->set_frame(face_id);
  card_face->set_position(pos);

  if( render_target != nullptr ) {
    if( target.set_render_target(render_target) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not render card: render targets not supported." );
      return false;
    }
  }

  card_face->draw(target);

  if( render_target != nullptr ) {
    if( target.reset_render_target() == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not render card:",
                    "failed to reset the rendering target." );
      return false;
    }
  }

  return true;
}

bool render_card_element( nom::int32 element_id, const nom::Point2i& pos,
                          nom::SpriteBatch* card_element,
                          nom::RenderTarget& target,
                          const nom::Texture* render_target )
{
  if( card_element == nullptr ) {
    return false;
  }

  if( card_element->valid() == false ) {
    return false;
  }

  switch(element_id)
  {
    case ::NONE:
    default:
    {
      card_element->set_frame(ELEMENT_NONE);
    } break;

    case EARTH: card_element->set_frame(ELEMENT_EARTH); break;
    case FIRE: card_element->set_frame(ELEMENT_FIRE); break;
    case HOLY: card_element->set_frame(ELEMENT_HOLY); break;
    case ICE: card_element->set_frame (ELEMENT_ICE); break;
    case POISON: card_element->set_frame(ELEMENT_POISON); break;
    case THUNDER: card_element->set_frame(ELEMENT_THUNDER); break;
    case WATER: card_element->set_frame(ELEMENT_WATER); break;
    case WIND: card_element->set_frame(ELEMENT_WIND); break;
  }

  card_element->set_position(pos);

  if( render_target != nullptr ) {
    if( target.set_render_target(render_target) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not render card: render targets not supported." );
      return false;
    }
  }

  card_element->draw(target);

  if( render_target != nullptr ) {
    if( target.reset_render_target() == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not render card:",
                    "failed to reset the rendering target." );
      return false;
    }
  }

  return true;
}

bool
render_card_text( nom::int32 rank, const nom::Point2i& pos,
                  nom::Text* card_text, nom::RenderTarget& target,
                  const nom::Texture* render_target )
{
  if( card_text == nullptr ) {
    return false;
  }

  if( card_text->valid() == false ) {
    return false;
  }

  if( rank == 10 ) {
    card_text->set_text("A");
  } else {
    card_text->set_text( std::to_string(rank) );
  }

  // IMPORTANT: This must be done **after** the internals of nom::Text has been
  // updated with the final texture!
  if( render_target != nullptr ) {
    if( target.set_render_target(render_target) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not update cache: render targets not supported." );
      return false;
    }
  }

  card_text->set_position(pos);
  card_text->draw(target);

  if( render_target != nullptr ) {
    if( target.reset_render_target() == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not render card:",
                    "failed to reset the rendering target." );
      return false;
    }
  }

  return true;
}

bool
render_card(  const Card& card, const nom::Point2i& pos,
              const CardResourceLoader* res, nom::RenderTarget& target,
              const nom::Texture* render_target )
{
  uint32 player_id = card.getPlayerID();

  if( render_target != nullptr ) {
    if( target.set_render_target(render_target) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not render card: render targets not supported." );
      return false;
    }
  }

  if( card == Card::null || card.face_down() == true ) {

    render_card_face( NOFACE_ID, Point2i::zero, res->card_faces_.get(),
                      target, render_target );
  } else {

    render_card_background( player_id, Point2i::zero,
                            res->card_backgrounds_[player_id].get(), target,
                            render_target );

    render_card_face( card.getID(), Point2i::zero, res->card_faces_.get(),
                      target, render_target );

    render_card_element(  card.getElement(), ELEMENT_ORIGIN,
                          res->card_elements_.get(), target, render_target );

    render_card_text( card.getNorthRank(), RANK_NORTH_ORIGIN,
                      res->card_text_.get(), target, render_target );

    render_card_text( card.getEastRank(), RANK_EAST_ORIGIN,
                      res->card_text_.get(), target, render_target );

    render_card_text( card.getWestRank(), RANK_WEST_ORIGIN,
                      res->card_text_.get(), target, render_target );

    render_card_text( card.getSouthRank(), RANK_SOUTH_ORIGIN,
                      res->card_text_.get(), target, render_target );
  }

  if( render_target != nullptr ) {
    if( target.reset_render_target() == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not render card:",
                    "failed to reset the rendering target." );
      return false;
    }
  }

  return true;
}

void
modify_card_rank( CardResourceLoader* card_res, CardHand* player_hand, bool modifier,
                  nom::uint32 direction )
{
  NOM_ASSERT(card_res != nullptr);
  NOM_ASSERT(player_hand != nullptr);

  Card pcard = player_hand->getSelectedCard();
  std::array<nom::int32, MAX_RANKS> ranks = {{0}};
  CardsIterator pos = player_hand->begin();

  // First, obtain current rank attributes of the selected card; validation is
  // done for us by the Card class.
  ranks = pcard.getRanks();

  if(modifier) {
    // ...increase...

    ranks[direction] = ranks [direction] + 1;
    pcard.setRanks(ranks);
  } else {
    // ...decrease...

    // This clamps the decreased attribute to not falling below one (1).
    ranks[direction] = std::max( ranks[direction] - 1, 1);
    pcard.setRanks(ranks);
  }

  // Get the position of the selected card before we erase it so we can
  // reposition the new card at the same index.
  pos = pos + player_hand->at(pcard);

  player_hand->erase(pcard);

  // Render a new card based on the new card ranks text
  pcard.set_card_renderer( create_card_renderer(card_res, pcard) );
  NOM_ASSERT(pcard.card_renderer() != nullptr);
  NOM_ASSERT(pcard.card_renderer()->valid() == true);

  // Update the player hand with our modified card attributes
  player_hand->cards.insert(pos, pcard);

  // Update the player's selected card
  player_hand->set_position( player_hand->at(pcard) );
  // player_hand->selectCard(pcard);
}

void set_face_down(CardHand* player_hand, bool face_down)
{
  NOM_ASSERT(player_hand != nullptr);
  if( player_hand == nullptr ) {
    return;
  }

  for( auto itr = player_hand->begin(); itr != player_hand->end(); ++itr ) {
    itr->set_face_down(face_down);
  }

  // Update the card renderings
  player_hand->reinit();
}

// NOTE: This implementation derives from [Handmade Hero](https://www.handmadehero.org/)'s
// "Enforcing a Video Frame Rate" (Week 4). It is currently assumed that
// "granular sleep" AKA high-resolution timing is properly supported by the
// platform -- this might come back to bite us in the ass someday!
void set_frame_interval(nom::uint32 interval)
{
  real32 target_seconds_per_frame =
    1.0f / (real32)interval;
  uint64 last_delta = 0;
  HighResolutionTimer anim_timer;

  // Abort our frame capping logic when explicitly requested
  if( interval == 0 ) {
    return;
  }

  anim_timer.start();
  last_delta = anim_timer.ticks();

  real32 elapsed_delta =
    HighResolutionTimer::elapsed_ticks(last_delta, anim_timer.ticks() );

  if(elapsed_delta < target_seconds_per_frame) {

    uint32 sleep_ms =
      (uint32)(1000.0f * (target_seconds_per_frame - elapsed_delta) );
    if(sleep_ms > 0) {
      nom::sleep(sleep_ms);
    }
  }
}

std::string version_string()
{
  std::stringstream version_str;

  version_str << APP_NAME << " " << "v"
              << TTCARDS_VERSION_MAJOR << "."
              << TTCARDS_VERSION_MINOR << "."
              << TTCARDS_VERSION_PATCH;

#if ! defined(NDEBUG)
  version_str << "-Debug";
#endif

  return version_str.str();
}

} // namespace ttcards

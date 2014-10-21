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
#include "HumanPlayer.hpp"

// Forward declarations
#include "CardHand.hpp"
#include "CardView.hpp"

HumanPlayer::~HumanPlayer()
{
  // NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE);
}

HumanPlayer::HumanPlayer(CardHand* hand, CardView* view) :
  hand_(hand),
  card_renderer_(view),
  player_id_(0)
{
  //NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE);

  NOM_ASSERT(this->hand_ != nullptr);
  NOM_ASSERT(this->card_renderer_ != nullptr);
}

nom::uint32 HumanPlayer::player_id() const
{
  return this->player_id_;
}

// Maps the player's (card) hand with their respective ID; this keeps track of
// who's card is which and is used in CardView -- the rendering the card
// background -- and most importantly, in the Board class where we compare cards
// placed to determine whom's card to flip over to the respective player.
//
// card.player_owner is the *original* player owner of a card and should never be
// altered once set initially here.
//
void HumanPlayer::set_player_id(nom::uint32 id)
{
  this->player_id_ = id;

  NOM_ASSERT(this->hand_ != nullptr);
  for( nom::uint32 pid = 0; pid < this->hand_->cards.size(); pid++ )
  {
    this->hand_->cards[pid].setPlayerID(id);
    this->hand_->cards[pid].setPlayerOwner(id);
  }
}

void HumanPlayer::update()
{
  // this->card_renderer_->update();
}

void HumanPlayer::draw(nom::IDrawable::RenderTarget& target)
{
  nom::Point2i pos(nom::Point2i::zero);

  NOM_ASSERT(this->hand_ != nullptr);
  NOM_ASSERT(this->card_renderer_ != nullptr);
  for( nom::int32 idx = 0; idx < this->hand_->size(); idx++ ) {

    // Position of Player's cards in their hand
    pos.x = this->position().x;
    pos.y = this->position().y + ( CARD_HEIGHT / 2 ) * idx;

    if( this->hand_->position() == idx) {
      pos.x -= 16;
    }

    this->card_renderer_->reposition(pos);
    this->card_renderer_->setViewCard( this->hand_->cards.at(idx) );
    this->card_renderer_->draw(target);

  } // end for this->hand loop
}

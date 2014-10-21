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
#include "CPU_Player.hpp"

// Forward declarations
#include "CardHand.hpp"
#include "Board.hpp"
#include "BoardTile.hpp"
#include "CardView.hpp"

CPU_Player::~CPU_Player()
{
  // NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE);
}

CPU_Player::CPU_Player( Difficulty difficulty, Board* board, CardHand* hand,
                        CardView* view, const action_callback& action)
{
  // NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE);

  this->difficulty_ = difficulty;
  this->board_ = board;
  this->hand_ = hand;
  this->card_renderer_ = view;
  this->action_ = action;
  this->player_id_ = 1;

  NOM_ASSERT(this->hand_ != nullptr);
  NOM_ASSERT(this->board_ != nullptr);
  NOM_ASSERT(this->card_renderer_ != nullptr);
}

nom::uint32 CPU_Player::player_id() const
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
void CPU_Player::set_player_id(nom::uint32 id)
{
  this->player_id_ = id;

  NOM_ASSERT(this->hand_ != nullptr);
  for( nom::uint32 pid = 0; pid < this->hand_->cards.size(); pid++ )
  {
    this->hand_->cards[pid].setPlayerID(id);
    this->hand_->cards[pid].setPlayerOwner(id);
  }
}

BoardTile CPU_Player::random_move()
{
  BoardTile tile;

  NOM_ASSERT(this->hand_ != nullptr);
  NOM_ASSERT(this->board_ != nullptr);

  int moveX = nom::uniform_int_rand<int>(0, BOARD_GRID_WIDTH - 1);
  int moveY = nom::uniform_int_rand<int>(0, BOARD_GRID_HEIGHT - 1);

  if( this->hand_->size() > 0 ) {

    Card strongest_card = this->hand_->strongest();
    this->hand_->selectCard(strongest_card);

    NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CPU_PLAYER, "random_move" );
    tile.set_bounds( nom::IntRect( moveX, moveY, -1, -1 ) );
  }

  // No move was found; BoardTile::null
  return tile;
}

BoardTile CPU_Player::edge_move()
{
  BoardTile tile;

  NOM_ASSERT(this->hand_ != nullptr);
  NOM_ASSERT(this->board_ != nullptr);

  // int rand_choice = 0;
  nom::IntRect board_edges[4];

  // First turn
  // if( this->board_->getPlayerCount(0) == 0 && this->hand_->cards.size() > 0 )
  {
    board_edges[0].x = 0;
    board_edges[0].y = 0;

    board_edges[1].x = 2;
    board_edges[1].y = 0;

    board_edges[2].x = 0;
    board_edges[2].y = 2;

    board_edges[3].x = 2;
    board_edges[3].y = 2;

    // rand_choice = nom::rand(0, 3);

    if( this->hand_->size() > 0 ) {

      // nom::uint32 rand_pick = nom::rand ( 0, this->game->hand.size() - 1 );
      // this->game->hand.selectCard ( this->game->hand.cards[ rand_pick ] );

      Card strongest_card = this->hand_->strongest();
      this->hand_->selectCard(strongest_card);
    }

    if( this->board_->getStatus( board_edges[0].x, board_edges[0].y ) == BAD_CARD_ID )
    {
      NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CPU_PLAYER, "edge_move" );
      tile = BoardTile( this->hand_->getSelectedCard(),
                        nom::IntRect( board_edges[0].x, board_edges[0].y, -1, -1),
                        this->hand_->getSelectedCard().getElement() );
    }
    else if( this->board_->getStatus( board_edges[1].x, board_edges[1].y ) == BAD_CARD_ID )
    {
      NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CPU_PLAYER, "edge_move" );
      tile = BoardTile( this->hand_->getSelectedCard(),
                        nom::IntRect( board_edges[1].x, board_edges[1].y, -1, -1 ),
                        this->hand_->getSelectedCard().getElement() );
    }
    else if( this->board_->getStatus( board_edges[2].x, board_edges[2].y ) == BAD_CARD_ID )
    {
      NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CPU_PLAYER, "edge_move" );
      tile = BoardTile( this->hand_->getSelectedCard(),
                        nom::IntRect( board_edges[2].x, board_edges[2].y, -1, -1 ),
                        this->hand_->getSelectedCard().getElement() );
    }
    else if( this->board_->getStatus(board_edges[3].x, board_edges[3].y) == BAD_CARD_ID )
    {
      NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CPU_PLAYER, "edge_move" );
      tile = BoardTile( this->hand_->getSelectedCard(),
                        nom::IntRect( board_edges[3].x, board_edges[3].y, -1, -1 ),
                        this->hand_->getSelectedCard().getElement() );
    }
  }

  // No move was found; BoardTile::null
  return tile;
}

BoardTile CPU_Player::best_move()
{
  BoardTile tile;

  NOM_ASSERT(this->hand_ != nullptr);
  NOM_ASSERT(this->board_ != nullptr);

  for( auto y = 0; y != BOARD_GRID_HEIGHT; ++y ) {
    for( auto x = 0; x != BOARD_GRID_WIDTH; ++x ) {
      for( auto itr = this->hand_->cards.begin(); itr != this->hand_->cards.end(); ++itr ) {

        // Compare card's WEST rank with opponent's EAST rank
        if( x != 0 ) {

          if( //board_->getPlayerID(x, y) != board_->getPlayerID(x - 1, y) &&
              // Validity check: existing tile must be owned by the other player
              board_->getPlayerID(x - 1, y) == 1 &&
              // Validity check: existing tile must **not** be empty
              board_->getStatus(x - 1, y) != BAD_CARD_ID &&
              // Validity check: potential tile must be empty
              board_->getStatus(x, y) == BAD_CARD_ID ) {

            if( (*itr).getWestRank() >
                board_->tile(x - 1, y).tile().getEastRank() == true ) {

              NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_CPU_PLAYER,
                              "best_move:",
                              (*itr).getName(), "[W > E]",
                              // Opponent's card
                              board_->tile(x - 1, y).tile().getName() );

              tile.set_bounds( board_->tile(x,y).bounds() );
              this->hand_->selectCard( (*itr) );
              return tile;
            }
          }
        }

        // Compare card's SOUTH rank with opponent's NORTH rank
        if( y != BOARD_GRID_HEIGHT - 1 ) {

          if( board_->getPlayerID(x, y) != board_->getPlayerID(x, y + 1) &&
              // Validity check: existing tile must be owned by the other player
              board_->getPlayerID(x, y + 1) == 1 &&
              // Validity check: existing tile must **not** be empty
              board_->getStatus(x, y + 1) != BAD_CARD_ID &&
              // Validity check: potential tile must be empty
              board_->getStatus(x, y) == BAD_CARD_ID ) {

            if( (*itr).getSouthRank() >
                board_->tile(x, y + 1).tile().getNorthRank() == true ) {

              NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_CPU_PLAYER,
                              "best_move:",
                              (*itr).getName(), "[S > N]",
                              // Opponent's card
                              board_->tile(x, y + 1).tile().getName() );

              tile.set_bounds( board_->tile(x,y).bounds() );
              this->hand_->selectCard( (*itr) );
              return tile;
            }
          }
        }

        // Compare card's EAST rank with opponent's WEST rank
        if( x != BOARD_GRID_WIDTH - 1 ) {

          if( //board_->getPlayerID(x, y) != board_->getPlayerID(x + 1, y) &&
              // Validity check: existing tile must be owned by the other player
              board_->getPlayerID(x + 1, y) == 1 &&
              // Validity check: existing tile must **not** be empty
              board_->getStatus(x + 1, y) != BAD_CARD_ID &&
              // Validity check: potential tile must be empty
              board_->getStatus(x, y) == BAD_CARD_ID ) {

            if( (*itr).getEastRank() >
                board_->tile(x + 1, y).tile().getWestRank() == true ) {

              NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_CPU_PLAYER,
                              "best_move:",
                              (*itr).getName(), "[E > W]",
                              // Opponent's card
                              board_->tile(x + 1, y).tile().getName() );

              tile.set_bounds( board_->tile(x,y).bounds() );
              this->hand_->selectCard( (*itr) );
              return tile;
            }
          }
        }

        // Compare card's NORTH rank with opponent's SOUTH rank
        if( y != 0 ) {

          if( //board_->getPlayerID(x, y) != board_->getPlayerID(x, y - 1) &&
              // Validity check: existing tile must be owned by the other player
              board_->getPlayerID(x, y - 1) == 1 &&
              // Validity check: existing tile must **not** be empty
              board_->getStatus(x, y - 1) != BAD_CARD_ID &&
              // Validity check: potential tile must be empty
              board_->getStatus(x, y) == BAD_CARD_ID ) {

            // Card comparison
            if( (*itr).getNorthRank() >
                board_->tile(x, y - 1).tile().getSouthRank() == true ) {

              NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_CPU_PLAYER,
                              "best_move:",
                              (*itr).getName(), "[N > S]",
                              // Opponent's card
                              board_->tile(x, y - 1).tile().getName() );

              tile.set_bounds( board_->tile(x,y).bounds() );
              this->hand_->selectCard( (*itr) );
              return tile;
            }
          }
        }
      }
    }
  }

  // No move was found; BoardTile::null
  return tile;
}

void CPU_Player::update()
{
  BoardTile tile;

  // You should see this only once per CPU turn -- otherwise something is
  // probably amiss!
  NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CPU_PLAYER, "...thinking..." );

  tile = this->best_move();

  if( tile == BoardTile::null ) {
    tile = this->edge_move();
  }

  if( tile == BoardTile::null ) {
    tile = this->random_move();
  }

  if( tile != BoardTile::null ) {
    this->action_.operator()(tile);
  }
}

void CPU_Player::draw(nom::IDrawable::RenderTarget& target)
{
  nom::Point2i pos(nom::Point2i::zero);

  NOM_ASSERT(this->hand_ != nullptr);
  NOM_ASSERT(this->card_renderer_ != nullptr);
  for( nom::int32 idx = 0; idx < this->hand_->size(); idx++ ) {

    // Position of Player's cards in their hand
    pos.x = this->position().x;
    pos.y = this->position().y + ( CARD_HEIGHT / 2 ) * idx;

    if( this->hand_->position() == idx) {
      pos.x += 16;
    }

    this->card_renderer_->reposition(pos);
    this->card_renderer_->setViewCard( this->hand_->cards.at(idx) );
    this->card_renderer_->draw(target);

  } // end for this->hand loop
}

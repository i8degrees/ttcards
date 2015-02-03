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
#include "CardsMenuState.hpp"

// Forward declarations
#include "Game.hpp"

using namespace nom;

CardsMenuState::CardsMenuState(nom::SDLApp* object) :
  nom::IState( Game::State::CardsMenu ),
  game( NOM_SCAST(Game*, object) ),
  cursor_state_(0)
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  nom::uint pid = 0; // temp var for for loop iteration

  this->game->hand[0].clear();
  this->game->hand[1].clear();

  // Generate a full player hand for the CPU with whatever -- don't care -- we
  // do this only for creating a static backdrop to add to this state; see
  // ::on_draw for details.
  while( this->game->hand[1].size() < MAX_PLAYER_HAND )
  {
    this->game->hand[1].shuffle( 1, 1, this->game->collection);
  }
  this->game->hand[1].set_face_down(true);

  // this->game->collection is initialized for us in the main app loop
  // Borrowed from Player class; this is perhaps a hack-(ish) workaround
    /* FIXME: */
  // As we presently expect / depend on this state to always be player1 and we
  // know the only required data structure being passed onwards is the player1
  // hand, we can fudge all the cards / objects leading up to said point without
  // anybody knowing the difference.
  //
  for ( pid = 0; pid < this->game->collection.cards.size(); pid++ )
  {
    this->game->collection.cards[pid].setPlayerID(Card::PLAYER1);
  }

  this->selected_card_ = this->game->collection.cards.front();

  this->card_pos = nom::Point2i (
                                  BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ),
                                  BOARD_ORIGIN_Y +
                                  (
                                    ( CARD_HEIGHT / 2 ) + CARD_HEIGHT * 1
                                  ) - 8
                                );
}

CardsMenuState::~CardsMenuState()
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->selected_card_ = Card();
}

void CardsMenuState::on_init( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->music_track->Play();

  // Reset the state of the cards model to ensure no persistent state; this
  // can lead to subtle bugs or worse if we don't do this
  this->game->cards_page_model_.reset( new CardsPageDataSource("cards_db") );

  if( this->game->cards_menu_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->game->cards_menu_.load_document_file( this->game->config.getString("GUI_CARDS_MENU") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_CARDS_MENU") );
      // return false;
    }
  #else
    if( this->game->cards_menu_.load_document_file( this->game->config.getString("GUI_CARDS_MENU_SCALE2X") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_CARDS_MENU_SCALE2X") );
      // return false;
    }
  #endif

  NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_CARDS_MENU_STATE,
                "---Deck from collection---" );

  // Create the card pages with an existing collection
  Cards deck;
  for(  auto itr = this->game->collection.cards.begin();
        itr != this->game->collection.cards.end(); ++itr )
  {
    // FIXME: This breaks card lookup, rendering, etc. due to the fact that up
    // to this point, we've been depending on the ID of the card matching the
    // array index.
    // Options I've thought of: experiment with the idea of modeling the data
    // with std::map<int,Card>, keeping a copy of the position index with the
    // card or model (yuck!) ...
    // if( (*itr).num() > 0 ) {
      deck.push_back( (*itr) );
    // }

      NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CARDS_MENU_STATE,
                    (*itr).getID(), ":", (*itr).getName(),
                    "[", (*itr).num(), "]" );
  }

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ != nullptr ) {
    this->game->cards_page_model_->insert_cards(0, deck);
  }

  // Build card row data
  this->game->gui_window_.update();

  NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CARDS_MENU_STATE,
                  "---Cards page model---",
                  this->game->cards_page_model_->dump() );

  // Event listener for mouse button clicks
  this->game->cards_menu_.register_event_listener(
    this->game->cards_menu_.document(), "mouseup",
    new nom::UIEventListener( [&] ( Rocket::Core::Event& ev ) {
      this->on_mouse_button_up(ev);
    })
  );

  // Build offset coordinate map for the game cursor; this is necessary for
  // syncing key, mouse wheel and joystick input.
  Rocket::Core::ElementList tags;
  Rocket::Core::Element* top =
    this->game->cards_menu_.document()->GetElementById("content");

  NOM_ASSERT(top != nullptr);
  if( top != nullptr ) {
    Rocket::Core::ElementUtilities::GetElementsByTagName(tags, top, "datagridrow");

    // NOM_DUMP( tags.size() );
    Rocket::Core::Vector2f position;
    Rocket::Core::Vector2f size;
    IntRect row;
    for(auto itr = tags.begin(); itr != tags.end(); ++itr)
    {
      // NOM_DUMP( (*itr)->GetTagName().CString() );

      position = (*itr)->GetAbsoluteOffset(Rocket::Core::Box::PADDING);
      size = (*itr)->GetBox().GetSize(Rocket::Core::Box::PADDING);

      row.x = position.x - this->game->cursor_.size().w;
      row.y = position.y + this->game->cursor_.size().h / 2;
      row.w = size.x;

      // This value correlates with the line-height RCSS property value in
      // gui/dataview.rml and must **not** be fractional, or else we accumulate
      // rounding err with each cursor offset row that results in mis-placement
      // of the cursor. This can ultimately break its intended Y-axis bounds.
      row.h = size.y;

      this->cursor_coords_map_.push_back(row);

      NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "cursor_coords_map_:", row);

      // Check the value of row.h to ensure non-fractional
      NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "datagridrow:", position.x, ",", position.y, ",", size.x, ",", size.y );
    }
  }

  // Starting origin for game cursor
  this->game->cursor_.set_position( this->cursor_coords_map_[0].position() );
  this->game->cursor_.set_frame(INTERFACE_CURSOR_RIGHT); // default cursor image

  this->cursor_state_ = 0;  // default state for navigating card menu

  nom::InputActionMapper state;

  nom::EventCallback cursor_prev( [&] ( const nom::Event& evt ) { this->cursor_prev(); } );
  nom::EventCallback cursor_next( [&] ( const nom::Event& evt ) { this->cursor_next(); } );
  nom::EventCallback prev_page( [&] ( const nom::Event& evt ) { this->prev_page(); } );
  nom::EventCallback next_page( [&] ( const nom::Event& evt ) { this->next_page(); } );

  nom::EventCallback delete_card( [&] ( const nom::Event& evt ) { this->remove_card(this->selected_card_); } );
  nom::EventCallback select_card( [&] ( const nom::Event& evt ) { this->add_card(this->selected_card_); } );

  // nom::EventCallback click_delete_card( [&] ( const nom::Event& evt ) { this->on_right_mouse_button_up(evt); } );
  // nom::EventCallback click_select_card( [&] ( const nom::Event& evt ) { this->on_left_mouse_button_up(evt); } );

  nom::EventCallback pause_game( [&] ( const nom::Event& evt ) { this->game->set_state( Game::State::Pause ); } );
  nom::EventCallback start_game( [&] (const nom::Event& evt) {
    this->game->set_state( Game::State::ConfirmationDialog );
  });

  state.insert( "cursor_prev", nom::KeyboardAction( SDL_KEYDOWN, SDLK_UP ), cursor_prev );
  state.insert( "cursor_prev", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::UP ), cursor_prev );
  state.insert( "cursor_prev", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::UP ), cursor_prev );

  state.insert( "cursor_next", nom::KeyboardAction( SDL_KEYDOWN, SDLK_DOWN ), cursor_next );
  state.insert( "cursor_next", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::DOWN ), cursor_next );
  state.insert( "cursor_next", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::DOWN ), cursor_next );

  state.insert( "prev_page", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFT ), prev_page );
  state.insert( "prev_page", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_X, nom::MouseWheelAction::LEFT ), prev_page );
  state.insert( "prev_page", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::LEFT ), prev_page );

  state.insert( "next_page", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHT ), next_page );
  state.insert( "next_page", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_X, nom::MouseWheelAction::RIGHT ), next_page );
  state.insert( "next_page", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::RIGHT ), next_page );

  state.insert( "delete_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_d ), delete_card );

  state.insert( "delete_card", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CIRCLE ), delete_card );

  state.insert( "select_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_SPACE ), select_card );
  state.insert( "select_card", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CROSS ), select_card );

  // state.insert( "pause_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_p ), pause_game );
  // state.insert( "pause_game", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::START ), pause_game );

  state.insert( "start_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RETURN ), start_game );
  state.insert( "start_game", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::START ), start_game );

  // Mouse button mappings
  // state.insert( "click_delete_card", nom::MouseButtonAction( SDL_MOUSEBUTTONUP, SDL_BUTTON_RIGHT ), click_delete_card );
  // state.insert( "click_select_card", nom::MouseButtonAction( SDL_MOUSEBUTTONUP, SDL_BUTTON_LEFT ), click_select_card );

  this->game->input_mapper.erase( "CardsMenuState" );
  this->game->input_mapper.insert( "CardsMenuState", state, true );
  this->game->input_mapper.activate_only( "CardsMenuState" );
  this->game->input_mapper.activate( "Game" );

  this->game->cards_menu_.show();
}

void CardsMenuState::on_exit( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  // Clear the CPU player's hand; it needs to be re-initialized with real data
  // in PlayState
  this->game->hand[1].clear();

  this->game->cards_menu_.close();
  Rocket::Core::Factory::ClearStyleSheetCache();
  Rocket::Core::Factory::ClearTemplateCache();
}

void CardsMenuState::on_pause(nom::void_ptr data)
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  // Hide the cursor so that it doesn't show up during undesirable states such
  // as during the ConfirmationDialogState or Pause states.
  this->game->cursor_.set_frame(INTERFACE_CURSOR_NONE);
}

void CardsMenuState::on_resume(nom::void_ptr data)
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  // Restore the rendering of the game cursor
  this->game->cursor_.set_frame(INTERFACE_CURSOR_RIGHT);

  // Response from the previous game state
  nom::int32_ptr response = static_cast<nom::int32_ptr>(data);

  // User is happy with their cards -- let's play! Response from
  // ConfirmationDialogState was 'yes'
  if( response != nullptr && *response == 1 ) {
    this->game->set_state(Game::State::Play);
  }
  else {
    // Continue running this state; response from ConfirmationDialogState was 'no'
    this->game->input_mapper.activate_only("CardsMenuState");
    this->game->input_mapper.activate("Game");
  }
}

void CardsMenuState::on_update( float delta_time )
{
  this->game->card.update();

  this->update_cursor();

  this->game->window.update();
  this->game->gui_window_.update();

  this->update_page_indicators();
}

void CardsMenuState::on_draw( nom::RenderWindow& target )
{
  this->game->background.draw(target);

  // Static backdrop for CPU player
  for( auto idx = 0; idx != this->game->hand[1].size(); ++idx)
  {
    this->player2_pos = Point2i (
                                  PLAYER2_ORIGIN_X,
                                  PLAYER2_ORIGIN_Y +
                                  ( CARD_HEIGHT / 2 ) * idx
                                );

    this->game->card.reposition(this->player2_pos);
    this->game->card.setViewCard(this->game->hand[1].cards[idx]);
    this->game->card.draw(target);
  }

  // Active player's card selection(s)
  for( nom::uint32 idx = 0; idx < this->game->hand[0].size(); idx++ )
  {
    this->player1_pos = Point2i (
                                  PLAYER1_ORIGIN_X,
                                  PLAYER1_ORIGIN_Y +
                                  ( CARD_HEIGHT / 2 ) * idx
                              );

    this->game->card.reposition(this->player1_pos);
    this->game->card.setViewCard( this->game->hand[0].cards.at ( idx ) );
    this->game->card.draw(target);
  }

  this->game->gui_window_.draw();
  this->game->cursor_.draw(target);

  this->game->card.setViewCard(this->selected_card_);
  this->game->card.reposition(this->card_pos);
  this->game->card.draw(target);
}

// Private scope

bool CardsMenuState::on_event( const nom::Event& ev )
{
  this->game->gui_window_.process_event(ev);

  return true;
}

void CardsMenuState::on_mouse_button_up(Rocket::Core::Event& ev)
{
  using namespace Rocket::Core;

  Element* target = ev.GetTargetElement();

  NOM_ASSERT( target != nullptr );
  if( target == nullptr ) return;

  if( ev == "mouseup" ) {

    Input::KeyIdentifier button =
      (Input::KeyIdentifier) ev.GetParameter<int>("button", 3);

    NOM_ASSERT( this->game->cards_page_model_ != nullptr );
    if( this->game->cards_page_model_ == nullptr ) return;

    Card card = this->game->cards_page_model_->lookup_by_name( target->GetInnerRML().CString() );

    if( target->GetTagName() == "card" && button == 0 ) // Left click
    {
      this->add_card(card);
    }
    else if( target->GetTagName() == "card" && button == 1 )  // Right click
    {
      this->remove_card(card);
    }

    NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_TEST, target->GetInnerRML().CString() );
  } // end if mouseup event
}

// void CardsMenuState::on_left_mouse_button_up(const nom::Event& ev)
// {
//   using namespace Rocket::Core;
//   Vector2f mouse_coords(ev.mouse.x, ev.mouse.y);

//   Element* target =
//     this->game->cards_menu_.document()->GetElementById("datagridrow");
//   NOM_ASSERT( target != nullptr );
//   if( target == nullptr ) return;

//   NOM_ASSERT( this->game->cards_page_model_ != nullptr );
//   if( this->game->cards_page_model_ == nullptr ) return;

//   Card card =
//     this->game->cards_page_model_->lookup_by_name( target->GetInnerRML().CString() );

//   if( target->IsPointWithinElement(mouse_coords) ) {
//       this->add_card(card);
//   } // end if target is within mouse hit test

//   NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_TEST, target->GetInnerRML().CString() );
// }

// void CardsMenuState::on_right_mouse_button_up(const nom::Event& ev)
// {
//   using namespace Rocket::Core;
//   Vector2f mouse_coords(ev.mouse.x, ev.mouse.y);

//   Element* target =
//     this->game->cards_menu_.document()->GetElementById("datagridrow");
//   NOM_ASSERT( target != nullptr );
//   if( target == nullptr ) return;

//   NOM_ASSERT( this->game->cards_page_model_ != nullptr );
//   if( this->game->cards_page_model_ == nullptr ) return;

//   Card card =
//     this->game->cards_page_model_->lookup_by_name( target->GetInnerRML().CString() );

//   if( target->IsPointWithinElement(mouse_coords) ) {
//       this->remove_card(card);
//   } // end if target is within mouse hit test

//   NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_TEST, target->GetInnerRML().CString() );
// }

void CardsMenuState::update_cursor()
{
  int row_index = 0;      // Card position index as per cards model
  int pos = 0;            // The card's position index (as per CardCollection)

  if( this->cursor_state_ == 0 )
  {
    this->game->cursor_.set_frame(INTERFACE_CURSOR_RIGHT);

    row_index = this->cursor_position();

    // Update selected card
    pos = this->game->cards_page_model_->map_row(row_index);
    this->selected_card_ = this->game->cards_page_model_->lookup_by_id(pos);
  }
}

void CardsMenuState::update_page_indicators()
{
  using namespace Rocket::Core;

  int page = 0;         // The current card page
  int num_pages = 0;  // Total card pages
  Element* page_left = nullptr;
  Element* page_right = nullptr;

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ == nullptr ) {
    return;
  }

  page = this->game->cards_page_model_->page();
  num_pages = this->game->cards_page_model_->total_pages();

  // Update left page indicator
  page_left =
    this->game->cards_menu_.document()->GetElementById("page-left");
  NOM_ASSERT( page_left != nullptr );

  if( page_left ) {
    page_left->SetClassNames("");

    if( page > 0 ) {
      page_left->SetClassNames("page-left");
    }
  } // end if page_left

  // Update right page indicator
  page_right =
    this->game->cards_menu_.document()->GetElementById("page-right");
  NOM_ASSERT( page_right != nullptr );

  if( page_right ) {
    page_right->SetClassNames("");

    if( page >= 0 && (page < num_pages - 1) ) {
      page_right->SetClassNames("page-right");
    }
  } // end if page_right
}

int CardsMenuState::cursor_position()
{
  NOM_ASSERT( this->game->cards_page_model_ != nullptr );

  int pos = 0;

  if( this->game->cards_page_model_ != nullptr ) {

    for(auto idx = 0; idx < this->game->cards_page_model_->per_page(); ++idx)
    {
      if( this->game->cursor_.position().y <= this->cursor_coords_map_.at(idx).y ) {
        return idx;
      }
      else { // catch all safety switch

        // assume we are at the last position in the index when all else fails
        pos = this->game->cards_page_model_->per_page();
      }
    }
  }

  return pos;
}

void CardsMenuState::set_cursor_position(int pos)
{
  this->game->cursor_.set_position( this->cursor_coords_map_.at(pos).position() );
  NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "cursor_pos:", this->game->cursor_.position() );
}

void CardsMenuState::prev_page()
{
  using namespace Rocket::Core;

  int page = 0;

  if( this->cursor_state_ == 0 ) {

    NOM_ASSERT( this->game->cards_page_model_ != nullptr );
    if( this->game->cards_page_model_ == nullptr ) return;

    page = this->game->cards_page_model_->page();

    // Update cards model, play audio & set column text for previous page
    if( page > 0 )
    {
      --page;
      this->game->cards_page_model_->set_page(page);

      this->game->cursor_move->Play();

      // Update column with page number
      this->game->cards_menu_.set_column_title( 1, "CARDS P. " + std::to_string(page+1) );
    }
  } // end if cursor state == 0
}

void CardsMenuState::next_page()
{
  using namespace Rocket::Core;

  int page = 0;
  int num_pages = 0;

  if( this->cursor_state_ == 0 ) {

    NOM_ASSERT( this->game->cards_page_model_ != nullptr );
    if( this->game->cards_page_model_ == nullptr ) return;

    page = this->game->cards_page_model_->page();
    num_pages = this->game->cards_page_model_->total_pages();

    // Update cards model, play audio & set column text for next page
    if( page < num_pages - 1 )
    {
      ++page;
      this->game->cards_page_model_->set_page(page);

      this->game->cursor_move->Play();

      // Update column with page number
      this->game->cards_menu_.set_column_title( 1, "CARDS P. " + std::to_string(page+1) );
    }

  } // end if cursor state == 0
}

void CardsMenuState::cursor_prev()
{
  int row_index = 0;      // Card position index as per cards model
  int pos = 0;            // The card's position index (as per CardCollection)
  Point2i move_to_offset(Point2i::zero);

  if( this->cursor_state_ == 0 ) {

    // Move up if the game cursor is before the first card entry of the page
    if( this->game->cursor_.position().y > this->cursor_coords_map_.at(0).y ) {

      move_to_offset.x = 0;
      move_to_offset.y = -(this->cursor_coords_map_.at(pos).h);
      this->game->cursor_.translate(move_to_offset);

      NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "cursor.y:", this->game->cursor_.position().y );
      row_index = this->cursor_position();

      pos = this->game->cards_page_model_->map_row(row_index);
      this->selected_card_ = this->game->cards_page_model_->lookup_by_id(pos);

      this->game->cursor_move->Play();
    }
  } // end if cursor state == 0
}

void CardsMenuState::cursor_next()
{
  int row_index = 0;      // Card position index as per cards model
  int pos = 0;            // The card's position index (as per CardCollection)
  int max_per_page = 0;   // maximum number of cards per one page
  Point2i move_to_offset(Point2i::zero);

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ != nullptr ) {
    // Next to last card
    max_per_page = this->game->cards_page_model_->per_page() - 1;
    NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "cursor.at(max_per_page).y:", this->cursor_coords_map_.at(max_per_page).y );
  }

  if( this->cursor_state_ == 0 ) {

    // Move down if the game cursor is not at the last card entry of the page
    if( this->game->cursor_.position().y >= this->cursor_coords_map_.at(0).y &&
        this->game->cursor_.position().y < this->cursor_coords_map_.at(max_per_page).y )
    {
      move_to_offset.x = 0;
      move_to_offset.y = this->cursor_coords_map_.at(pos).h;
      this->game->cursor_.translate(move_to_offset);

      NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "cursor.y:", this->game->cursor_.position().y );
      row_index = this->cursor_position();

      pos = this->game->cards_page_model_->map_row(row_index);
      this->selected_card_ = this->game->cards_page_model_->lookup_by_id(pos);

      this->game->cursor_move->Play();
    }
  } // end if cursor state == 0
}

void CardsMenuState::add_card(const Card& card)
{
  using namespace Rocket::Core;

  int pos = 0;            // The card's position index (as per CardCollection)
  int page = 0;           // Current page of model
  int row_index = 0;      // Element's position of the card (relative to page)

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ == nullptr ) return;

  page = this->game->cards_page_model_->page();
  Card c = card;
  pos = c.getID();

  // Card logic for adding a card to the player's hand
  //
  // 1. Decrease available card count by one
  // 2. Sync the cards model to reflect modified card count (-1)
  // 3. Update player hand w/ modified card.
  // 4. Update game cursor position.
  // 5. Queue audio clip
  if( c.num() > 0 )
  {
    c.set_num( c.num() - 1 );

    if( this->game->hand[0].push_back(c) == true ) {

      this->game->cards_page_model_->insert_card(pos, c);
      this->selected_card_ = c;

      // Get the position, relative to current page, from model for
      // updating game cursor position
      row_index =
        this->game->cards_page_model_->map_page_row(pos, page);
      this->set_cursor_position(row_index);

      this->game->card_place->Play();
    } // end if successful add
  }
}

void CardsMenuState::remove_card(const Card& card)
{
  using namespace Rocket::Core;

  int pos = 0;            // The card's position index (as per CardCollection)
  int page = 0;           // Current page of model
  int row_index = 0;      // Element's position of the card (relative to page)

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ == nullptr ) return;

  page = this->game->cards_page_model_->page();
  Card c = card;
  pos = c.getID();

  // Used to compare the selected card from the current model with the game
  // cards collection (number of cards to return)
  Card ref_card = this->game->collection.cards.at(pos);

  // Card logic for removing a card from the player's hand
  //
  // 1. Increase available card count by one
  // 2. Sync the cards model to reflect modified card count (+1)
  // 3. Remove the card from the player's hand.
  // 4. Update game cursor position.
  // 5. Queue audio clip
  if( c.num() < ref_card.num() )
  {
    c.set_num( c.num() + 1 );

    if( this->game->hand[0].erase(c) == true ) {

      this->game->cards_page_model_->insert_card(pos, c);
      this->selected_card_ = card;

      // Get the position, relative to current page, from model for
      // updating game cursor position
      row_index =
        this->game->cards_page_model_->map_page_row(pos, page);
      this->set_cursor_position(row_index);

      this->game->cursor_cancel->Play();
    } // end if successful removal
  }
}

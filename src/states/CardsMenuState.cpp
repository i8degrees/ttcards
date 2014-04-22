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

using namespace nom;

CardsMenuState::CardsMenuState ( const nom::SDLApp::shared_ptr& object ) :
  game { NOM_PTR_CAST( Game, object) }
{
  nom::Gradient linear;

  nom::uint pid = 0; // temp var for for loop iteration

  NOM_LOG_TRACE( TTCARDS );

  this->game->hand[0].clear();
  this->game->hand[1].clear();

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
    this->game->collection.cards[pid].setPlayerID ( Card::PLAYER1 );
  }

  linear.set_start_color ( nom::Color4i::Gray );
  linear.set_end_color ( nom::Color4i::LightGray );
  linear.set_fill_direction ( nom::Gradient::FillDirection::Left );

  Point2i menu_box_origin = Point2i( PICK_CARDS_MENU_ORIGIN_X, PICK_CARDS_MENU_ORIGIN_Y );
  Size2i menu_box_size = Size2i( PICK_CARDS_MENU_WIDTH, PICK_CARDS_MENU_HEIGHT );

  nom::Window::WindowShape style;
  // style.push_back( nom::IDrawable::SharedPtr( new nom::Gradient( linear ) ));
  style.push_back( nom::IDrawable::SharedPtr( new nom::GrayWindow( menu_box_origin, menu_box_size ) ));

  nom::Window* window = new nom::Window( menu_box_origin, menu_box_size, style );

  this->menu_box = nom::MessageBox  (
                                      window,
                                      menu_box_origin,
                                      menu_box_size
                                    );

  this->per_page = 11; // number of cards to display per menu page
  this->total_pages = this->game->collection.cards.size() / per_page;
  this->current_index = 0; // current card position

  this->selectedCard = this->game->collection.cards.front();

  this->card_pos = nom::Point2i (
                                  BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ),
                                  BOARD_ORIGIN_Y +
                                  (
                                    ( CARD_HEIGHT / 2 ) + CARD_HEIGHT * 1
                                  ) - 8
                                );
}

CardsMenuState::~CardsMenuState ( void )
{
NOM_LOG_TRACE ( TTCARDS );

  this->selectedCard = Card();
}

void CardsMenuState::on_init ( nom::void_ptr data )
{
  unsigned int idx = 0; // iterator for cursor_coords_map

  // Initialize card name text so that we can obtain height info early on
  this->card_text.set_font ( this->game->info_text );
  this->card_text.set_text ( this->game->collection.cards[0].getName() );

  //this->info_text_height = this->card_text.height();
  // FIXME: Height calculation is messed up
  this->info_text_height = this->card_text.height() + 4;

  // FIXME: We must set the menu_box labels to quiet the debug logs
  this->menu_box.set_title ( nom::Text("",this->game->info_small_text,0) );
  this->menu_box.set_text ( nom::Text("",this->game->info_text,0) );

  this->title_text.set_font ( this->game->info_small_text );

  this->game->cursor.set_position ( Point2i(MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y) );
  this->game->cursor.set_frame ( INTERFACE_CURSOR_RIGHT ); // default cursor image
  this->game->cursor.set_state ( 0 ); // default state for navigating card menu

  for ( idx = 0; idx < per_page; idx++ )
  {
    this->cursor_coords_map[idx] = std::make_pair ( MENU_CARDS_CURSOR_ORIGIN_Y + ( this->info_text_height * idx ), idx );

    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << "\nidx:" << std::get<1>(this->cursor_coords_map[idx]) << " " << "y:" << std::get<0>(this->cursor_coords_map[idx]) << "\n";
    #endif
  }
}

void CardsMenuState::on_exit ( nom::void_ptr data )
{
  std::cout << "\n" << "CardsMenu state onExit" << "\n";
}

void CardsMenuState::on_pause ( nom::void_ptr data )
{
  std::cout << "\n" << "CardsMenu state Paused" << "\n";
}

void CardsMenuState::on_resume ( nom::void_ptr data )
{
  std::cout << "\n" << "CardsMenu state Resumed" << "\n";
}

void CardsMenuState::on_key_down( const nom::Event& ev )
{
  switch ( ev.key.sym )
  {
    default: break;

    // Pause game
    case SDLK_p:
    {
      this->game->set_state( Game::State::Pause );
      break;
    }

    case SDLK_RETURN:
    {
      this->game->set_state( Game::State::Play );
      break;
    }

    case SDLK_LEFT: this->moveCursorLeft(); break;
    case SDLK_RIGHT: this->moveCursorRight(); break;
    case SDLK_UP: this->moveCursorUp(); break;
    case SDLK_DOWN: this->moveCursorDown(); break;

    case SDLK_d: if ( this->game->hand[0].erase( this->selectedCard ) ) this->game->cursor_cancel.Play(); break;

    case SDLK_SPACE:
    {
      if ( this->game->hand[0].push_back ( this->selectedCard ) )
      {
        this->game->card_place.Play();
      }
      break;
    }
  } // end switch ( key )
}

void CardsMenuState::on_joy_button_down( const nom::Event& ev )
{
  switch ( ev.jbutton.button )
  {
    default: break;

    case nom::PSXBUTTON::UP: this->moveCursorUp(); break;
    case nom::PSXBUTTON::RIGHT: this->moveCursorRight(); break;
    case nom::PSXBUTTON::DOWN: this->moveCursorDown(); break;
    case nom::PSXBUTTON::LEFT: this->moveCursorLeft(); break;

    case nom::PSXBUTTON::TRIANGLE: /* TODO */ break;
    case nom::PSXBUTTON::CIRCLE: if ( this->game->hand[0].erase ( this->selectedCard ) ) this->game->cursor_cancel.Play(); break;
    case nom::PSXBUTTON::CROSS: if ( this->game->hand[0].push_back ( this->selectedCard ) ) this->game->card_place.Play(); break;

    case nom::PSXBUTTON::START:
    {
      this->game->set_state( 1 );
      break;
    }
  } // switch
}

void CardsMenuState::on_mouse_left_button_down( const nom::Event& ev )
{
  // TODO
}

void CardsMenuState::on_mouse_middle_button_down( const nom::Event& ev )
{
  if ( this->game->hand[0].push_back ( this->selectedCard ) )
  {
    this->game->card_place.Play();
  }
}

void CardsMenuState::on_mouse_right_button_down( const nom::Event& ev )
{
  if ( this->game->hand[0].push_back ( this->selectedCard ) )
    this->game->card_place.Play();
}

void CardsMenuState::on_mouse_wheel( const nom::Event& ev )
{
  // Do not check mouse wheel state unless it is a valid event; we receive
  // invalid data here if we do not check for this.
  if( ev.type != SDL_MOUSEWHEEL ) return;

  if ( this->game->cursor.state() == 0 )
  {
    if ( ev.wheel.x > 0 )
    {
      this->moveCursorLeft();
    }
    else if ( ev.wheel.x < 0 )
    {
      this->moveCursorRight();
    }
    else if ( ev.wheel.y > 0 )
    {
      this->moveCursorUp();
    }
    else if ( ev.wheel.y < 0 )
    {
      this->moveCursorDown();
    }
  }
  this->game->cursor_move.Play();
}

void CardsMenuState::on_update ( float delta_time )
{
  this->game->card.update();

  this->updateCursor();
  //this->game->menu_elements.Update();

  this->game->window.update();
}

void CardsMenuState::on_draw ( nom::IDrawable::RenderTarget& target )
{
  unsigned int y_offset = MENU_CARDS_FIELD_ORIGIN_Y; // card text, helper elements, card numbers

  this->game->background.draw ( target );

  // FIXME / This is a lazy patch until I get around to fixing this :-)
  this->game->card.face ( true ); // Turn drawing of faces down on

  // static player2 hand background
  for ( nom::int32 idx = 0; idx < MAX_PLAYER_HAND; idx++ )
  {
    this->player2_pos = Point2i (
                                  PLAYER2_ORIGIN_X,
                                  PLAYER2_ORIGIN_Y +
                                  ( CARD_HEIGHT / 2 ) * idx
                                );

    this->game->card.reposition ( this->player2_pos );
    this->game->card.draw ( target );
  }

  // FIXME / This is a lazy patch until I get around to fixing this :-)
  this->game->card.face ( false ); // Turn drawing of faces down back off

  // Active player's card selection(s)
  for ( nom::uint32 idx = 0; idx < this->game->hand[0].size(); idx++ )
  {
    this->player1_pos = Point2i (
                                  PLAYER1_ORIGIN_X,
                                  PLAYER1_ORIGIN_Y +
                                  ( CARD_HEIGHT / 2 ) * idx
                              );

    this->game->card.reposition ( this->player1_pos );
    this->game->card.setViewCard ( this->game->hand[0].cards.at ( idx ) );
    this->game->card.draw ( target );
  }
  /*
  this->menu_box.draw ( target );

  for ( nom::uint32 i = current_index; i < total_pages + current_index + 1; i++ ) // padded + 1 since page starts at zero, not one
  {
    // Draw the top-left box title
    this->title_text.set_text ( "CARDS" );
    this->title_text.set_position ( nom::Point2i(MENU_CARDS_TITLE_ORIGIN_X, MENU_CARDS_TITLE_ORIGIN_Y) );
    this->title_text.draw ( target );

    // Draw page number if we have more than one page to display
    if ( total_pages > 0 )
    {
      this->title_text.set_text ( "P. " + std::to_string ( current_index / per_page + 1 ) ); // padded + 1 since page starts at zero, not one
      this->title_text.set_position ( nom::Point2i(MENU_CARDS_TITLE_PAGE_ORIGIN_X, MENU_CARDS_TITLE_PAGE_ORIGIN_Y) );
      this->title_text.draw ( target );
    }

    // Draw the top-right box title (number of cards)
    this->title_text.set_text ( "NUM." );
    this->title_text.set_position ( nom::Point2i(MENU_CARDS_TITLE_NUM_ORIGIN_X, MENU_CARDS_TITLE_NUM_ORIGIN_Y) );
    this->title_text.draw ( target );

    // Draw the card selection helper element
    this->game->menu_elements.set_position ( nom::Point2i( MENU_CARDS_HELPER_ORIGIN_X, y_offset ) );

    if ( this->game->hand[0].exists ( this->game->collection.cards[i] ) )
      this->game->menu_elements.set_frame ( INTERFACE_MENU_ELEMENT_USED );
    else
      this->game->menu_elements.set_frame ( INTERFACE_MENU_ELEMENT );

    this->game->menu_elements.update();
    this->game->menu_elements.draw ( target );

    // Draw the card's name onto our menu box
    // FIXME ( this->game->info_text_gray )
    if ( this->game->hand[0].exists ( this->game->collection.cards[i] ) )
    {
      this->card_text.set_text ( this->game->collection.cards[i].getName() );
      this->card_text.set_color ( GrayText );
      this->card_text.set_position ( nom::Point2i(MENU_CARDS_NAME_ORIGIN_X, y_offset) );
      this->card_text.draw ( target );
    }
    else
    {
      this->card_text.set_text ( this->game->collection.cards[i].getName() );
      this->card_text.set_color ( nom::Color4i::White );
      this->card_text.set_position ( nom::Point2i(MENU_CARDS_NAME_ORIGIN_X, y_offset) );
      this->card_text.draw ( target );
    }

    // Draw the number of cards in player's possession
    // TODO: Stub
    if ( this->game->hand[0].exists ( this->game->collection.cards[i] ) )
    {
      this->card_text.set_text ( "0" );
      this->card_text.set_color ( GrayText );
      this->card_text.set_position ( nom::Point2i(MENU_CARDS_NUM_ORIGIN_X, y_offset) );
      this->card_text.draw ( target );
    }
    else
    {
      this->card_text.set_text ( "1" );
      this->card_text.set_color ( nom::Color4i::White );
      this->card_text.set_position ( nom::Point2i(MENU_CARDS_NUM_ORIGIN_X, y_offset) );
      this->card_text.draw ( target );
    }

    // Lastly, check to see which page indicators we need to draw
    if ( current_index >= per_page )
    {
      this->game->menu_elements.set_frame ( INTERFACE_MENU_ELEMENT_PAGE_LEFT );
      this->game->menu_elements.set_position ( nom::Point2i( MENU_CARDS_PAGE_LEFT_ORIGIN_X, MENU_CARDS_PAGE_LEFT_ORIGIN_Y ) );
      this->game->menu_elements.update();
      this->game->menu_elements.draw ( target );
    }

    if ( current_index / per_page < total_pages - 1 ) // calculate current page minus padding of one
    {
      this->game->menu_elements.set_frame ( INTERFACE_MENU_ELEMENT_PAGE_RIGHT );
      this->game->menu_elements.set_position ( nom::Point2i( MENU_CARDS_PAGE_RIGHT_ORIGIN_X, MENU_CARDS_PAGE_RIGHT_ORIGIN_Y ) );
      this->game->menu_elements.update();
      this->game->menu_elements.draw ( target );
    }

    // Move on to the next card in stack to draw
    // We calculate height after setting the text buffer for each card name
    y_offset += this->info_text_height;
  }
  */

  this->drawCursor ( target );

  this->game->card.setViewCard ( this->selectedCard );
  this->game->card.reposition ( this->card_pos );
  this->game->card.draw ( target );
}

void CardsMenuState::updateCursor ( void )
{
  unsigned int pos = 0;

  if ( this->game->cursor.state() == 0 )
  {
    this->game->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );

    pos = this->getCursorPos() + this->current_index;
    this->selectedCard = this->game->collection.cards[pos];

    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << "\npos: " << pos << "\n";
      std::cout << "\nselectedCard: " << this->game->collection.cards[pos].getName() << "\n";
    #endif
  }

  this->game->cursor.update();
}

void CardsMenuState::drawCursor ( nom::IDrawable::RenderTarget& target )
{
  this->game->cursor.draw ( target );
}

// Helper method for obtaining card hand index position based off given origin
// coords definitions, creating us an ID map, initialized early on within the
// encapsulating class
//
// cursor_coords_map
//   [ index, y coordinate value ]
//
unsigned int CardsMenuState::getCursorPos ( void )
{
  unsigned int pos = 0;
  unsigned int idx = 0;

  for ( idx = 0; idx < per_page; idx++ )
  {
    if ( this->game->cursor.y() <= std::get<0>(cursor_coords_map[idx]) )
      return std::get<1>(cursor_coords_map[idx]);
    else // catch all safety switch
    // assume we are at the last position in the index when all else fails
      pos = per_page;
  }

  return pos;
}

// Helper method for paging menu backwards
 // TODO: rename method call
void CardsMenuState::moveCursorLeft ( void )
{
  if ( this->game->cursor.state() == 0 )
  {
    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << current_index << " " << per_page << "\n";
    #endif

    if ( current_index > 0 )
    {
      current_index -= per_page;
      this->game->cursor_move.Play();
    }
  }
}

// Helper method for paging menu forwards
// TODO: rename method call
void CardsMenuState::moveCursorRight ( void )
{
  if ( this->game->cursor.state() == 0 )
  {
    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << current_index << " " << per_page << "\n";
    #endif

    if ( current_index < Card::CARDS_COLLECTION - per_page )
    {
      current_index += per_page;
      this->game->cursor_move.Play();
    }
  }
}

void CardsMenuState::moveCursorUp ( void )
{
  unsigned int pos = 0;

  if ( this->game->cursor.state() == 0 )
  {
    if ( this->game->cursor.y() > MENU_CARDS_CURSOR_ORIGIN_Y )
    {
      this->game->cursor.move ( 0, -( this->info_text_height ) );

      // FIXME: onMouseWheel breaks without the below support code:
      pos = this->current_index + this->getCursorPos();
      this->selectedCard = this->game->collection.cards[pos];

      this->game->cursor_move.Play();

      #ifdef DEBUG_CARDS_MENU_CURSOR
        std::cout << "\npos: " << pos << "\n";
        std::cout << "\nselectedCard: " << this->game->collection.getCards (pos).getName() << "\n";
      #endif
    }
  }
}

void CardsMenuState::moveCursorDown ( void )
{
  unsigned int pos = 0;

  if ( this->game->cursor.state() == 0 )
  {
    if ( this->game->cursor.y() < PICK_CARDS_MENU_HEIGHT )
    {
      this->game->cursor.move ( 0, this->info_text_height );

      // FIXME: onMouseWheel breaks without the below support code:
      pos = current_index + this->getCursorPos();
      this->selectedCard = this->game->collection.cards[pos];

      this->game->cursor_move.Play();

      #ifdef DEBUG_CARDS_MENU_CURSOR
        std::cout << "\npos: " << pos << "\n";
        std::cout << "\nselectedCard: " << this->game->collection.getCards (pos).getName() << "\n";
      #endif
    }
  }
}

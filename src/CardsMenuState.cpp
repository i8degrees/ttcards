/******************************************************************************
    CardsMenuState.cpp

  Cards Menu state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "CardsMenuState.h"

CardsMenu::CardsMenu ( Gfx *engine )
{
  unsigned int pid = 0; // temp var for for loop iteration

  #ifdef DEBUG_CARDS_MENU_OBJ
    std::cout << "CardsMenu::CardsMenu (): Hello, world!" << "\n" << std::endl;
  #endif

  logger = logDebug.Read( "./data/offsets.val" );

  this->engine = engine;
  this->collection.Load ( CARDS_DB );

  // Borrowed from Player class; this is perhaps a hack-(ish) workaround
  for ( pid = 0; pid < this->collection.cards.size(); pid++ )
    this->collection.cards[pid].setPlayerID ( PLAYER1_ID ); // 1

  this->hand.reset ();

  this->msgbox[0].setColor ( 41, 41, 41 ); // top1
  this->msgbox[1].setColor ( 133, 133, 133 ); // top2

  this->msgbox[2].setColor ( 41, 41, 41 ); // left1
  this->msgbox[3].setColor ( 133, 133, 133 ); // left2

  this->msgbox[4].setColor ( 57, 57, 57 ); // bottom1
  this->msgbox[5].setColor ( 57, 57, 57 ); // right1

  this->menu_box.setBackground ( &linear );
  //this->menu_box.disable ( true );

  for ( unsigned int i = 0; i < 6; i++ )
    this->menu_box.setBorder ( msgbox[i] );

  this->per_page = 11; // number of cards to display per menu page
  this->total_pages = this->collection.cards.size() / per_page;
  this->current_index = 0; // current card position

  this->selectedCard = this->collection.cards.front();

  this->Load();
}

CardsMenu::~CardsMenu ( void )
{
  unsigned int pid = 0; // temp var for for loop iteration

  #ifdef DEBUG_CARDS_MENU_OBJ
    std::cout << "CardsMenu::~CardsMenu (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  // Borrowed from Player class
  for ( pid = 0; pid < this->collection.cards.size(); pid++ )
    this->collection.cards[pid].setPlayerID ( NOPLAYER_ID ); // 0

  this->selectedCard = 0;
  this->hand.reset ();

  if ( this->engine )
    this->engine = NULL;

  if ( this->background )
  {
    SDL_FreeSurface ( this->background );
    this->background = NULL;
  }
}

void CardsMenu::Load ( void )
{
  unsigned int idx = 0; // cursor_coords_map

  this->background = this->engine->LoadImage ( BOARD_BACKGROUND );

  this->info_text.Load ( INFO_FONTFACE, GColor ( 110, 144, 190 ), 16, 16 );
  this->info_small_text.Load ( INFO_SMALL_FONTFACE, GColor ( 110, 144, 190 ), 16, 16 );

  // Initialize card name text so that we can obtain height info early on
  this->info_text.setTextBuffer ( this->collection->cards[0].getName() );
  this->info_text_height = this->info_text.getTextHeight();

  this->menu_element = Sprite ( MENU_ELEMENT_WIDTH, MENU_ELEMENT_HEIGHT );
  this->menu_element.LoadImage ( MENU_ELEMENTS, GColor ( 0, 0, 0 ) );

  this->cursor = Sprite ( CURSOR_WIDTH, CURSOR_HEIGHT );
  this->cursor.LoadImage ( INTERFACE_CURSOR, GColor ( 0, 0, 0 ) );
  this->cursor.SetSheetDimensions ( 78, 16, 0, 0 );
  this->cursor.SetSheetID ( INTERFACE_CURSOR_RIGHT );
  this->cursor.SetXY ( MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y );
  this->cursor.setState ( 0 ); // default state for navigating card menu

  // We cannot map std::pair<0, 0>, so we are "missing" the first element here,
  // which we do account for within the card tracking / positioning code
  for ( idx = 0; idx < per_page; idx++ )
  {
    this->cursor_coords_map[idx] = std::make_pair ( MENU_CARDS_CURSOR_ORIGIN_Y + ( this->info_text_height * idx ), idx );

    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << "\nidx:" << std::get<1>(this->cursor_coords_map[idx]) << " " << "y:" << std::get<0>(this->cursor_coords_map[idx]) << "\n";
    #endif
  }



}

void CardsMenu::Pause ( void )
{
  std::cout << "\n" << "CardsMenu state Paused" << "\n";
}

void CardsMenu::Resume ( void )
{
  std::cout << "\n" << "CardsMenu state Resumed" << "\n";
}

void CardsMenu::HandleInput ( void )
{
  SDLInput::Input();
}

void CardsMenu::onExit ( void )
{
  this->engine->Quit();
}

void CardsMenu::onKeyDown ( SDLKey key, SDLMod mod )
{
  switch ( key )
  {
    case SDLK_ESCAPE:
    case SDLK_q: this->engine->Quit(); break;
    // Reset / New Game State
    case SDLK_r: if ( mod == KMOD_LSHIFT ) reloadDebugFile(); else this->engine->PopStateThenChangeState ( std::unique_ptr<TTcards>( new TTcards ( this->engine ) ) ); break;
     // Pause State
    //case SDLK_p: this->engine->PopState (); break;

    case SDLK_LEFT: this->moveCursorLeft(); break;
    case SDLK_RIGHT: this->moveCursorRight(); break;
    case SDLK_UP: this->moveCursorUp(); break;
    case SDLK_DOWN: this->moveCursorDown(); break;

    case SDLK_d: /*if ( mod == KMOD_LMETA )*/ this->hand.removeCard ( this->selectedCard ); break;
    case SDLK_SPACE: this->hand.addCard ( this->selectedCard ); break;
    default: break;
  }
}

void CardsMenu::onMouseLeftButtonDown ( unsigned int x, unsigned int y )
{
  // TODO
}

void CardsMenu::onMouseRightButtonDown ( unsigned int x, unsigned int y )
{
  // TODO
}

void CardsMenu::onMouseWheel ( bool up, bool down )
{
  if ( this->cursor.getState() == 0 )
  {
    if ( up )
      this->moveCursorUp();
    else if ( down )
      this->moveCursorDown();
  }
}

void CardsMenu::Update ( void )
{
  this->updateCursor();

  this->engine->UpdateScreen ();
}

void CardsMenu::Draw ( void )
{
  unsigned int y_offset = MENU_CARDS_FIELD_ORIGIN_Y; // card text, helper elements, card numbers

  unsigned int hand_index = 0; // "dummy" card index var

  engine->DrawSurface ( this->background, 0, 0 ); // draw static board background

  // static player2 hand background
  for ( hand_index = 0; hand_index < MAX_PLAYER_HAND; hand_index++ ) // TODO: std::get<1>(player_coords)
    this->card.drawFaceDown ( this->engine, PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index );

  // Active player's card selection(s)
  for ( hand_index = 0; hand_index < this->hand.getCount(); hand_index++ ) // TODO: std::get<1>(player_coords)
  {
    if ( this->hand.isValid ( this->hand.cards.at ( hand_index) ) == true )
      this->card.DrawCard ( this->engine, this->hand.cards.at ( hand_index ), PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index );
  }

  this->menu_box.Draw ( this->engine->screen, PICK_CARDS_MENU_ORIGIN_X, PICK_CARDS_MENU_ORIGIN_Y, PICK_CARDS_MENU_WIDTH, PICK_CARDS_MENU_HEIGHT );

  for ( int i = current_index; i < total_pages + current_index + 1; i++ ) // padded + 1 since page starts at zero, not one
  {
    // Draw the top-left box title
    this->info_small_text.setTextBuffer ( "CARDS" );
    this->info_small_text.Draw ( this->engine, MENU_CARDS_TITLE_ORIGIN_X, MENU_CARDS_TITLE_ORIGIN_Y );

    // Draw page number if we have more than one page to display
    if ( total_pages > 0 )
    {
      this->info_small_text.setTextBuffer ( "P. " + std::to_string ( current_index / per_page + 1 ) ); // padded + 1 since page starts at zero, not one
      this->info_small_text.Draw ( this->engine, MENU_CARDS_TITLE_PAGE_ORIGIN_X, MENU_CARDS_TITLE_PAGE_ORIGIN_Y );
    }

    // Draw the top-right box title (number of cards)
    this->info_small_text.setTextBuffer ( "NUM." );
    this->info_small_text.Draw ( this->engine, MENU_CARDS_TITLE_NUM_ORIGIN_X, MENU_CARDS_TITLE_NUM_ORIGIN_Y );

    // Draw the card selection helper element
    this->menu_element.SetXY ( MENU_CARDS_HELPER_ORIGIN_X, y_offset );
    this->menu_element.SetSheetID ( INTERFACE_MENU_ELEMENT );
    this->menu_element.Draw ( this->engine );

    // Draw the card's name onto our menu box
    this->info_text.setTextBuffer ( this->collection->cards[i].getName() );
    this->info_text.Draw ( this->engine, MENU_CARDS_NAME_ORIGIN_X, y_offset );

    // Draw the number of cards in player's possession
    this->info_text.setTextBuffer ( "1" );
    this->info_text.Draw ( this->engine, MENU_CARDS_NUM_ORIGIN_X, y_offset );

    // Lastly, check to see which page indicators we need to draw
    if ( current_index >= per_page )
    {
      this->menu_element.SetSheetID ( INTERFACE_MENU_ELEMENT_PAGE_LEFT );
      this->menu_element.SetXY ( MENU_CARDS_PAGE_LEFT_ORIGIN_X, MENU_CARDS_PAGE_LEFT_ORIGIN_Y );
      this->menu_element.Draw ( this->engine );
    }

    if ( current_index / per_page < total_pages - 1 ) // calculate current page minus padding of one
    {
      this->menu_element.SetSheetID ( INTERFACE_MENU_ELEMENT_PAGE_RIGHT );
      this->menu_element.SetXY ( MENU_CARDS_PAGE_RIGHT_ORIGIN_X, MENU_CARDS_PAGE_RIGHT_ORIGIN_Y );
      this->menu_element.Draw ( this->engine );
    }

    // Move on to the next card in stack to draw
     // We calculate height after setting the text buffer for each card name
    y_offset += this->info_text_height;
  }

  this->drawCursor();

  this->card.DrawCard ( this->engine, this->selectedCard, BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ), BOARD_ORIGIN_Y + ( ( CARD_HEIGHT / 2 ) + CARD_HEIGHT * 1 ) - 8 );
}

// Helper method for debug logger
void CardsMenu::reloadDebugFile ( void )
{
  logger.clear();

  logger = logDebug.Read ( "./data/offsets.val" );
}

void CardsMenu::updateCursor ( void )
{
  this->cursor.SetSheetID ( INTERFACE_CURSOR_RIGHT );
}

void CardsMenu::drawCursor ( void )
{
  this->cursor.Draw ( this->engine );
}

// Helper method for obtaining card hand index position based off given origin
// coords definitions, creating us an ID map, initialized early on within the
// encapsulating class
//
// cursor_coords_map
//   [ index, y coordinate value ]
//
unsigned int CardsMenu::getCursorPos ( void )
{
  unsigned int pos = 0;
  unsigned int idx = 0;

  for ( idx = 0; idx < per_page; idx++ )
  {
    if ( this->cursor.GetY() <= std::get<0>(cursor_coords_map[idx]) )
      return std::get<1>(cursor_coords_map[idx]);
    else // catch all safety switch
    // assume we are at the last position in the index when all else fails
      pos = per_page;
  }

  return pos;
}

// Helper method for paging menu backwards
void CardsMenu::moveCursorLeft ( void ) // TODO: rename method call
{
  unsigned int pos = 0;

  if ( this->cursor.getState() == 0 )
  {
    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << current_index << " " << per_page << "\n";
    #endif

    if ( current_index > 0 )
      current_index -= per_page;

    pos = this->getCursorPos();
    pos = this->current_index + pos;
    this->selectedCard = this->collection.cards[pos];

    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << "\npos: " << pos << "\n";
      std::cout << "\nselectedCard: " << this->collection.cards[pos].getName() << "\n";
    #endif
  }
}

// Helper method for paging menu forwards
void CardsMenu::moveCursorRight ( void ) // TODO: rename method call
{
  unsigned int pos = 0;

  if ( this->cursor.getState() == 0 )
  {
    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << current_index << " " << per_page << "\n";
    #endif

    if ( current_index < MAX_COLLECTION - per_page )
        current_index += per_page;

    pos = this->getCursorPos();
    pos = this->current_index + pos;
    this->selectedCard = this->collection.cards[pos];

    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << "\npos: " << pos << "\n";
      std::cout << "\nselectedCard: " << this->collection.cards[pos].getName() << "\n";
    #endif
  }
}

void CardsMenu::moveCursorUp ( void )
{
  unsigned int pos = 0;

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.GetY() > MENU_CARDS_CURSOR_ORIGIN_Y )
    {
      this->cursor.UpdateXY ( 0, -( this->info_text_height ) );

      pos = this->getCursorPos();
      pos = this->current_index + pos;
      this->selectedCard = this->collection.cards[pos];

      #ifdef DEBUG_CARDS_MENU_CURSOR
        std::cout << "\npos: " << pos << "\n";
        std::cout << "\nselectedCard: " << this->collection.getCards (pos).getName() << "\n";
      #endif
    }
  }
}

void CardsMenu::moveCursorDown ( void )
{
  unsigned int pos = 0;

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.GetY() < PICK_CARDS_MENU_HEIGHT )
    {
      this->cursor.UpdateXY ( 0, this->info_text_height );

      pos = this->getCursorPos();
      pos = current_index + pos;
      this->selectedCard = this->collection.cards[pos];

      #ifdef DEBUG_CARDS_MENU_CURSOR
        std::cout << "\npos: " << pos << "\n";
        std::cout << "\nselectedCard: " << this->collection.getCards (pos).getName() << "\n";
      #endif
    }
  }
}

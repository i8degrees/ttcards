/******************************************************************************
    CardsMenuState.cpp

  Cards Menu state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "CardsMenuState.h"

CardsMenu::CardsMenu ( Gfx *engine, Collection *cards_db )
{
  #ifdef DEBUG_CARDS_MENU_OBJ
    std::cout << "CardsMenu::CardsMenu (): Hello, world!" << "\n" << std::endl;
  #endif

  this->engine = engine;
  this->collection = cards_db;

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

  this->total_pages = this->collection->cards.size();
  this->per_page = 11;
  this->current_index = 0;
  this->current_page = 0;


  logger = logDebug.Read( "./data/offsets.val" );

  this->Load();
}

CardsMenu::~CardsMenu ( void )
{
  #ifdef DEBUG_CARDS_MENU_OBJ
    std::cout << "CardsMenu::~CardsMenu (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->engine )
    this->engine = NULL;

  if ( this->collection )
    this->collection = NULL;
}

void CardsMenu::Load ( void )
{
  this->info_text.Load ( INFO_FONTFACE, GColor ( 110, 144, 190 ), 16, 16 );
  this->info_small_text.Load ( INFO_SMALL_FONTFACE, GColor ( 110, 144, 190 ), 16, 16 );

  this->menu_element = Sprite ( MENU_ELEMENT_WIDTH, MENU_ELEMENT_HEIGHT );
  this->menu_element.LoadImage ( MENU_ELEMENTS, GColor ( 0, 0, 0 ) );

  this->cursor = Sprite ( CURSOR_WIDTH, CURSOR_HEIGHT );
  this->cursor.LoadImage ( INTERFACE_CURSOR, GColor ( 0, 0, 0 ) );
  this->cursor.SetSheetDimensions ( 78, 16, 0, 0 );
  this->cursor.SetSheetID ( INTERFACE_CURSOR_RIGHT );
  this->cursor.SetXY ( MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y );
  this->cursor.setState ( 2 );

  // We cannot map std::pair<0, 0>, so we are "missing" the first element here
  //for ( int idx = 0; idx < MAX_PLAYER_HAND; idx++ )
    //this->cursor_coords_map[idx] = std::make_pair ( std::get<1>(player_cursor_coords[0]) + ( CARD_HEIGHT / 2 ) * idx, idx );

  // Initialize card name text so that we can obtain height info ASAP
  this->info_text.setTextBuffer ( this->collection->cards[0].getName() );
  this->card_name_height = this->info_text.getTextHeight();
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

    default: break;
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

  this->drawCursor();
  this->menu_box.Draw ( this->engine->screen, PICK_CARDS_MENU_ORIGIN_X, PICK_CARDS_MENU_ORIGIN_Y, PICK_CARDS_MENU_WIDTH, PICK_CARDS_MENU_HEIGHT );

  for ( int i = current_index; i < total_pages / per_page + current_index; i++ )
  {
    // Draw the top-left box title
    this->info_small_text.setTextBuffer ( "CARDS" );
    this->info_small_text.Draw ( this->engine, MENU_CARDS_TITLE_ORIGIN_X, MENU_CARDS_TITLE_ORIGIN_Y );

    // Draw page number if we have more than one page to display
    if ( current_index > 11 )
    {
      this->info_small_text.setTextBuffer ( "P. " + std::to_string ( current_index ) );
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
    if ( current_index > 11 )
    {
      this->menu_element.SetSheetID ( INTERFACE_MENU_ELEMENT_PAGE_LEFT );
      this->menu_element.SetXY ( MENU_CARDS_PAGE_LEFT_ORIGIN_X, MENU_CARDS_PAGE_LEFT_ORIGIN_Y );
      this->menu_element.Draw ( this->engine );
    }

    if ( current_index < total_pages )
    {
      this->menu_element.SetSheetID ( INTERFACE_MENU_ELEMENT_PAGE_RIGHT );
      this->menu_element.SetXY ( MENU_CARDS_PAGE_RIGHT_ORIGIN_X, MENU_CARDS_PAGE_RIGHT_ORIGIN_Y );
      this->menu_element.Draw ( this->engine );
    }

    // Move on to the next card in stack to draw
     // We calculate height after setting the text buffer for each card name
    y_offset += this->card_name_height;
  }
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

// Helper method for obtaining card hand index position based off coords ID map
unsigned int CardsMenu::getCursorPos ( void )
{
  unsigned int pos = 0;
/*
  if ( this->cursor.GetY() <= std::get<0>(cursor_coords_map[0]) )
    pos = 0;
  else if ( this->cursor.GetY() <= std::get<0>(cursor_coords_map[1]) )
    pos = std::get<1>(cursor_coords_map[1]);
  else if ( this->cursor.GetY() <= std::get<0>(cursor_coords_map[2]) )
    pos = std::get<1>(cursor_coords_map[2]);
  else if ( this->cursor.GetY() <= std::get<0>(cursor_coords_map[3]) )
    pos = std::get<1>(cursor_coords_map[3]);
  else
    pos = 4;
*/
  return pos;
}

void CardsMenu::moveCursorLeft ( void )
{
  if ( this->cursor.getState() == 2 )
  {
    std::cout << current_index << " " << per_page << "\n";

    if ( current_index > 11 )
      if ( ( current_index -= per_page ) >= MAX_COLLECTION )
        current_index -= per_page;

    std::cout << current_index << " " << per_page << "\n";
  }
}

void CardsMenu::moveCursorRight ( void )
{
  if ( this->cursor.getState() == 2 )
  {
    std::cout << current_index << " " << per_page << "\n";
    if ( current_index < MAX_COLLECTION )
      if ( ( current_index += per_page ) <= MAX_COLLECTION )
        current_index += per_page;
    std::cout << current_index << " " << per_page << "\n";
  }
}

void CardsMenu::moveCursorUp ( void )
{
  //unsigned int pos = 0;

  if ( this->cursor.getState() == 2 )
  {
    if ( this->cursor.GetY() > PICK_CARDS_MENU_ORIGIN_Y )
    {
      std::cout << "\ncard_name_height: " << this->card_name_height << "\n\n";
      this->cursor.UpdateXY ( 0, -( this->card_name_height ) );
      std::cout << "\ncursorY: " << this->cursor.GetY() << "\n\n";
    }
  }
}

void CardsMenu::moveCursorDown ( void )
{
  //unsigned int pos = 0;

  if ( this->cursor.getState() == 2 )
  {
    if ( this->cursor.GetY() < PICK_CARDS_MENU_ORIGIN_Y + PICK_CARDS_MENU_HEIGHT )
    {
      std::cout << "\ncard_name_height: " << this->card_name_height << "\n\n";
      this->cursor.UpdateXY ( 0, this->card_name_height );
      std::cout << "\ncursorY: " << this->cursor.GetY() << "\n\n";
    }
  }
}

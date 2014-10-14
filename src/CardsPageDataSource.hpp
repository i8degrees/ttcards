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
#ifndef TTCARDS_CARDS_PAGE_DATA_SOURCE_HPP
#define TTCARDS_CARDS_PAGE_DATA_SOURCE_HPP

#include <Rocket/Core.h>
#include <Rocket/Controls/DataFormatter.h>
#include <Rocket/Controls/DataSource.h>

#include "nomlib/config.hpp"

#include "Card.hpp"
#include "CardCollection.hpp"

/// \brief Mock model of the CardsMenu player's hand selection widget in TTcards
///
/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Controls/DataGrid
/// \see http://librocket.com/wiki/documentation/tutorials/Datagrid
///
/// \remarks This interface should **not** be used outside of unit tests; for
/// starters, assertion macros are disabled for running under this particular
/// testing environment.
///
/// \note This interface only supports using one table per data source.
class CardsPageDataSource: public Rocket::Controls::DataSource
{
  public:
    CardsPageDataSource(  const std::string& source,
                          const std::string& table_name = "cards" );

    /// \brief Destructor.
    ///
    /// \remarks libRocket handles the deletion of the DataSource object
    /// instance for us.
    virtual ~CardsPageDataSource();

    /// \note Implements Rocket::Controls::DataSource::GetRow.
    virtual void GetRow( Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns );

    /// \brief Get the total number of rows for a page.
    ///
    /// \returns An integer value of the number of rows per the current page.
    ///
    /// \note The number of rows returned influence the minimal size (height)
    /// of the the datagrid, even when RCSS properties such as 'max-height',
    /// 'height', etc. are being applied.
    ///
    /// \note Implements Rocket::Controls::DataSource::GetNumRows.
    ///
    /// \see ::num_rows, ::per_page.
    virtual int GetNumRows( const Rocket::Core::String& table );

    const std::string& table_name() const;

    /// \brief Set the table name to use on the data source.
    void set_table_name( const std::string& name );

    /// \brief Get the total number of cards (rows) in storage.
    nom::size_type num_rows() const;

    /// \brief Fetches the contents of a row of a table within the data source.
    ///
    /// \param row        The container object to use use in row results output.
    /// \param table      The name of the table to grab results for.
    /// \param row_index  The element's index position.
    /// \param columns    The columns within the row to grab results for.
    ///
    /// \remarks Convenience alias for ::GetRow.
    // void row( nom::StringList& row, const std::string& table, int row_index, const StringList& columns );
    void row( nom::StringList& row, int row_index, const nom::StringList& columns );

    int insert_card( int pos, const Card& card );

    /// \param pos The beginning element position to perform the insertion.
    int insert_cards( int pos, const std::vector<Card>& cards );

    int append_card( const Card& card );

    int append_cards( const std::vector<Card>& cards );

    /// \brief Test whether the cards storage is empty.
    ///
    /// \returns Boolean TRUE if the cards storage is empty; i.e.:
    /// ```::num_rows() == 0```.
    ///   Boolean FALSE if the cards storage is not empty; i.e.:
    /// ```::num_rows() != 0```.
    bool empty() const;

    /// \brief Destroy all the cards in storage.
    void erase_cards();

    /// \brief Erase a card from the storage container.
    ///
    /// \param pos The element position to erase, starting from the beginning
    /// of the cards storage container; zero-based index.
    ///
    /// \returns The resulting size of the cards storage after removal on
    /// success, or nom::npos on failure, i.e.: invalid position.
    int erase_card( int pos );

    /// \brief Erase a range of cards from the storage container.
    ///
    /// \param begin_pos The starting element position, from the beginning of
    /// the cards storage container; zero-based index.
    ///
    /// \param end_pos The ending element position, from the beginning of
    /// the cards storage container; zero-based index.
    ///
    /// \returns The resulting size of the cards storage after removal on
    /// success, or nom::npos on failure, i.e.: invalid position(s).
    int erase_cards( int begin_pos, int end_pos );

    /// \brief Rudimentary debugging aid.
    std::string dump();

    /// \brief Lookup a card by name.
    ///
    /// \param name The card's name.
    const Card& lookup_by_name( const std::string& name ) const;

    /// \brief Lookup a card by ID.
    ///
    /// \param id The card's identifier.
    const Card& lookup_by_id( int id ) const;

    int per_page() const;
    int total_pages() const;
    int page() const;

    void set_per_page( int pg );

    /// \brief Set the current page in the table.
    ///
    /// \remarks This method call is potentially expensive, as it calls for a
    /// refresh on all row indexes within the table.
    void set_page( int page );

    /// \brief Get the row index (position) of a page.
    ///
    /// \param index  The row index (of the internal representation) to map.
    /// \param pg     The page number to use to map.
    ///
    /// \returns A non-negative row index that is accurate for the currently
    /// rendered page, or negative one (-1) on failure, such as an invalid page
    /// or row index.
    ///
    /// \remarks This method is useful for mapping game objects, such as a game
    /// cursor controlled by the user's keyboard.
    int map_page_row(int index, int pg) const;

    int map_row(int page_row) const;

    /// \brief Recalculate the total number of pages.
    ///
    /// \remarks The total number of pages is calculated by dividing the total
    /// number of rows (internally stored) by the number of cards set for a
    /// single page.
    ///
    /// \note This method should ordinarily not need to be called.
    ///
    /// \see ::num_rows, ::set_per_page
    void update();

  private:
    void set_total_pages( int num_pages );

    /// \brief The name of the table of the data source.
    std::string table_name_;

    /// \brief The internal storage for the data source.
    std::vector<Card> db_;

    /// \brief The number of cards shown per page.
    int per_page_;

    /// \brief The total number of available pages.
    int total_pages_;

    /// \brief The current (shown) page.
    int page_;
};

/// \remarks This interface should **not** be used outside of unit tests; for
/// starters, assertion macros are disabled for running under this particular
/// testing environment.
class CardStatusFormatter: public Rocket::Controls::DataFormatter
{
  public:
    /// \brief Default constructor.
    ///
    /// \remarks The default column formatter used is 'card_status'.
    CardStatusFormatter();

    virtual ~CardStatusFormatter();

    /// \brief Construct an object using a custom column formatter name.
    ///
    /// \param formatter The datagrid column formatter attribute name.
    CardStatusFormatter( const std::string& formatter );

    void FormatData(  Rocket::Core::String& formatted_data,
                      const Rocket::Core::StringList& raw_data );
};

/// \remarks This interface should **not** be used outside of unit tests; for
/// starters, assertion macros are disabled for running under this particular
/// testing environment.
class CardNameFormatter: public Rocket::Controls::DataFormatter
{
  public:
    /// \brief Default constructor.
    ///
    /// \remarks The default column formatter used is 'card_name'.
    CardNameFormatter();

    virtual ~CardNameFormatter();

    /// \brief Construct an object using a custom column formatter name.
    ///
    /// \param formatter The datagrid column formatter attribute name.
    CardNameFormatter( const std::string& formatter );

    void FormatData(  Rocket::Core::String& formatted_data,
                      const Rocket::Core::StringList& raw_data );
};

#endif // include guard defined

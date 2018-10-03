#include "addressbook.hpp"

namespace addressbook {

	addressbook::addressbook( account_name self ) : contract( self ) {}

	uint64_t addressbook::person::primary_key() const {
		return (key);
	}

	void	addressbook::load(
			account_name user,
			std::string first_name, 
			std::string last_name, 
			std::string street, 
			std::string city, 
			std::string state
		)
	{
		require_auth( user );
		address_index addresses( _self, _self );
		auto iterator = addresses.find( user );

		eosio_assert( addresses.find( user ) == addresses.end(), "user already exist" );
		addresses.emplace( user, [&]( auto& row ) {
			row.key = user;
			row.first_name = first_name;
			row.last_name = last_name;
			row.street = street;
			row.city = city;
			row.state = state;
		});
	}

	void 	addressbook::modify(
			account_name user,
			std::string first_name, 
			std::string last_name, 
			std::string street, 
			std::string city, 
			std::string state
		)
	{
		require_auth( user );
		address_index addresses( _self, _self );
		auto iterator = addresses.find( user );
		eosio_assert( addresses.find( user ) != addresses.end(), "user don't exist" );
		addresses.modify( iterator, user, [&]( auto& row ) {
			row.first_name = first_name;
			row.last_name = last_name;
			row.street = street;
			row.city = city;
			row.state = state;
		});
	}

	void 	addressbook::remove( account_name user ) {
		require_auth( user );
		address_index addresses( _self, _self );
		auto iterator = addresses.find( user );
		eosio_assert( iterator != addresses.end(), "Record does not exist" );
		addresses.erase( iterator );
	}
}

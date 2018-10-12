#include "addressbook.hpp"

namespace book {

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
		send_summary( user, "successfully load record to addressbook" );
		increment_counter( user, "loaded" );
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
		send_summary( user, "successfully modified record from addressbook" );
		increment_counter( user, "modified" );
	}

	void 	addressbook::remove( account_name user ) {
		require_auth( user );
		address_index addresses( _self, _self );
		auto iterator = addresses.find( user );
		eosio_assert( iterator != addresses.end(), "Record does not exist" );
		addresses.erase( iterator );
		send_summary( user, "successfully remove record in addressbook" );
		increment_counter( user, "removed" );
	}
	
	void 	addressbook::notify( account_name user, std::string msg ) {
		require_auth( get_self() );
		require_recipient( user );
	}

	void 	addressbook::send_summary( account_name user, std::string message ) {
		action(
			permission_level{ get_self(), N(active) },
			get_self(),
			N(notify),
			std::make_tuple( user, name{user}.to_string() + message )
		).send();
	}

	void 	addressbook::increment_counter(account_name user, std::string type) {
		action counter = action(
			permission_level{ get_self(), N(active) },
			N(abcounter),
			N(count),
			std::make_tuple( user, type)
		);
		counter.send();
	}
}


extern "C" {
using namespace book;

	void apply(uint64_t self, uint64_t code, uint64_t action) {
		addressbook _addressbook(self);
		if( code == self && action == N(load) ) {
			execute_action( &_addressbook, &addressbook::load );
		}
		else if ( code == self && action == N(modify) ) {
			execute_action( &_addressbook, &addressbook::modify);
		}
		else if( code == self && action == N(notify) ) {
			execute_action( &_addressbook, &addressbook::notify );
		}
		else if( code == self && action == N(erase) ) {
			execute_action( &_addressbook, &addressbook::remove );
		}
	}
};

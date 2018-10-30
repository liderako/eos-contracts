#include "addressbook.hpp"

namespace book {

	addressbook::addressbook(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds) : contract(receiver, code, ds) {}

	capi_name addressbook::person::primary_key() const {
		return (key.value);
	}

	void	addressbook::load(
			eosio::name user,
			std::string first_name, 
			std::string last_name, 
			std::string street, 
			std::string city, 
			std::string state
		)
	{
		require_auth( user );
		address_index addresses( _code, _code.value );
		auto iterator = addresses.find( user.value );

		eosio_assert( iterator == addresses.end(), "user already exist" );
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
			 eosio::name user,
			std::string first_name, 
			std::string last_name, 
			std::string street, 
			std::string city, 
			std::string state
		)
	{
		require_auth( user );
		address_index addresses( _code, _code.value );
		auto iterator = addresses.find( user.value );
		eosio_assert( iterator != addresses.end(), "user don't exist" );
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

	void 	addressbook::remove(  eosio::name user ) {
		require_auth( user );
		address_index addresses( _code, _code.value );
		auto iterator = addresses.find( user.value );
		eosio_assert( iterator != addresses.end(), "Record does not exist" );
		addresses.erase( iterator );
		send_summary( user, "successfully remove record in addressbook" );
		increment_counter( user, "removed" );
	}
	
	void 	addressbook::notify(  eosio::name user, std::string msg ) {
		require_auth( get_self() );
		require_recipient( user.value );
	}

	void 	addressbook::send_summary(  eosio::name user, std::string message ) {
		action(
			permission_level{ get_self(), name("active") },
			get_self(),
			name("notify"),
			std::make_tuple( user, name{user}.to_string() + message )
		).send();
	}

	void 	addressbook::increment_counter( eosio::name user, std::string type) {
		action counter = action(
			permission_level{ get_self(), eosio::name("active") },
			eosio::name("abcounter"),
			eosio::name("count"),
			std::make_tuple( user, type)
		);
		counter.send();
	}
}

extern "C" {
using namespace book;

	void apply(eosio::name self, eosio::name code, eosio::name action) {
		if( code == self && action == eosio::name("load") ) {
			execute_action( self, code, &addressbook::load );
		}
		else if ( code == self && action == eosio::name("modify") ) {
			execute_action( self, code, &addressbook::modify);
		}
		else if( code == self && action == eosio::name("notify") ) {
			execute_action( self, code, &addressbook::notify );
		}
		else if( code == self && action == eosio::name("remove") ) {
			execute_action( self, code, &addressbook::remove );
		}
	}
};

//set account permission addressbook active '{"threshold": 1,"keys": [{"key": "EOS8aTaugtHoTA5vMfgFM5heTRfMk2PuvE3o6JBaZrVVHNYqwKDi4","weight": 1}], "accounts": [{"permission":{"actor":"addressbook","permission":"eosio.code"},"weight":1}]}' -p addressbook@owner

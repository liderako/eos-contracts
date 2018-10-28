#include "abcounter.hpp"

namespace abcounter {
	abcounter::abcounter(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds) : contract(receiver, code, ds) {}

	void abcounter::count( eosio::name user, std::string type ) {
		require_auth( eosio::name("addressbook") );

		count_index counts( _code, _code.value );

		auto iterator = counts.find( user.value );
		if ( iterator == counts.end() ) {
			counts.emplace( eosio::name("addressbook"), [&]( auto& row ) {
				row.key = user;
				row.loaded = (type == "loaded") ? 1 : 0;
				row.modified = (type == "modified") ? 1 : 0;
				row.removed = (type == "removed") ? 1 : 0;
			});
		}
		else {
			counts.modify( iterator, eosio::name("addressbook"), [&]( auto& row ){
				(type == "loaded") ? row.loaded += 1 : 0;
				(type == "modified" ? row.modified += 1 : 0);
				(type == "removed" ? row.removed += 1 : 0);
			});
		}
	}

	uint64_t	abcounter::counter::primary_key() const {
		return (key.value);
	}
}
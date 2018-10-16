#include "abcounter.hpp"

namespace abcounter {
	abcounter::abcounter( account_name self ) : contract( self ) {}

	void abcounter::count( account_name user, std::string type ) {
		require_auth( N(addressbook) );

		count_index counts( _self, _self );
		auto iterator = counts.find( user );
		if ( iterator == counts.end() ) {
			counts.emplace( N(addressbook), [&]( auto& row ) {
				row.key = user;
				row.loaded = (type == "loaded") ? 1 : 0;
				row.modified = (type == "modified") ? 1 : 0;
				row.removed = (type == "removed") ? 1 : 0;
			});
		}
		else {
			counts.modify( iterator, N(addressbook), [&]( auto& row ){
				(type == "loaded") ? row.loaded += 1 : 0;
				(type == "modified" ? row.modified += 1 : 0);
				(type == "removed" ? row.removed += 1 : 0);
			});
		}
	}

	uint64_t	abcounter::counter::primary_key() const {
		return (key);
	}
}
#include "balance.hpp"

namespace balancebook {
//public
	balance::balance( account_name self ) :
		contract( self ),
		balance_of( _self, _self )
	{}

	void 		balance::deposit( const account_name sender, const asset& amount ) {
		require_auth( sender );
		assert_amount( amount );

		auto iterator = balance_of.find( sender );
		transfer( sender, _self, amount );
		if ( iterator == balance_of.end() ) {
			create_balance( sender, amount, iterator );
		}
		else {
			add_balance( sender, amount, iterator );
		}
	}

	void 		balance::withdraw( const account_name sender, const asset& amount ) {
		require_auth( sender );
		assert_amount( amount );

		auto iterator = balance_of.find( sender );
		if (iterator == balance_of.end()) {
			eosio_assert( false, "unknown user");
		}
		sub_balance( sender, amount, iterator );
		
		transfer( _self, sender, amount );

		if( iterator->is_empty() ) {
			delete_balance( iterator );
		}
	}

// private
	template<typename T>
	void	balance::create_balance( const account_name user, const asset& amount, T iterator) {
		balance_of.emplace( user, [&]( auto& row ) {
			row.owner = user;
			row.eos_balance = amount;
		});
	}
	
	template<typename T>
	void	balance::add_balance( const account_name user, const asset& amount, T iterator) {
		balance_of.modify( iterator, user, [&]( auto& row ) {
			row.eos_balance += amount;
		});
	}

	template<typename T>
	void	balance::sub_balance( const account_name user, const asset& amount, T iterator) {
		balance_of.modify( iterator, user, [&]( auto& row ) {
			eosio_assert( row.eos_balance >= amount, "insufficient balance" );
			row.eos_balance -= amount;
		});
	}

	template<typename T>
	void	balance::delete_balance( T iterator ) {
		balance_of.erase( iterator );
	}

	void 	balance::transfer( const account_name from, const account_name to, const asset& amount ) {
		action(
			permission_level{ from, N(active) },
			N(eosio.token), N(transfer),
			std::make_tuple( from, to, amount, std::string(""))
		).send();
	}

	uint64_t 	balance::t_balance::primary_key() const {
		return (owner);
	}

	void		balance::assert_amount(const asset& amount) {
		eosio_assert( amount.is_valid(), "invalid amount for action" );
		eosio_assert( amount.amount > 0, "must asset positive amount for action" );
	}

	bool 		balance::t_balance::is_empty() const {
		return ( eos_balance.amount == 0 );
	}
}

extern "C" {
using namespace balancebook;

	void apply(uint64_t self, uint64_t code, uint64_t action) {
		balance _balance(self);
		if( code == self && action == N(deposit) ) {
			execute_action( &_balance, &balance::deposit );
		}
		else if ( code == self && action == N(withdraw) ) {
			execute_action( &_balance, &balance::withdraw );
		}
	}
};

// cleos set account permission addressbook active '{"threshold": 1,"keys": [{"key": "","weight": 1}], "accounts": [{"permission":{"actor":"addressbook","permission":"eosio.code"},"weight":1}]}' -p addressbook@owner
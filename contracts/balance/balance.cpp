#include "balance.hpp"

namespace balancebook {
//public
	balance::balance( account_name self ) :
		contract( self ),
		balance_of( _self, _self )
	{}

	void 		balance::deposit( const account_name _sender, const asset& _quantity ) {
		auto data = unpack_action_data<balance::transfer_args>();

		if (data.from == _self || data.to != _self)
			return ;
		const account_name from = data.from;
		const asset amount = data.amount;

		require_auth( from );
		assert_amount( amount );

		auto iterator = balance_of.find( from );
		if ( iterator == balance_of.end() ) {
			create_balance( from, amount, iterator );
		}
		else {
			add_balance( from, amount, iterator );
		}
	}

	void 		balance::withdraw( const account_name from, const asset& amount ) {
		require_auth( from );
		assert_amount( amount );

		auto iterator = balance_of.find( from );
		if (iterator == balance_of.end()) {
			eosio_assert( false, "unknown user");
		}
		sub_balance( from, amount, iterator );
		
		transfer( _self, from, amount );

		if( iterator->is_empty() ) {
			delete_balance( iterator );
		}
	}

// private
	template<typename T>
	void	balance::create_balance( const account_name user, const asset& amount, T iterator) {
		balance_of.emplace( _self, [&]( auto& row ) {
			row.owner = user;
			row.eos_balance = amount;
		});
	}
	
	template<typename T>
	void	balance::add_balance( const account_name user, const asset& amount, T iterator) {
		balance_of.modify( iterator, _self, [&]( auto& row ) {
			row.eos_balance += amount;
		});
	}

	template<typename T>
	void	balance::sub_balance( const account_name user, const asset& amount, T iterator) {
		balance_of.modify( iterator, _self, [&]( auto& row ) {
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

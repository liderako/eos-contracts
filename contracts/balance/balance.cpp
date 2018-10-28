#include "balance.hpp"

namespace balancebook {
//public
	balance::balance(
			eosio::name receiver,
			eosio::name code,
			eosio::datastream<const char*> ds
		)
		: contract(receiver, code, ds),
		balance_of( _code, _code.value );
	{
		auto iterator = balance_of.find( _code.value );
		if ( iterator == balance_of.end() ) {
			create_balance( receiver, eosio::asset(0, eosio::symbol(eosio::symbol_code("EOS"), 4)));
		}
	}

	void 		balance::deposit( const eosio::name _sender, const eosio::asset& _quantity ) {
		auto data = eosio::unpack_action_data<balance::transfer_args>();

		if (data.from == _self || data.to != _self)
			return ;
		const eosio::name from = data.from;
		const eosio::asset amount = data.amount;

		require_auth( from );
		assert_amount( amount );

		auto iterator = balance_of.find( from.value );
		if ( iterator == balance_of.end() ) {
			create_balance( from, amount);
		}
		else {
			add_balance( from, amount, iterator );
		}
	}

	void 		balance::withdraw( const eosio::name from, const eosio::asset& amount ) {
		require_auth( from );
		assert_amount( amount );

		auto iterator = balance_of.find( from.value );
		if (iterator == balance_of.end()) {
			eosio_assert( false, "unknown user");
		}
		sub_balance( from, amount, iterator );
		
		transfer( _self, from, amount );
	}

// private
	void	balance::create_balance( const eosio::name user, const eosio::asset& amount) {
		balance_of.emplace( _self, [&]( auto& row ) {
			row.owner = user;
			row.eos_balance = amount;
		});
	}
	
	template<typename T>
	void	balance::add_balance( const eosio::name user, const eosio::asset& amount, T iterator) {
		balance_of.modify( iterator, _self, [&]( auto& row ) {
			row.eos_balance += amount;
		});
	}

	template<typename T>
	void	balance::sub_balance( const eosio::name user, const eosio::asset& amount, T iterator) {
		balance_of.modify( iterator, _self, [&]( auto& row ) {
			eosio_assert( row.eos_balance >= amount, "insufficient balance" );
			row.eos_balance -= amount;
		});

		if( iterator->is_empty() ) {
			delete_balance( iterator );
		}
	}

	template<typename T>
	void	balance::delete_balance( T iterator ) {
		balance_of.erase( iterator );
	}

	void 	balance::transfer( const eosio::name from, const eosio::name to, const eosio::asset& amount ) {
		eosio::action(
			eosio::permission_level{ from, eosio::name("active") },
			eosio::name("eosio.token"), eosio::name("transfer"),
			std::make_tuple( from, to, amount, std::string(""))
		).send();
	}

	uint64_t 	balance::t_balance::primary_key() const {
		return (owner.value);
	}

	void		balance::assert_amount( const eosio::asset& amount ) {
		eosio_assert( amount.is_valid(), "invalid amount for action" );
		eosio_assert( amount.amount > 0, "must asset positive amount for action" );
	}

	bool 		balance::t_balance::is_empty() const {
		return ( eos_balance.amount == 0 );
	}
}

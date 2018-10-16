#include "cryptopixel.hpp"

// cryptopixel::_price(1000, N(EOS));
// public
cryptopixel::cryptopixel( account_name self ) : balance(self), _price(10000, eosio::string_to_symbol(4,"EOS")) {
}

cryptopixel::t_color::t_color(
		checksum256 id,
		uint64_t coordinates,
		account_name owner,
		uint64_t color,
		std::string massage
	)
{
	_id = id; 
	_coordinates = coordinates;
	_owner = owner;
	_color = color;
	_massage = massage;
}

// eosio::action
void 	cryptopixel::buypixel( account_name sender, checksum256 id ) {
	require_auth( sender );

	// если у пользователя нету денег отклонить

	auto iterator_balance = balance_of.find( sender );
	eosio_assert( iterator_balance != balance_of.end(), "user don't have a balance" );
	eosio_assert( iterator_balance->eos_balance >= _price, "user don't have enough money");

	sub_balance( sender, _price, iterator_balance );
}

void 	cryptopixel::transfer( account_name sender, account_name to, checksum256 id ) {
	require_auth( sender );

	// если id нету отклонить
	// если владелец id не sender отклонить
	// если to не существует отклонить?? Как сделать
}

// private

void 	cryptopixel::t_color::only_owner( account_name sender ) const {
	eosio_assert( sender == _owner, "sender is not owner" );
}

bool 	cryptopixel::t_color::is_empty() const {
	return ( _owner == 0 );
}

checksum256 	cryptopixel::t_color::primary_key() const {
	return ( _id );
}

#include "cryptopixel.hpp"

// cryptopixel::_price(1000, N(EOS));
// public
cryptopixel::cryptopixel( account_name self ) :
	balance(self),
	_price(10000, eosio::string_to_symbol(4,"EOS")),
	pixel_of( _self, _self )
{
}

// cryptopixel::t_color::t_color(
// 		uint64_t id,
// 		uint64_t x,
// 		uint64_t y,
// 		account_name owner,
// 		uint64_t color,
// 		std::string massage
// 	)
// {
// 	_id = id; 
// 	_x = x;
// 	_y = y;
// 	_owner = owner;
// 	_color = color;
// 	_massage = massage;
// }

// eosio::action
void 	cryptopixel::buypixel( account_name sender, uint64_t x, uint64_t y, uint64_t color, std::string massage ) {

	eosio_assert(x <= MAX_X, "x is too big number");
	eosio_assert(y <= MAX_Y, "y is too big number");

	uint64_t id = y * 1000 + x;
	eosio_assert(id <= MAX_ID, "id is too big number");
	require_auth( sender );

	auto iterator_balance = balance_of.find( sender );
	eosio_assert( iterator_balance != balance_of.end(), "user don't have a balance" );
	eosio_assert( iterator_balance->eos_balance >= _price, "user don't have enough money");

	sub_balance( sender, _price, iterator_balance );

	auto iterator_balance_owner = balance_of.find( _self );
	add_balance( _self, _price, iterator_balance_owner );

	auto iterator_id = pixel_of.find( id );
	eosio_assert( iterator_id == pixel_of.end(), "pixel alredy a buy" );

	create_pixel( sender, id, x, y, color, massage);
}

void 	cryptopixel::transfer( account_name sender, account_name to, uint64_t id ) {
	require_auth( sender );

	auto iterator_id = pixel_of.find( id );
	eosio_assert( iterator_id != pixel_of.end(), "pixel doesn't exists" );
	eosio_assert( iterator_id->_owner == sender, "user is not owner" );
	
	pixel_of.modify( iterator_id, _self, [&]( auto& row ) {
		row._owner = to;
	});
}

// private

void cryptopixel::create_pixel( account_name sender, uint64_t id, uint64_t x, uint64_t y, uint64_t color, std::string massage ) {
	pixel_of.emplace( _self, [&]( auto& row ) {
		row._id = id;
		row._x = x;
		row._y = y;
		row._owner = sender;
		row._color = color;
		row._massage = massage;
	});
}

void 	cryptopixel::t_color::only_owner( account_name sender ) const {
	eosio_assert( sender == _owner, "sender is not owner" );
}

bool 	cryptopixel::t_color::is_empty() const {
	return ( _owner == 0 );
}

uint64_t 	cryptopixel::t_color::primary_key() const {
	return ( _id );
}

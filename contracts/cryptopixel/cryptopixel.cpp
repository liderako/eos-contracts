#include "cryptopixel.hpp"

// public
cryptopixel::cryptopixel( eosio::name receiver, eosio::name code,  eosio::datastream<const char*> ds ) :
	balance( receiver, code, ds ),
	_price_init(1000, eosio::symbol(eosio::symbol_code("EOS"), 4)),
	_pixel_of( _code, _code.value ),
	_sell_of( _code, _code.value )
{
}

bool 		cryptopixel::t_color::is_empty() const { // it's need???
	return ( _owner.value == 0 );
}

uint64_t 	cryptopixel::t_color::primary_key() const {
	return ( _id );
}

uint64_t	cryptopixel::t_sell::primary_key() const {
	return ( _id );
}

// eosio::action
void 		cryptopixel::initpixel( const eosio::name sender, uint64_t x, uint64_t y, uint64_t color, std::string message ) {

	eosio_assert(x <= MAX_X, "x is too big number");
	eosio_assert(y <= MAX_Y, "y is too big number");
	eosio_assert(color >= 0 && color <= MAX_COLOR, "invalid color");

	uint64_t id = y * 1000 + x;
	eosio_assert(id <= MAX_ID, "id is too big number");
	require_auth( sender );

	auto iterator_balance = balance_of.find( sender.value );
	eosio_assert( iterator_balance != balance_of.end(), "user don't have a balance" );
	eosio_assert( iterator_balance->eos_balance >= _price_init, "user don't have enough money");

	sub_balance( _price_init, iterator_balance );

	auto iterator_balance_owner = balance_of.find( _self.value );
	add_balance( _price_init, iterator_balance_owner );

	auto iterator_id = _pixel_of.find( id );
	eosio_assert( iterator_id == _pixel_of.end(), "pixel alredy a buy" );

	create_pixel( sender, id, x, y, color, message);
}

void 		cryptopixel::transfer( const eosio::name sender, const eosio::name to, uint64_t id ) {
	require_auth( sender );

	auto iterator_id = _pixel_of.find( id );
	eosio_assert( iterator_id != _pixel_of.end(), "pixel doesn't exists" );
	eosio_assert( iterator_id->_owner == sender, "user is not owner" );
	
	auto iterator_sell = _sell_of.find( id );
	if ( iterator_sell != _sell_of.end() ) {
		_sell_of.erase( iterator_sell ); // cancel sell
	}
	_pixel_of.modify( iterator_id, _self, [&]( auto& row ) {
		row._owner = to;
	});
}

void 		cryptopixel::editpixel( const eosio::name sender, uint64_t id, uint64_t color, std::string message ) {
	require_auth( sender );

	auto iterator_id = _pixel_of.find( id );
	eosio_assert( iterator_id != _pixel_of.end(), "pixel doesn't exists" );
	eosio_assert( iterator_id->_owner == sender, "user is not owner" );
	eosio_assert(color >= 0 && color <= MAX_COLOR, "invalid color");

	_pixel_of.modify( iterator_id, _self, [&]( auto& row ) {
		row._color = color;
		row._message = message;
	});
}


void 		cryptopixel::sellpixel( const eosio::name sender, uint64_t id, const eosio::asset price_sell ) {
	require_auth( sender );

	auto iterator_id = _pixel_of.find( id );
	eosio_assert( iterator_id != _pixel_of.end(), "pixel doesn't exists" );
	eosio_assert( iterator_id->_owner == sender, "user is not owner" );
	eosio_assert( price_sell >= _price_init, "small price" );

	auto iterator_sell = _sell_of.find( id );
	eosio_assert( iterator_sell == _sell_of.end(), "pixel alredy selling" );

	_sell_of.emplace( _self, [&]( auto& row ) {
		row._id = id;
		row._owner = sender;
		row._price = price_sell;
	});
}

void 		cryptopixel::cancelsell( const eosio::name sender, uint64_t id ) {
	require_auth( sender );

	auto iterator_sell = _sell_of.find( id );
	eosio_assert( iterator_sell != _sell_of.end(), "pixel doesn't exists for sell" );
	eosio_assert( iterator_sell->_owner == sender, "user is not owner" );

	_sell_of.erase( iterator_sell );
}

void 		cryptopixel::buypixel( const eosio::name sender, uint64_t id, uint64_t new_color, std::string new_message ) {
	require_auth( sender );

	auto iterator_sell = _sell_of.find( id );
	auto iterator_id = _pixel_of.find( id );
	auto iterator_balance_sender = balance_of.find( sender.value );

	eosio_assert( iterator_sell != _sell_of.end(), "pixel doesn't exists for sell" );
	eosio_assert( iterator_balance_sender != balance_of.end(), "user don't have a balance" );
	eosio_assert( iterator_balance_sender->eos_balance >= iterator_sell->_price, "user don't have enough money" );
	eosio_assert( sender != iterator_sell->_owner, "can not buy from yourself");
	eosio_assert(color >= 0 && color <= MAX_COLOR, "invalid color");

	auto iterator_balance_owner = balance_of.find( iterator_sell->_owner.value );
	sub_balance( iterator_sell->_price, iterator_balance_sender );
	if (iterator_balance_owner == balance_of.end()) {
		create_balance(iterator_sell->_owner, iterator_sell->_price);
	}
	else {
		add_balance( iterator_sell->_price, iterator_balance_owner );
	}
	_pixel_of.modify( iterator_id, _self, [&]( auto& row ) {
		row._owner = sender;
		row._color = new_color;
		row._message = new_message;
	});
	_sell_of.erase( iterator_sell );
}

// private

void		cryptopixel::create_pixel( const eosio::name sender, uint64_t id, uint64_t x, uint64_t y, uint64_t color, std::string message ) {
	_pixel_of.emplace( _self, [&]( auto& row ) {
		row._id = id;
		row._x = x;
		row._y = y;
		row._owner = sender;
		row._color = color;
		row._message = message;
	});
}

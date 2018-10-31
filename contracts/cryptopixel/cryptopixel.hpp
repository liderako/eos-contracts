#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include "../balance/balance.cpp"

class [[eosio::contract]] cryptopixel : public balancebook::balance {
	public:
		cryptopixel( eosio::name receiver, eosio::name code,  eosio::datastream<const char*> ds );
		
		ACTION initpixel( const eosio::name sender, uint64_t x, uint64_t y, uint64_t color, std::string message ) ;

		ACTION transfer( const eosio::name sender, const eosio::name to, uint64_t id );

		ACTION editpixel( const eosio::name sender, uint64_t id, uint64_t color, std::string message );
		
		ACTION buypixel( const eosio::name sender, uint64_t id, uint64_t new_color, std::string new_message );

		ACTION sellpixel( const eosio::name sender, uint64_t id, const eosio::asset price_sell );

		ACTION cancelsell( const eosio::name sender, uint64_t id );
	private:
		struct [[eosio::table, eosio::contract("cryptopixel")]] t_color {
				uint64_t		_id;
				uint64_t 		_x;
				uint64_t 		_y;
				eosio::name 	_owner;
				uint64_t 		_color;
				std::string 	_message;

				uint64_t 		primary_key() const;
				bool 			is_empty() const;
		};

		struct [[eosio::table, eosio::contract("cryptopixel")]] t_sell {
			uint64_t 		_id;
			eosio::name 	_owner;
			eosio::asset 	_price;

			uint64_t primary_key() const;
		};

		static uint32_t const MAX_X = 1000;

		static uint32_t const MAX_Y = 1000;

		static uint32_t const MAX_ID = MAX_X * MAX_Y;

		static uint32_t const MAX_COLOR = 0xFFFFFF;

		typedef eosio::multi_index<eosio::name("pixel.map"), t_color> pixel_index;

		typedef eosio::multi_index<eosio::name("sell.map"), t_sell> sell_index;

		pixel_index _pixel_of;
		
		sell_index	_sell_of;

		eosio::asset _price_init;

		void create_pixel( const eosio::name sender, uint64_t id, uint64_t x, uint64_t y, uint64_t color, std::string message );
};

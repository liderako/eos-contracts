#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include "../balance/balance.cpp"

class [[eosio::contract]] cryptopixel : public balancebook::balance {
	public:
		cryptopixel( eosio::name receiver, eosio::name code,  eosio::datastream<const char*> ds );
		
		ACTION initpixel( const eosio::name sender, uint64_t x, uint64_t y, uint64_t color, std::string massage ) ;

		ACTION transfer( const eosio::name sender, const eosio::name to, uint64_t id );

	private:
		struct [[eosio::table, eosio::contract("cryptopixel")]] t_color {
				uint64_t		_id;
				uint64_t 		_x;
				uint64_t 		_y;
				eosio::name 	_owner;
				uint64_t 		_color;
				std::string 	_massage;

				uint64_t 		primary_key() const;
				bool 			is_empty() const;
				// void			only_owner( account_name owner ) const;
		};

		static uint32_t const MAX_X = 1000;

		static uint32_t const MAX_Y = 1000;

		static uint32_t const MAX_ID = MAX_X * MAX_Y;

		eosio::asset _price;

		typedef eosio::multi_index<eosio::name("pixel.map"), t_color> pixel_index;

		pixel_index pixel_of;

		void create_pixel( const eosio::name sender, uint64_t id, uint64_t x, uint64_t y, uint64_t color, std::string massage );
};

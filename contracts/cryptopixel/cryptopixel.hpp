#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include "../balance/balance.cpp"

class cryptopixel : public balancebook::balance {
	public:
		cryptopixel( account_name self );
		
		[[eosio::action]]
		void buypixel( account_name sender, uint64_t x, uint64_t y, uint64_t color, std::string massage ) ;

		[[eosio::action]]
		void transfer( account_name sender, account_name to, uint64_t id );

	private:
		struct [[eosio::table]] t_color {
				uint64_t		_id;
				uint64_t 		_x;
				uint64_t 		_y;
				account_name 	_owner;
				uint64_t 		_color;
				std::string 	_massage;

				//t_color(uint64_t id, uint64_t x, uint64_t y, account_name owner, uint64_t color, std::string massage);
				
				uint64_t 		primary_key() const;
				bool 			is_empty() const;
				void			only_owner( account_name owner ) const;
		};

		static uint32_t const MAX_X = 1000;

		static uint32_t const MAX_Y = 1000;

		static uint32_t const MAX_ID = MAX_X * MAX_Y;

		eosio::asset _price;

		typedef eosio::multi_index<N(pixel.map), t_color> pixel_index;

		pixel_index pixel_of;

		void create_pixel( account_name sender, uint64_t id, uint64_t x, uint64_t y, uint64_t color, std::string massage );
};

// id = 0;
// id = 1; x = 1; y = 0;
// id = 1000; x = 1000; y = 0;
// id = 1001; x = 0; y = 1
// id 1 000 000 / 1000 
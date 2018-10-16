#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include "../balance/balance.cpp"

class cryptopixel : public balancebook::balance {
	public:
		cryptopixel( account_name self );
		
		[[eosio::action]]
		void buypixel( account_name sender, checksum256 id );

		[[eosio::action]]
		void transfer( account_name sender, account_name to, checksum256 id );

	private:
		struct [[eosio::table]] t_color {
			public:
				t_color(
					checksum256 id, // sha256(x, y)
					uint64_t _coordinates,
					account_name owner,
					uint64_t color,
					std::string massage
				);

			private:
				checksum256		_id;
				uint64_t 		_coordinates;
				account_name 	_owner;
				uint64_t 		_color;
				std::string 	_massage;

				checksum256 	primary_key() const;
				bool 			is_empty() const;
				void			only_owner( account_name owner ) const;
		};

		static const uint32_t MAX_X = 1000;

		static const uint32_t MAX_Y = 1000;

		const eosio::asset PRICE;

		typedef eosio::multi_index<N(pixel.map), t_color> pixel_index;
};

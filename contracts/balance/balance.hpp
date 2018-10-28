#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

namespace balancebook {

	class [[eosio::contract]] balance : public eosio::contract {
		public:
			balance(eosio::name receiver, eosio::name code,  eosio::datastream<const char*> ds);

			ACTION withdraw( const eosio::name sender, const eosio::asset& quantity );

			ACTION deposit( const eosio::name sender, const eosio::asset& quantity );
			
			struct transfer_args {
				eosio::name		from;
				eosio::name		to;
				eosio::asset 	amount;
				std::string		memo;
			};
			
			template<typename T>
			void	add_balance( const eosio::name user, const eosio::asset& quantity, T iterator );

			template<typename T>
			void	sub_balance( const eosio::name user, const eosio::asset& amount, T iterator);

		protected:
			struct [[eosio::table]] t_balance {
				eosio::name	owner;
				eosio::asset	eos_balance;

				uint64_t		primary_key() const;
				bool			is_empty() const;
			};
			typedef eosio::multi_index<eosio::name("balance.of"), t_balance> balance_index;
			
			balance_index 	balance_of;

		private:
			void 	assert_amount( const eosio::asset& amount );

			void 	transfer( const eosio::name from, const eosio::name to, const eosio::asset& amount );

			template<typename T>
			void	delete_balance( T iterator );

			void	create_balance( const eosio::name user, const eosio::asset& quantity );
	};
}

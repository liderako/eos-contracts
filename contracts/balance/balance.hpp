#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

namespace balancebook {
	using namespace eosio;

	class balance : public eosio::contract {
		public:
			balance( account_name self );

			[[eosio::action]]
			void withdraw( const account_name sender, const asset& quantity );

			[[eosio::action]]
			void deposit( const account_name sender, const asset& quantity );
			
			struct transfer_args {
				account_name	from;
				account_name	to;
				asset			amount;
				std::string		memo;
			};
			
			template<typename T>
			void	add_balance( const account_name user, const asset& quantity, T iterator );

			template<typename T>
			void	sub_balance( const account_name user, const asset& amount, T iterator);

		protected:
			struct [[eosio::table]] t_balance {
				account_name	owner;
				asset			eos_balance;

				uint64_t		primary_key() const;
				bool			is_empty() const;
			};
			typedef eosio::multi_index<N(balance.of), t_balance> balance_index;
			
			balance_index 	balance_of;

		private:
			void 	assert_amount( const asset& amount );

			void 	transfer( const account_name from, const account_name to, const asset& amount );

			template<typename T>
			void	delete_balance( T iterator );

			template<typename T>
			void	create_balance( const account_name user, const asset& quantity, T iterator );
	};
}

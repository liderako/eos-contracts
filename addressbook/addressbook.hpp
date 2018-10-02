#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

namespace addressbook {
	using namespace eosio;

	class addressbook : public eosio::contract {
		public:
			addressbook( account_name self );
			[[eosio::action]]
			void upsert(
				account_name user, 
				std::string first_name, 
				std::string last_name, 
				std::string street, 
				std::string city, 
				std::string state
			);

		private:
			struct [[eosio::table]] person {
				uint64_t key;
				std::string first_name;
				std::string last_name;
				std::string street;
				std::string city;
				std::string state;

				uint64_t primary_key() const;
			};

		typedef eosio::multi_index<N( person ), person> address_index;
	};
	EOSIO_ABI( addressbook, (upsert))
}
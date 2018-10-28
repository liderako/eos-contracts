#include <eosiolib/eosio.hpp>

namespace book {
	using namespace eosio;

	class [[eosio::contract]] addressbook : public eosio::contract {
		public:
			addressbook(eosio::name receiver, eosio::name code,  datastream<const char*> ds);

			ACTION load(
				eosio::name user, 
				std::string first_name, 
				std::string last_name, 
				std::string street, 
				std::string city, 
				std::string state
			);
			
			ACTION modify(
				eosio::name user, 
				std::string first_name, 
				std::string last_name, 
				std::string street, 
				std::string city, 
				std::string state
			);

			ACTION remove(eosio::name user);

			ACTION notify(eosio::name user, std::string msg);

		private:
			struct [[eosio::table]] person {
				eosio::name key;
				std::string first_name;
				std::string last_name;
				std::string street;
				std::string city;
				std::string state;

				capi_name primary_key() const;
			};

		typedef eosio::multi_index<eosio::name("people"), person> address_index;

		void send_summary(eosio::name user, std::string message);

		void increment_counter( eosio::name user, std::string type );
	};
}
// EOSIO_ABI( book::addressbook, (load)(modify)(remove)(notify))
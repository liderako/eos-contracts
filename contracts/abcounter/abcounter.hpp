#include <eosiolib/eosio.hpp>

namespace abcounter {
	class [[eosio::contract]] abcounter : public eosio::contract {
		public:
			using contract::contract;
			abcounter(eosio::name receiver, eosio::name code,  eosio::datastream<const char*> ds);
		
		ACTION count( eosio::name user, std::string type );
		private:
			struct [[eosio::table]] counter {
				eosio::name key;
				uint64_t loaded;
				uint64_t modified;
				uint64_t removed;
				uint64_t primary_key() const;
			};
			using count_index = eosio::multi_index<eosio::name("counts"), counter>;
	};
	EOSIO_DISPATCH( abcounter, (count));
}

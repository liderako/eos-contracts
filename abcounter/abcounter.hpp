#include <eosiolib/eosio.hpp>

namespace abcounter {
	class abcounter : public eosio::contract {
		public:
			using contract::contract;
			abcounter( account_name self );
		[[eosio::action]]
		void count( account_name user, std::string type );
		private:
			struct [[eosio::table]] counter {
				account_name key;
				uint64_t loaded;
				uint64_t modified;
				uint64_t removed;
				uint64_t primary_key() const;
			};
			using count_index = eosio::multi_index<N(counts), counter>;
	};
	EOSIO_ABI( abcounter, (count));
}

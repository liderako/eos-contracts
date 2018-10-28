#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;

class [[eosio::contract]] hello : public eosio::contract {
	public:
		using contract::contract;

        ACTION hi( capi_name user ) {
            print( "Hello, ", eosio::name{user} );
        }
};
EOSIO_DISPATCH( hello, (hi))

#include "balance.cpp"

extern "C" {
using namespace balancebook;
	void 	apply( eosio::name self, eosio::name code, eosio::name action ) {
		if (code == N(eosio.token) && action == eosio::name("transfer") ) {
			execute_action( self, code, &balance::deposit );
		}
		else if ( code == self && action == eosio::name("withdraw") ) {
			execute_action( self, code, &balance::withdraw );
		}
	}
};

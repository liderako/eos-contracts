#include "balance.cpp"

extern "C" {
using namespace balancebook;
	void 	apply( eosio::name self, eosio::name code, eosio::name action ) {
		balance _balance(self, code, action);
		if (code == eosio::name("eosio.token") && action == eosio::name("transfer") ) {
			execute_action( self, code, &_balance.deposit );
		}
		else if ( code == self && action == eosio::name("withdraw") ) {
			execute_action( self, code, &_balance.withdraw );
		}
	}
};

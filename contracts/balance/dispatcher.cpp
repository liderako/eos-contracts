#include "balance.cpp"

extern "C" {
using namespace balancebook;
    void apply(uint64_t self, uint64_t code, uint64_t action) {
        balance _balance(self);
        if(code == N(eosio.token) && action == eosio::name("transfer") ) {
            execute_action( &_balance, &balance::deposit );
        }
        else if ( code == self && action == eosio::name("withdraw") ) {
            execute_action( &_balance, &balance::withdraw );
        }
    }
};

#include "balance.cpp"

extern "C" {
using namespace balancebook;
    void apply(uint64_t self, uint64_t code, uint64_t action) {
        balance _balance(self);
        if(code == N(eosio.token) && action == N(transfer) ) {
            execute_action( &_balance, &balance::deposit );
        }
        else if ( code == self && action == N(withdraw) ) {
            execute_action( &_balance, &balance::withdraw );
        }
    }
};

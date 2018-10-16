#include "cryptopixel.cpp"

extern "C" {
	void	apply( uint64_t self, uint64_t code, uint64_t action ) {
		cryptopixel _pixel(self);
		if (code == N(eosio.token) && action == N(transfer) ) {
			execute_action( &_pixel, &cryptopixel::deposit );
		}
		else if ( code == self && action == N(withdraw) ) {
			execute_action( &_pixel, &cryptopixel::withdraw );
		}
		else if ( code == self && action == N(buypixel) ) {
			execute_action( &_pixel, &cryptopixel::buypixel );
		}
		else if ( code == self && action == N(transfer) ) {
			execute_action( &_pixel, &cryptopixel::transfer );
		}
	}
};

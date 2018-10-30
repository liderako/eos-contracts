#include "cryptopixel.cpp"

extern "C" {
	void	apply( eosio::name self, eosio::name code, eosio::name action ) {

		if (code == eosio::name("eosio.token") && action == eosio::name("transfer") ) {
			execute_action( self, code, &cryptopixel::deposit );
		}
		else if ( code == self && action == eosio::name("withdraw") ) {
			execute_action( self, code, &cryptopixel::withdraw );
		}
		else if ( code == self && action == eosio::name("initpixel") ) {
			execute_action( self, code, &cryptopixel::initpixel );
		}
		else if ( code == self && action == eosio::name("transfer") ) {
			execute_action( self, code, &cryptopixel::transfer );
		}
	}
};

// set account permission cryptopixel active '{"threshold": 1,"keys": [{"key": "EOS5rSX7m2xUYanCD2TRXTNSy17DzFqd33TBkY2yrzfxQxpEVmzfM","weight": 1}], "accounts": [{"permission":{"actor":"cryptopixel","permission":"eosio.code"},"weight":1}]}' -p cryptopixel@owner

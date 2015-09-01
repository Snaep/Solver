#include "combinator.h"
#include "defaults.h"

int Combinator_Initialize( struct Combinator* c, int k, int* values, int n ) {
	//parameter validation
	if( c == NULL || values == NULL || k > 64 || k > n ) return -1;
	
	c->_permutation = 0;
	c->_b = 0;

	//create first combination 
	//e.g. 1111000...
	for( int i = 0; i < k; i++ ) {
		c->_i[i] = i;
	}

	c->values = values;
	c->_k = k;
	c->_n = n;
	c->_nmk = n - k;

	return 0;
}

int Combinator_RotateCombination( int* _i, int _nmk, int _n, int bit ) {
	if( bit == 0 ) {
		_i[0]++;
		if( _i[0] > _nmk ) return -1;
		return _i[0] + 1;
	}

	_i[bit]++;
	while( _i[bit] == _n ) {
		_i[bit] = Combinator_RotateCombination( _i, _nmk, _n, bit - 1 );
		if( _i[bit] < 0 ) return -1;
	}

	return _i[bit] + 1;
}

int Combinator_GetNext( struct Combinator* c, int* dest ) {
	//if no other combinations are available return -1
	if( c->_b != 0 ) return -1;

	//map combnation to dest
	for( int i = 0; i < c->_k; i++ ) {
		dest[i] = c->values[c->_i[i]];
	}

	//create next combination
	//save availability to _b
	c->_b = Combinator_RotateCombination( c->_i, c->_nmk, c->_n, c->_k - 1 ) == -1;
	return 0;
}

#include "combinator.h"

void Combinator_Initialize( struct Combinator* c, int k, int* values, int n ) {
	c->_permutation = 0;
	c->_b = 0;

	for( int i = 0; i < k; i++ ) {
		c->_i[i] = i;
	}

	c->values = values;
	c->_k = k;
	c->_n = n;
	c->_nmk = n - k;
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
	if( c->_b != 0 ) return -1;

	for( int i = 0; i < c->_k; i++ ) {
		dest[i] = c->values[c->_i[i]];
	}

	c->_b = Combinator_RotateCombination( c->_i, c->_nmk, c->_n, c->_k - 1 ) == -1;
	return 0;
}

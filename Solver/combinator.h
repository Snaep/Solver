#pragma once

struct Combinator {
	int* values;
	unsigned long long _permutation;
	int _n, _k, _nmk;
	int _b;
	int _i[64];
};

//to initialize k out of n combinator
//no repitition
//no order
//k: max 64
void Combinator_Initialize( struct Combinator* c, int k, int* values, int n );

//writes next combination to dest
//dest must be array with n places
//return   0: success
//returns != 0: no more combinations
int Combinator_GetNext( struct Combinator* c, int* dest );

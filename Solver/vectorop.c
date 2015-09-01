#include "vectorop.h"

#ifndef SUDOKU_CELLTYPE_BITVECTOR
//cout = c1 & c2
void vand( SudokuCell c1, SudokuCell c2, SudokuCell cout, int length ) {
	int i;
	for( i = 0; i < length; i++ ) cout[i] = c1[i] & c2[i];
}
//cout = c1 | c2
void vor( SudokuCell c1, SudokuCell c2, SudokuCell cout, int length ) {
	int i;
	for( i = 0; i < length; i++ ) cout[i] = c1[i] | c2[i];
}
//cout = c1 ^ c2
void vxor( SudokuCell c1, SudokuCell c2, SudokuCell cout, int length ) {
	int i;
	for( i = 0; i < length; i++ ) cout[i] = c1[i] ^ c2[i];
}
//cout = ~c1
void vnot( SudokuCell c1, SudokuCell cout, int length ) {
	int i;
	for( i = 0; i < length; i++ ) cout[i] = 1 - c1[i];
}
//returns no. of 1 elements
int vpopcnt( SudokuCell c1, int length ) {
	int i, r;

	for( i = 0, r = 0; i < length; i++ ) r += ( c1[i] != 0 );

	return r;
}

//c == 0
int viszero( SudokuCell c, int length ) {
	int i = 0;
	for( i = 0; i < length; i++ ) {
		if( c[i] != 0 ) return 0;
	}
	return 1;
}
//c1 |= c2
void vset( SudokuCell c1, SudokuCell c2, int length ) {
	int i;

	for( i = 0; i < length; i++ ) c1[i] |= c2[i];
}
//c1 &= ~c2
void vunset( SudokuCell c1, SudokuCell c2, int length ) {
	int i;

	for( i = 0; i < length; i++ ) c1[i] &= ( 1 - c2[i] );
}

//sets all c1 'bits' to v
void vinit( SudokuCell c1, int v, int length ) {
	int i = 0;
	for( i = 0; i < length; i++ ) c1[i] = v;
}

//c1 = c2
void vcopy( SudokuCell c1, SudokuCell c2, int length ) {
	int i = 0;
	for( i = 0; i < length; i++ ) c1[i] = c2[i];
}

//c1 == c2
int vequals( SudokuCell c1, SudokuCell c2, int length ) {
	int i = 0;
	for( i = 0; i < length; i++ ) if( c1[i] != c2[i] ) return 0;
	return 1;
}
#endif
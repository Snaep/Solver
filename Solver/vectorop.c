#include "vectorop.h"

#ifndef SUDOKU_CELLTYPE_BITVECTOR
void vand( SudokuCell c1, SudokuCell c2, SudokuCell cout, int length ) {
	int i;
	for( i = 0; i < length; i++ ) cout[i] = c1[i] & c2[i];
}
void vor( SudokuCell c1, SudokuCell c2, SudokuCell cout, int length ) {
	int i;
	for( i = 0; i < length; i++ ) cout[i] = c1[i] | c2[i];
}
void vxor( SudokuCell c1, SudokuCell c2, SudokuCell cout, int length ) {
	int i;
	for( i = 0; i < length; i++ ) cout[i] = c1[i] ^ c2[i];
}
void vnot( SudokuCell c1, SudokuCell cout, int length ) {
	int i;
	for( i = 0; i < length; i++ ) cout[i] = 1 - c1[i];
}
int vpopcnt( SudokuCell c1, int length ) {
	int i, r;

	for( i = 0, r = 0; i < length; i++ ) r += ( c1[i] != 0 );

	return r;
}

int viszero( SudokuCell c, int length ) {
	int i = 0;
	for( i = 0; i < length; i++ ) {
		if( c[i] != 0 ) return 0;
	}
	return 1;
}
void vset( SudokuCell c1, SudokuCell c2, int length ) {
	int i;

	for( i = 0; i < length; i++ ) c1[i] |= c2[i];
}
void vunset( SudokuCell c1, SudokuCell c2, int length ) {
	int i;

	for( i = 0; i < length; i++ ) c1[i] &= ( 1 - c2[i] );
}

void vinit( SudokuCell c1, int v, int length ) {
	int i = 0;
	for( i = 0; i < length; i++ ) c1[i] = v;
}

void vcopy( SudokuCell c1, SudokuCell c2, int length ) {
	int i = 0;
	for( i = 0; i < length; i++ ) c1[i] = c2[i];
}

int vequals( SudokuCell c1, SudokuCell c2, int length ) {
	int i = 0;
	for( i = 0; i < length; i++ ) if( c1[i] != c2[i] ) return 0;
	return 1;
}
#endif
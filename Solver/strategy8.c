#include "strategies.h"

//hidden pair col
#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy8( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	SudokuCell candidate;

	for( i = 0; i < sud->length; i++ ) {
		//find partner for cell
		if( i == y || sud->cellvalue[i][x] == 0 ) continue;
		candidate = ( sud->grid[y][x] & sud->grid[i][x] );

		//if partner is naked pair (cell & partner) == (cell | partner)
		if( candidate == ( sud->grid[y][x] | sud->grid[y][i] ) ) continue;

		//remove all candidates occuring in other cells from current pair
		for( j = 0; j < sud->length; j++ ) {
			if( j == i || j == y ) continue;
			candidate &= ( ~( sud->grid[j][x] ) );
		}

		//if number of candidates in pair is still 2, it is a hidden pair
		//remove all other candidates from pair cells
		if( __popcnt64( candidate ) == 2 ) {
			sud->grid[y][x] = candidate;
			sud->grid[i][x] = candidate;
			return 1;
		}
	}
	return 0;
}
#else
int strategy8( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	unsigned int candidate[64], buffer[64];

	for( i = 0; i < sud->length; i++ ) {
		//find partner for cell
		if( i == y || sud->cellvalue[i][x] == 0 ) continue;
		vandl( sud->grid[y][x], sud->grid[i][x], candidate );

		//if partner is naked pair (cell & partner) == (cell | partner)
		vorl( sud->grid[y][x], sud->grid[i][x], buffer );
		if( vequalsl( candidate, buffer ) ) continue;

		//remove all candidates occuring in other cells from current pair
		for( j = 0; j < sud->length; j++ ) {
			if( j == i || j == y ) continue;
			vunsetl( candidate, sud->grid[j][x] );
		}
		//if number of candidates in pair is still 2, it is a hidden pair
		//remove all other candidates from pair cells
		if( vpopcntl( candidate ) == 2 ) {
			vcopyl( sud->grid[y][x], candidate );
			vcopyl( sud->grid[i][y], candidate );
			return 1;
		}
	}
	return 0;
}
#endif

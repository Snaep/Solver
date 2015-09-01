#include "strategies.h"

//hidden pair in row-neighbourhood
#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy9( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	SudokuCell candidate;

	for( i = 0; i < sud->length; i++ ) {
		if( &sud->grid[y][x] == sud->cellbox[y][x][i] || sud->cellboxvalue[y][x][i] == 0 ) continue;
		candidate = ( sud->grid[y][x] & *sud->cellbox[y][x][i] );
		if( candidate == ( sud->grid[y][x] | *sud->cellbox[y][x][i] ) ) continue;

		for( j = 0; j < sud->length; j++ ) {
			if( j == i || &sud->grid[y][x] == sud->cellbox[y][x][j] ) continue;
			candidate &= ( ~( *sud->cellbox[y][x][j] ) );
		}

		if( __popcnt64( candidate ) == 2 ) {
			sud->grid[y][x] = candidate;
			sud->grid[y][i] = candidate;
			return 1;
		}
	}
	return 0;
}
#else
int strategy9( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	unsigned int candidate[64], buffer[64];

	for( i = 0; i < sud->length; i++ ) {
		if( &sud->grid[y][x] == sud->cellbox[y][x][i] || sud->cellboxvalue[y][x][i] == 0 ) continue;
		vandl( sud->grid[y][x], *sud->cellbox[y][x][i], candidate );
		vorl( sud->grid[y][x], *sud->cellbox[y][x][i], buffer );
		if( vequalsl( candidate, buffer ) ) continue;
		for( j = 0; j < sud->length; j++ ) {
			if( j == i || &sud->grid[y][x] == sud->cellbox[y][x][j] ) continue;
			vunsetl( candidate, *sud->cellbox[y][x][j] );
		}
		if( vpopcntl( candidate ) == 2 ) {
			vcopyl( sud->grid[y][x], candidate );
			vcopyl( *sud->cellbox[y][x][i], candidate );
			return 1;
		}
	}
	return 0;
}
#endif
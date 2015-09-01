#include "strategies.h"

//naked pair row
#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy4( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	SudokuCell changed;
	if( __popcnt64( sud->grid[y][x] ) != 2 ) return 0;

	for( i = 0; i < sud->length; i++ ) {
		if( i == x ) continue;
		if( ( sud->grid[y][i] ^ sud->grid[y][x] ) == 0 ) {
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				if( j != i && j != x ) {
					changed |= ( sud->grid[y][j] & sud->grid[y][x] );
					sud->grid[y][j] &= ( ~( sud->grid[y][x] ) );
				}
			}

			return changed != 0;
		}
	}

	return 0;
}
#else
int strategy4( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j, changed;
	unsigned int buffer[64];

	if( vpopcntl( sud->grid[y][x] ) != 2 ) return 0;

	for( i = 0; i < sud->length; i++ ) {
		if( i == x ) continue;

		vxorl( sud->grid[y][i], sud->grid[y][x], buffer );
		if( viszerol( buffer ) ) {
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				if( j != i && j != x ) {
					vandl( sud->grid[y][j], sud->grid[y][x], buffer );
					changed += vpopcntl( buffer );
					vunsetl( sud->grid[y][j], sud->grid[y][x] );
				}
			}

			return changed != 0;
		}
	}

	return 0;
}

#endif
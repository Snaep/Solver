#include "strategies.h"

//naked pair row
#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy4( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	SudokuCell changed;

	//if cell contains more than 2 its not naked pair
	if( __popcnt64( sud->grid[y][x] ) != 2 ) return 0;

	for( i = 0; i < sud->length; i++ ) {
		if( i == x ) continue;

		//find identical partner
		if( sud->grid[y][i] == sud->grid[y][x] ) {
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				//delete candidates from all other cells in neighbourhood
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

	//if cell contains more than 2 its not naked pair
	if( vpopcntl( sud->grid[y][x] ) != 2 ) return 0;

	for( i = 0; i < sud->length; i++ ) {
		if( i == x ) continue;

		//find identical partner
		if(vequalsl(sud->grid[y][x], sud->grid[y][i])) {
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				//delete candidates from all other cells in neighbourhood
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
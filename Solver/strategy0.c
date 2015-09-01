#include "strategies.h"

//prüft ob eine Zelle nur einen möglichen Wert hat
#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy0( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
	unsigned int i;

	if( __popcnt64( sud->grid[y][x] ) == 1 ) {
		for( i = 0; i < sud->length; i++ ) {
			if( sud->grid[y][x] & ( 1ll << i ) ) {
				Sudoku_SetCell( sud, x, y, i + 1 );
				return 1;
			}
		}
	}

	return 0;
}

#else
int strategy0( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
	unsigned int i;
		
	if( vpopcntl( sud->grid[y][x] ) == 1 ) {
		for( i = 0; i < sud->length; i++ ) {
			if( sud->grid[y][x][i] ) {
				Sudoku_SetCell( sud, x, y, i + 1 );
				return 1;
			}
		}
	}

	return 0;
}
#endif
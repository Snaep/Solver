#include "strategies.h"

//Naked pairs box
#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy6( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	SudokuCell changed;

	//if cell contains more than 2 its not naked pair
	if( __popcnt64( sud->grid[y][x] ) != 2 ) return 0;

	for( i = 0; i < sud->length; i++ ) {
		//find identical partner
		if( ( ( sud->grid[y][x] ) == ( *sud->cellbox[y][x][i] ) ) && ( &sud->grid[y][x] ) != sud->cellbox[y][x][i] ) {
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				//delete candidates from all other cells in neighbourhood
				if( ( j != i ) && ( sud->cellbox[y][x][j] != &sud->grid[y][x] ) ) {
					changed |= ( ( *sud->cellbox[y][x][j] ) & sud->grid[y][x] );
					*sud->cellbox[y][x][j] &= ( ~( sud->grid[y][x] ) );
				}
			}
			return changed != 0;
		}
	}

	return 0;
}

#else
int strategy6( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j, changed;
	unsigned int buffer[64];

	//if cell contains more than 2 its not naked pair
	if( vpopcntl( sud->grid[y][x] ) != 2 ) return 0;

	for( i = 0; i < sud->length; i++ ) {
		if( &sud->grid[y][x] == sud->cellbox[y][x][i] ) continue;

		//find identical partner
		if(vequalsl( *sud->cellbox[y][x][i], sud->grid[y][x] ) ) {
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				//delete candidates from all other cells in neighbourhood
				if( j != i && &sud->grid[y][x] != sud->cellbox[y][x][j] ) {
					vandl( *sud->cellbox[y][x][j], sud->grid[y][x], buffer );
					changed += vpopcntl( buffer );
					vunsetl( *sud->cellbox[y][x][j], sud->grid[y][x] );
				}
			}

			return changed != 0;
		}
	}

	return 0;
}
#endif
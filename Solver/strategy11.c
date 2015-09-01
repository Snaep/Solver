#include "strategies.h"

#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy11( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	struct Combinator c;
	SudokuCell cellok;
	SudokuCell subset;
	SudokuCell changed;

	unsigned int i, j, k;
	unsigned int index[64] = { 0 };
	unsigned int combination[5] = { 0 };

	j = 0;

	//count number of empty cells in neighbourhood
	for( i = 0; i < sud->length; i++ ) {
		if( sud->cellvalue[y][i] == 0 && i != x ) {
			index[j++] = i;
		}
	}

	if( j <= SUDOKU_SUBSET_MIN ) return 0;

	//for defined subset sizes
	for( i = SUDOKU_SUBSET_MIN; i <= SUDOKU_SUBSET_MAX; i++ ) {
		Combinator_Initialize( &c, i, index, j );
		combination[i] = x;

		//for each available combination
		while( Combinator_GetNext( &c, combination ) == 0 ) {

			subset = 0;
			for( j = 0; j <= i; j++ ) {
				//combine all subsets
				subset |= sud->grid[y][combination[j]];
				for( k = j + 1; k <= i; k++ ) {
					//create validation mask
					if( sud->grid[y][combination[j]] & sud->grid[y][combination[k]] ) {
						cellok |= ( ( 1 << j ) | ( 1 << k ) );
						break;
					}
				}
			}
			//validate subset
			if( __popcnt64( cellok ) != i + 1 ) continue;
			if( __popcnt64( subset ) != i + 1 ) continue;

			//remove candidates from all other cells in neighbourhood
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				if( ( cellok & ( 1ll << j ) ) == 0 ) {
					changed |= ( sud->grid[y][j] & subset );
					sud->grid[y][j] &= ( ~subset );
				}
			}

			return changed != 0;
		}

	}

	return 0;
}
#else
int strategy11( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	struct Combinator c;
	unsigned int cellok[64];
	unsigned int subset[64];
	unsigned int buffer[64];

	unsigned int changed;

	unsigned int i, j, k;
	unsigned int index[64] = { 0 };
	unsigned int combination[5] = { 0 };

	j = 0;

	//count number of empty cells in neighbourhood
	for( i = 0; i < sud->length; i++ ) {
		if( sud->cellvalue[y][i] == 0 && i != x ) {
			index[j++] = i;
		}
	}

	if( j <= SUDOKU_SUBSET_MIN ) return 0;

	//for defined subset sizes
	for( i = SUDOKU_SUBSET_MIN; i <= SUDOKU_SUBSET_MAX; i++ ) {
		Combinator_Initialize( &c, i, index, j );
		combination[i] = x;

		//for each available combination
		while( Combinator_GetNext( &c, combination ) == 0 ) {

			vinitl( subset, 0 );
			for( j = 0; j <= i; j++ ) {
				vsetl( subset, sud->grid[y][combination[j]] );

				for( k = j + 1; k <= i; k++ ) {
					//combine all subsets
					vandl( sud->grid[y][combination[j]], sud->grid[y][combination[k]], buffer );

					//create validation mask
					if( !viszerol( buffer ) ) {
						cellok[j] = 1;
						cellok[k] = 1;
						break;
					}
				}
			}
			//validate subset
			if( vpopcntl( cellok ) != i + 1 ) continue;
			if( vpopcntl( subset ) != i + 1 ) continue;

			//remove candidates from all other cells in neighbourhood
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				if( cellok[j] != 0 ) {
					vandl( sud->grid[y][j], subset, buffer );
					changed += vpopcntl( buffer );
					vunsetl( sud->grid[y][j], subset );
				}
			}

			return changed != 0;
		}

	}

	return 0;
}

#endif
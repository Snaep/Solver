#include "strategies.h"

#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy13( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
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
		if( sud->cellvalue[i][x] == 0 && i != y ) {
			index[j++] = i;
		}
	}

	if( j <= SUDOKU_SUBSET_MIN ) return 0;

	//for defined subset sizes
	for( i = SUDOKU_SUBSET_MIN; i <= SUDOKU_SUBSET_MAX; i++ ) {
		Combinator_Initialize( &c, i, index, j );
		combination[i] = y;

		//for each available combination
		while( Combinator_GetNext( &c, combination ) == 0 ) {
			subset = 0ll;

			for( j = 0; j <= i; j++ ) {
				for( k = j + 1; k <= i; k++ ) {
					//combine all subsets
					//create validation mask
					if( sud->grid[combination[j]][x] & sud->grid[combination[k]][x] ) {
						cellok |= ( ( 1 << j ) | ( 1 << k ) );
						subset |= sud->grid[combination[j]][x] & sud->grid[combination[k]][x];
						break;
					}
				}
			}

			//validate subset
			if( __popcnt64( cellok ) != i + 1 ) continue;

			for( j = 0; j < sud->length; j++ ) {
				if( ( cellok & ( 1ll << j ) ) == 0 ) subset &= ( ~( sud->grid[j][x] ) );
			}

			if( __popcnt64( subset ) != i + 1 ) continue;

			//remove other candidates from cells in subset
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				if( ( subset & ( 1ll << j ) ) != 0 ) {
					changed |= ( sud->grid[j][x] & ( ~( subset ) ) );
					sud->grid[j][x] &= subset;
				}
			}
			return changed != 0;
		}
	}


	return 0;
}
#else
int strategy13( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
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
		if( sud->cellvalue[i][x] == 0 && i != y ) {
			index[j++] = i;
		}
	}

	if( j <= SUDOKU_SUBSET_MIN ) return 0;

	//for defined subset sizes
	for( i = SUDOKU_SUBSET_MIN; i <= SUDOKU_SUBSET_MAX; i++ ) {
		Combinator_Initialize( &c, i, index, j );
		combination[i] = y;

		//for each available combination
		while( Combinator_GetNext( &c, combination ) == 0 ) {

			vinitl( subset, 0 );
			for( j = 0; j <= i; j++ ) {
				for( k = j + 1; k <= i; k++ ) {
					//combine all subsets
					//create validation mask
					vandl( sud->grid[combination[j]][x], sud->grid[combination[k]][x], buffer );
					if( vpopcntl( buffer ) != 0 ) {
						cellok[j] = 1;
						cellok[k] = 1;
						vsetl( subset, buffer );
						break;
					}
				}
			}

			//validate subset
			if( vpopcntl( cellok ) != i + 1 ) continue;

			for( j = 0; j < sud->length; j++ ) {
				if( cellok[j] == 0 ) vunsetl( subset, sud->grid[j][x] );
			}

			if( vpopcntl( subset ) != i + 1 ) continue;

			//remove other candidates from cells in subset
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				if( subset[j] != 0 ) {
					vnotl( subset, buffer );
					vandl( buffer, sud->grid[j][x], buffer );
					changed += vpopcntl( buffer );
					vandl( sud->grid[j][x], subset, sud->grid[j][x] );
				}
			}
			return changed != 0;
		}
	}


	return 0;
}
#endif
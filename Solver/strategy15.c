#include "strategies.h"

#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy15( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
	struct Combinator c;

	SudokuCell cellok;
	SudokuCell subset;
	SudokuCell changed;

	unsigned int i, j, k;
	unsigned int index[64] = { 0 };
	unsigned int combination[5] = { 0 };
	unsigned int curcell = BOXINDEX( sud, x, y );

	j = 0;

	for( i = 0; i < sud->length; i++ ) {
		if( *sud->cellboxvalue[y][x][i] == 0 && i != curcell ) {
			index[j++] = i;
		}
	}

	if( j <= 3 ) return 0;


	for( i = 2; i < 4; i++ ) {
		Combinator_Initialize( &c, i, index, j );
		combination[i] = curcell;

		while( Combinator_GetNext( &c, combination ) == 0 ) {
			subset = 0ll;

			for( j = 0; j <= i; j++ ) {
				for( k = j + 1; k <= i; k++ ) {
					if( *sud->cellbox[y][x][combination[j]] & *sud->cellbox[y][x][combination[k]] ) {
						cellok |= ( ( 1 << j ) | ( 1 << k ) );
						subset |= *sud->cellbox[y][x][combination[j]] & *sud->cellbox[y][x][combination[k]];
						break;
					}
				}
			}

			if( __popcnt64( cellok ) != i + 1 ) continue;

			for( j = 0; j < sud->length; j++ ) {
				if( ( cellok & ( 1ll << j ) ) == 0 ) subset &= ( ~( *sud->cellbox[y][x][j] ) );
			}

			if( __popcnt64( subset ) != i + 1 ) continue;

			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				if( ( subset & ( 1ll << j ) ) != 0 ) {
					changed |= ( *sud->cellbox[y][x][j] & ( ~( subset ) ) );
					*sud->cellbox[y][x][j] &= subset;
				}
			}
			return changed != 0;
		}
	}


	return 0;
}

#else
int strategy15( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
	struct Combinator c;

	unsigned int cellok[64];
	unsigned int subset[64];
	unsigned int buffer[64];
	unsigned int changed;

	unsigned int i, j, k;
	unsigned int index[64] = { 0 };
	unsigned int combination[5] = { 0 };
	unsigned int curcell = BOXINDEX( sud, x, y );

	j = 0;

	for( i = 0; i < sud->length; i++ ) {
		if( sud->cellboxvalue[y][x][i] == 0 && i != curcell ) {
			index[j++] = i;
		}
	}

	if( j <= 3 ) return 0;

	for( i = 2; i < 4; i++ ) {
		Combinator_Initialize( &c, i, index, j );
		combination[i] = curcell;

		while( Combinator_GetNext( &c, combination ) == 0 ) {

			vinitl( subset, 0 );
			for( j = 0; j <= i; j++ ) {
				for( k = j + 1; k <= i; k++ ) {
					vandl( *sud->cellbox[y][x][combination[j]], *sud->cellbox[y][x][combination[k]], buffer );
					if( vpopcntl( buffer ) != 0 ) {
						cellok[j] = 1;
						cellok[k] = 1;
						vsetl( subset, buffer );
						break;
					}
				}
			}

			if( vpopcntl( cellok ) != i + 1 ) continue;

			for( j = 0; j < sud->length; j++ ) {
				if( cellok[j] == 0 ) vunsetl( subset, *sud->cellbox[y][x][j] );
			}

			if( vpopcntl( subset ) != i + 1 ) continue;

			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				if( subset[j] != 0 ) {
					vnotl( subset, buffer );
					vandl( buffer, sud->cellboxvalue[y][x][j], buffer );
					changed += vpopcntl( buffer );

					vandl( sud->cellboxvalue[y][x][j], subset, sud->cellboxvalue[y][x][j] );
				}
			}
			return changed != 0;
		}
	}


	return 0;
}
#endif
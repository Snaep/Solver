#include "strategies.h"

//prüft ob ein Wert innerhalb einer Zeile
//nur in der aktuellen Zelle möglich ist
#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy1( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
	unsigned int i;
	SudokuCell row;

	row = 0;
	for( i = 0; i < sud->length; i++ ) {
		if( i != x ) {
			row |= sud->grid[y][i];
		}
	}

	//loop through candidates
	for( i = 0; i < sud->length; i++ ) {
		//wenn kandidat gefunden
		if( ( sud->grid[y][x] & ( 1ll << i ) ) != 0 ) {
			//wenn kandidat an keiner anderen Stelle in Reihe möglich ist
			//Setze Zellwert
			if( ( row & ( 1ll << i ) ) == 0 ) {
				Sudoku_SetCell( sud, x, y, i + 1 );
				return 1;
			}
		}
	}

	return 0;
}

#else
int strategy1( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
	unsigned int i, j, k;

	//loop through candidates
	for( i = 0; i < sud->length; i++ ) {

		//wenn kandidat gefunden
		if( sud->grid[y][x][i] != 0 ) {

			//wenn kandidat an keiner anderen Stelle in Reihe möglich ist
			//Setze Zellwert
			j = 1;
			for( k = 0; k < sud->length; k++ ) {
				if( y != k && sud->grid[k][x][i] != 0 ) {
					j = 0;
					break;
				}
			}
			if( j != 0 ) {
				Sudoku_SetCell( sud, x, y, i + 1 );
				return 1;
			}
		}
	}

	return 0;
}

#endif
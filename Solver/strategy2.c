#include "strategies.h"

//prüft ob ein Wert innerhalb einer Spalte
//nur in der aktuellen Zelle möglich ist
#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy2( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i;
	SudokuCell col;

	col = 0;
	for( i = 0; i < sud->length; i++ ) {
		if( i != y ) {
			col |= sud->grid[i][x];
		}
	}

	//loop through candidates
	for( i = 0; i < sud->length; i++ ) {

		//wenn kandidat gefunden
		if( ( sud->grid[y][x] & ( 1ll << i ) ) != 0 ) {

			//wenn kandidat an keiner anderen Stelle in Spalte möglich ist
			//Setze Zellwert
			if( ( col & ( 1ll << i ) ) == 0 ) {
				Sudoku_SetCell( sud, x, y, i + 1 );
				return 1;
			}
		}
	}

	return 0;
}

#else
int strategy2( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j, k;

	//loop through candidates
	for( i = 0; i < sud->length; i++ ) {

		//wenn kandidat gefunden
		if( sud->grid[y][x][i] != 0 ) {

			//wenn kandidat an keiner anderen Stelle in Reihe möglich ist
			//Setze Zellwert
			j = 1;
			for( k = 0; k < sud->length; k++ ) {
				if( x != k && sud->grid[y][k][i] != 0 ) {
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
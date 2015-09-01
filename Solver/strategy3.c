#include "strategies.h"

//prüft ob ein Wert innerhalb einer Box
//nur in der aktuellen Zelle möglich ist
#ifdef SUDOKU_CELLTYPE_BITVECTOR
int strategy3( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i;
	SudokuCell box;

	box = 0;
	//Erzeuge Bitvektor aus aktueller Box
	for( i = 0; i < sud->length; i++ ) {
		if( sud->cellbox[y][x][i] != &sud->grid[y][x] ) {
			box |= *sud->cellbox[y][x][i];
		}
	}

	//Laufe durch die Kandiaten 
	for( i = 0; i < sud->length; i++ ) {
		//Wenn Kandidat gefunden
		if( ( sud->grid[y][x] & ( 1ll << i ) ) != 0 ) {
			if( ( box & ( 1ll << i ) ) == 0 ) {
				Sudoku_SetCell( sud, x, y, i + 1 );
				return 1;
			}
		}
	}

	return 0;
}
#else
int strategy3( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j, k;

	//loop through candidates
	for( i = 0; i < sud->length; i++ ) {

		//wenn kandidat gefunden
		if( sud->grid[y][x][i] != 0 ) {

			//wenn kandidat an keiner anderen Stelle in Reihe möglich ist
			//Setze Zellwert
			j = 1;
			for( k = 0; k < sud->length; k++ ) {
				if( &sud->grid[y][x] != sud->cellbox[y][x][k] && ( *sud->cellbox[y][x][k] )[i] != 0 ) {
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
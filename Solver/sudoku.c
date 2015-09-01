#include "sudoku.h"
#include "defaults.h"
#include "vectorop.h"

unsigned char* ReadAllBytes( const wchar_t * file ) {
	FILE *fptr;
	long len;
	unsigned char* retv;

	if( _wfopen_s( &fptr, file, L"r" ) != 0 ) return NULL;

	//determinate file size
	fseek( fptr, 0, SEEK_END );
	len = ftell( fptr );

	//allocate and read complete file
	retv = ( unsigned char* )malloc( len * sizeof( unsigned char ) + sizeof( unsigned char ) );
	if( retv != NULL ) {
		fseek( fptr, 0, SEEK_SET );
		fread( retv, 1, len, fptr );
		retv[len] = L'\0';
	}

	//close file
	fclose( fptr );
	return retv;
}

int Sudoku_Allocate( struct Sudoku* sud ) {
	unsigned int i, j, k, cellindex;

	//allocate grid
	sud->grid = ( SudokuCell** )calloc( sud->length, sizeof( SudokuCell* ) );
	if( sud->grid == NULL )return -1;

	sud->cellvalue = ( int** )calloc( sud->length, sizeof( int* ) );
	if( sud->cellvalue == NULL )return -1;

#ifdef SUDOKU_CELLTYPE_BITVECTOR
	//allocate grid rows

	for( i = 0; i < sud->length; i++ ) {
		sud->grid[i] = ( SudokuCell* )malloc( sud->length * sizeof( SudokuCell ) );
		if( sud->grid[i] == NULL )return -1;
		//alle werte auf möglich setzen
		for( j = 0; j < sud->length; j++ ) sud->grid[i][j] = ( SudokuCell )( ( 1ll << sud->length ) - 1 );

		sud->cellvalue[i] = ( int* )calloc( sud->length, sizeof( int ) );
		if( sud->cellvalue[i] == NULL ) return -1;
	}

	//allocate contains helper
	for( i = 0; i < 3; i++ ) {
		sud->contains[i] = ( unsigned long long* ) calloc( sud->length, sizeof( unsigned long long ) );
		if( sud->contains[i] == NULL )return -1;
	}


	sud->cellbox = ( SudokuCell**** )calloc( sud->length, sizeof( SudokuCell*** ) );
	if( sud->cellbox == NULL )return -1;

	sud->cellboxvalue = ( unsigned int**** )calloc( sud->length, sizeof( unsigned int*** ) );
	if( sud->cellboxvalue == NULL )return -1;

	//allocate and set box helper
	for( i = 0; i < sud->length; i++ ) {
		sud->cellbox[i] = ( SudokuCell*** )calloc( sud->length, sizeof( SudokuCell** ) );
		if( sud->cellbox[i] == NULL )return -1;

		sud->cellboxvalue[i] = ( unsigned int*** )calloc( sud->length, sizeof( unsigned int** ) );
		if( sud->cellboxvalue[i] == NULL )return -1;

		for( j = 0; j < sud->length; j++ ) {
			//position (0,0) in box->allocate
			if( j % sud->length_of_box == 0 && i % sud->length_of_box == 0 ) {
				sud->cellbox[i][j] = ( SudokuCell** )calloc( sud->length, sizeof( SudokuCell* ) );
				if( sud->cellbox[i][j] == NULL )return -1;
				sud->cellboxvalue[i][j] = ( unsigned int** )calloc( sud->length, sizeof( unsigned int* ) );
				if( sud->cellboxvalue[i][j] == NULL )return -1;

				//position (0,x) in box -> copy ptr from above cell)
			} else if( j % sud->length_of_box == 0 ) {
				sud->cellbox[i][j] = sud->cellbox[i - 1][j];
				sud->cellboxvalue[i][j] = sud->cellboxvalue[i - 1][j];
			} else {
				sud->cellbox[i][j] = sud->cellbox[i][j - 1];
				sud->cellboxvalue[i][j] = sud->cellboxvalue[i][j - 1];
			}
		}
	}

#else

	//allocate grid rows
	for( i = 0; i < sud->length; i++ ) {
		sud->grid[i] = ( SudokuCell* )calloc( sizeof( SudokuCell ), sud->length );
		if( sud->grid[i] == NULL ) return -1;
		//alle werte auf möglich setzen
		for( j = 0; j < sud->length; j++ ) {
			sud->grid[i][j] = ( SudokuCell )malloc( sizeof( int ) * sud->length );
			if( sud->grid[i][j] == NULL ) return -1;
			for( k = 0; k < sud->length; k++ ) {
				sud->grid[i][j][k] = 1;
			}
		}

		sud->cellvalue[i] = ( int* )calloc( sud->length, sizeof( int ) );
	}

	//allocate contains helper
	for( i = 0; i < 3; i++ ) {
		sud->contains[i] = ( SudokuCell* )calloc( sud->length, sizeof( SudokuCell ) );
		if( sud->contains[i] == NULL ) return -1;
		for( j = 0; j < sud->length; j++ ) {
			sud->contains[i][j] = calloc( sizeof( unsigned int ), sud->length );
			if( sud->contains[i][j] == 0 ) return -1;
		}
	}

	sud->cellbox = ( SudokuCell**** )calloc( sud->length, sizeof( SudokuCell*** ) );
	if( sud->cellbox == NULL ) return -1;

	sud->cellboxvalue = ( unsigned int**** )calloc( sud->length, sizeof( unsigned int*** ) );
	if( sud->cellboxvalue == NULL ) return -1;

	//allocate and set box helper
	for( i = 0; i < sud->length; i++ ) {
		sud->cellbox[i] = ( SudokuCell*** )calloc( sud->length, sizeof( SudokuCell** ) );
		if( sud->cellbox[i] == NULL ) return -1;

		sud->cellboxvalue[i] = ( unsigned int*** )calloc( sud->length, sizeof( unsigned int** ) );
		if( sud->cellboxvalue[i] == NULL ) return -1;

		for( j = 0; j < sud->length; j++ ) {
			//position (0,0) in box-> allocate
			if( j % sud->length_of_box == 0 && i % sud->length_of_box == 0 ) {
				sud->cellbox[i][j] = ( SudokuCell** )calloc( sud->length, sizeof( SudokuCell* ) );
				if( sud->cellbox[i][j] == NULL ) return -1;
				sud->cellboxvalue[i][j] = ( unsigned int** )calloc( sud->length, sizeof( unsigned int* ) );
				if( sud->cellboxvalue[i][j] == NULL ) return -1;

				//position (0,x) in box -> copy ptr from above cell)
			} else if( j % sud->length_of_box == 0 ) {
				sud->cellbox[i][j] = sud->cellbox[i - 1][j];
				sud->cellboxvalue[i][j] = sud->cellboxvalue[i - 1][j];
			} else {
				sud->cellbox[i][j] = sud->cellbox[i][j - 1];
				sud->cellboxvalue[i][j] = sud->cellboxvalue[i][j - 1];
			}
		}
	}

#endif

	//i:y j:x
	//fill box pointer arrays with pointer to cells inside
	//the same box
	for( i = 0; i < sud->length; i += sud->length_of_box ) {
		for( j = 0; j < sud->length; j += sud->length_of_box ) {
			for( cellindex = 0; cellindex < sud->length; cellindex++ ) {
				sud->cellbox[i][j][cellindex] = &sud->grid[i + cellindex / sud->length_of_box][j + cellindex % sud->length_of_box];
				sud->cellboxvalue[i][j][cellindex] = &sud->cellvalue[i + cellindex / sud->length_of_box][j + cellindex % sud->length_of_box];
			}
		}
	}

	return 0;
}

int Sudoku_ParseFile( struct Sudoku* sud, const wchar_t* filepath, const wchar_t delimiter ) {
	unsigned char* file;
	unsigned int i, rowindex, cellvalue, cellindex;
	int retv;

	//read file
	file = ReadAllBytes( filepath );
	if( file == NULL ) return SUDOKUERROR_FILE;

	//clear struct
	memset( sud, 0, sizeof( struct Sudoku ) );
	retv = SUDOKUERROR_PARSER;

	//determine grid size by counting occurences of delimiter in first row
	for( i = 0; file[i] != '\n'; i++ ) {
		if( file[i] == delimiter ) sud->length++;
	}

	//minimum size of grid is 2x2 (1 2 \n3 4 \n)
	if( sud->length < 2 ) {
		retv = SUDOKUERROR_GRIDSIZE;
		goto END;

	}

	//determinate length of a box and validate square sudoku
	sud->length_of_box = ( int )( 0.5f + sqrt( sud->length ) );
	if( sud->length_of_box * sud->length_of_box != sud->length ) {
		return SUDOKUERROR_GRIDSIZE;
		goto END;
	}

	if( Sudoku_Allocate( sud ) != 0 ) {
		Sudoku_Free( sud );
		retv = SUDOKUERROR_OUTOFMEMORY;
		goto END;
	}

	cellvalue = 0;
	cellindex = 0;
	rowindex = 0;

	//parse grid
	for( i = 0; file[i] != '\0'; i++ ) {
		//if current tchar is delimiter, finish cell
		if( file[i] == delimiter ) {
			Sudoku_SetCell( sud, cellindex++, rowindex, cellvalue );
			if( cellindex == sud->length ) {
				cellindex = 0;
				//if no. of rows == no. of cols + 1 end parsing
				if( rowindex == sud->length ) break;
			}
			cellvalue = 0;
			//if current tchar is end of line increment rowindex
		} else if( file[i] == '\n' ) {
			rowindex++;
			//if current tchar is not ctrl tchar, parse as cell content
		} else if( file[i] == '\r' ) {
			continue;
		} else {
			cellvalue *= 10;
			cellvalue += file[i] - '0';
		}
	}

	retv = 0;
END:
	//free loaded file
	if( file != NULL ) free( file );
	return retv;
}

int Sudoku_SetCell( struct Sudoku* sud, unsigned int x, unsigned int y, unsigned int value ) {
	unsigned int i;

	if( value == 0 || value > sud->length || x >= sud->length || y >= sud->length ) return -1;

	//store value in grid
	sud->cellvalue[y][x] = value;

	//reduce value to shiftvalue (1ll << 0) == 1 
	value--;

#ifdef SUDOKU_CELLTYPE_BITVECTOR
	//set all other values to impossible
#pragma omp atomic
	sud->grid[y][x] = 0ll;

	//store value in contains
#pragma omp atomic
	sud->contains[NEIGHBOURHOOD_COL][x] |= ( 1ll << value );
#pragma omp atomic
	sud->contains[NEIGHBOURHOOD_ROW][y] |= ( 1ll << value );
#pragma omp atomic
	sud->contains[NEIGHBOURHOOD_BOX][BOXINDEX( sud, x, y )] |= ( 1ll << value );

	//remove other cells possibility for value
	//col / row
	for( i = 0; i < sud->length; i++ ) {
#pragma omp atomic
		sud->grid[y][i] &= ~( 1ll << value );
#pragma omp atomic
		sud->grid[i][x] &= ~( 1ll << value );
#pragma omp atomic
		*sud->cellbox[y][x][i] &= ~( 1ll << value );
	}
#else
#pragma omp critical "SetCell" 
	{
		//set all other values to impossible
		vinitl( sud->grid[y][x], 0 );

		//store value in contains
		sud->contains[NEIGHBOURHOOD_COL][x][value] = 1;
		sud->contains[NEIGHBOURHOOD_ROW][y][value] = 1;
		sud->contains[NEIGHBOURHOOD_BOX][BOXINDEX( sud, x, y )][value] = 1;

		//remove other cells possibility for value
		//col / row / box
		for( i = 0; i < sud->length; i++ ) {
			sud->grid[y][i][value] = 0;
			sud->grid[i][x][value] = 0;
			( *sud->cellbox[y][x][i] )[value] = 0;
		}
	}
#endif

	return 0;
}

void Sudoku_Print( struct Sudoku* sud ) {
	unsigned int i, j, k;
	printf( "\r\n" );
	//y - iteration
	for( i = 0; i < sud->length; i++ ) {
		//on box index, print box border
		if( i && i % sud->length_of_box == 0 ) {
			for( k = 0; k < CONSOLE_WIDTH; k++ ) printf( "-" );
			printf( "\r\n" );
		}
		//x - iteration
		for( j = 0; j < sud->length; j++ ) {
			//on box index, print box border
			if( j && j % sud->length_of_box == 0 ) printf( "|" );
			//if cell is set, print cellvalue
			if( sud->cellvalue[i][j] != 0 ) {
				if( sud->cellvalue[i][j] < 10 ) {
					printf( "  %i", sud->cellvalue[i][j] );
				} else {
					printf( " %i", sud->cellvalue[i][j] );
				}
			} else {
				//if cell is empty
				//fill space between cells
				//with ' '
				printf( "   " );
			}
		}
		printf( "\r\n" );
	}

	//print seperator
	printf( "\r\n" );
	for( k = 0; k < CONSOLE_WIDTH; k++ ) printf( "--" );
	printf( "\r\n\r\n" );
}

int Sudoku_ValidatePhase1( struct Sudoku* sud ) {
	unsigned int i, j, k;

#ifdef SUDOKU_CELLTYPE_BITVECTOR
	//iterate over all neighbourhoodtypes
	for( i = 0; i < 3; i++ ) {
		//iterate over all neighbourhoods of given type 
		for( j = 0; j < sud->length; j++ ) {
			//if a neighbourhood exists, that does not contain all candidates
			//there is a empty cell or a candidat occurs multiple times within 
			//that neighbourhood
			if( sud->contains[i][j] != ( ( 1 << sud->length ) - 1 ) ) return VALIDATION_PHASE1FAILED;
		}
	}
#else
	//iterate over all neighbourhoodtypes
	for( i = 0; i < 3; i++ ) {
		//iterate over all neighbourhoods of given type 
		for( j = 0; j < sud->length; j++ ) {
			for( k = 0; k < sud->length; k++ ) {
				//if a neighbourhood exists, that does not contain all candidates
				//there is a empty cell or a candidat occurs multiple times within 
				//that neighbourhood
				if( sud->contains[i][j][k] == 0 ) return VALIDATION_PHASE1FAILED;
			}
		}
	}
#endif
	return VALIDATION_SUCCESS;
}

int Sudoku_ValidatePhase2( struct Sudoku* sud ) {
	unsigned int i, j, k;

#ifdef SUDOKU_CELLTYPE_BITVECTOR	
	//loop through cells
	for( i = 0; i < sud->length; i++ ) {
		for( j = 0; j < sud->length; j++ ) {

			//if empty cell found
			//sulotion is invalid
			if( sud->cellvalue[i][j] == 0 ) {

				//iterate throug candidates
				for( k = 0; k < sud->length; k++ ) {
					//if candidat found
					//and candidat is presetn in no neighbourhood
					//the cell is empty
					if( ( ( sud->grid[i][j] & ( 1ll << k ) ) != 0 ) && ( (
						( ( sud->contains[NEIGHBOURHOOD_COL][j] & ( 1ll << k ) ) != 0 ) ||
						( ( sud->contains[NEIGHBOURHOOD_ROW][i] & ( 1ll << k ) ) != 0 ) ||
						( ( sud->contains[NEIGHBOURHOOD_BOX][BOXINDEX( sud, j, i )] & ( 1ll << k ) ) != 0 ) ) == 0 ) ) {
						return VALIDATION_EMPTYCELL;
					}
				}
				//if all possible candidates are present in any of the neighbourhoods
				//cell is conflicted
				return VALIDATION_CONFLICT;
			}
		}
	}
#else

	//loop through cells
	for( i = 0; i < sud->length; i++ ) {
		for( j = 0; j < sud->length; j++ ) {
			//if empty cell found
			//sulotion is invalid
			if( sud->cellvalue[i][j] == 0 ) {

				//iterate throug candidates
				for( k = 0; k < sud->length; k++ ) {
					//if candidat found
					//and candidat is presetn in no neighbourhood
					//the cell is empty
					if( ( sud->grid[i][j][k] != 0 ) && ( (
						( sud->contains[NEIGHBOURHOOD_COL][j][k] != 0 ) ||
						( sud->contains[NEIGHBOURHOOD_ROW][i][k] != 0 ) ||
						( sud->contains[NEIGHBOURHOOD_BOX][BOXINDEX( sud, j, i )][k] != 0 ) ) == 0 ) ) {
						return VALIDATION_EMPTYCELL;
					}
				}
				//if all possible candidates are present in any of the neighbourhoods
				//cell is conflicted
				return VALIDATION_CONFLICT;
			}
		}
	}
#endif
	return VALIDATION_SUCCESS;
}

int Sudoku_Validate( struct Sudoku* sud ) {
	//validate phase1
	//if phase1 validates sudoku
	//there is no need for phase 2
	if( Sudoku_ValidatePhase1( sud ) == VALIDATION_SUCCESS ) return VALIDATION_SUCCESS;

	//validate phase2
	return Sudoku_ValidatePhase2( sud );
}

void Sudoku_Free( struct Sudoku* sud ) {
	unsigned int i, j;
#ifdef SUDOKU_CELLTYPE_BITVECTOR
	//free sudoku if allocated
	if( sud != NULL ) {
		//free sudoku grid if allocated
		if( sud->grid != NULL ) {
			for( i = 0; i < sud->length; i++ ) {
				if( sud->grid[i] != NULL ) free( sud->grid[i] );
			}

			free( sud->grid );
		}
		//free cellvalue grid
		if( sud->cellvalue != NULL ) {
			for( i = 0; i < sud->length; i++ ) {
				if( sud->cellvalue[i] != NULL ) free( sud->cellvalue[i] );
			}
			free( sud->cellvalue );
		}

		//free cellbox
		if( sud->cellbox != NULL ) {
			for( i = 0; i < sud->length; i += sud->length_of_box ) {
				if( sud->cellbox[i] != NULL ) {
					for( j = 0; j < sud->length; j += sud->length_of_box ) {
						if( sud->cellbox[i][j] != NULL ) free( sud->cellbox[i][j] );
					}
					free( sud->cellbox[i] );
				}
			}
			free( sud->cellbox );
		}

		//free cellboxvalue
		if( sud->cellboxvalue != NULL ) {
			for( i = 0; i < sud->length; i += sud->length_of_box ) {
				if( sud->cellboxvalue[i] != NULL ) {
					for( j = 0; j < sud->length; j += sud->length_of_box ) {
						if( sud->cellboxvalue[i][j] != NULL ) free( sud->cellboxvalue[i][j] );
					}
					free( sud->cellboxvalue[i] );
				}
			}
			free( sud->cellboxvalue );
		}

		//free contains grid
		for( i = 0; i < 3; i++ ) {
			if( sud->contains[i] != NULL ) free( sud->contains[i] );
		}

		free( sud );
		sud = NULL;
	}
#else
	if( sud != NULL ) {
		//free sudoku grid if allocated
		if( sud->grid != NULL ) {
			for( i = 0; i < sud->length; i++ ) {
				if( sud->grid[i] != NULL ) {
					for( j = 0; j < sud->length; j++ ) if( sud->grid[i][j] ) free( sud->grid[i][j] );
					free( sud->grid[i] );
				}
			}

			free( sud->grid );
		}
		//free cellvalue grid
		if( sud->cellvalue != NULL ) {
			for( i = 0; i < sud->length; i++ ) {
				if( sud->cellvalue[i] != NULL ) free( sud->cellvalue[i] );
			}
			free( sud->cellvalue );
		}

		//free cellbox
		if( sud->cellbox != NULL ) {
			for( i = 0; i < sud->length; i += sud->length_of_box ) {
				if( sud->cellbox[i] != NULL ) {
					for( j = 0; j < sud->length; j += sud->length_of_box ) {
						if( sud->cellbox[i][j] != NULL ) free( sud->cellbox[i][j] );
					}
					free( sud->cellbox[i] );
				}
			}
			free( sud->cellbox );
		}

		//free cellboxvalue
		if( sud->cellboxvalue != NULL ) {
			for( i = 0; i < sud->length; i += sud->length_of_box ) {
				if( sud->cellboxvalue[i] != NULL ) {
					for( j = 0; j < sud->length; j += sud->length_of_box ) {
						if( sud->cellboxvalue[i][j] != NULL ) free( sud->cellboxvalue[i][j] );
					}
					free( sud->cellboxvalue[i] );
				}
			}
			free( sud->cellboxvalue );
		}

		//free contains grid
		for( i = 0; i < 3; i++ ) {
			if( sud->contains[i] != NULL ) {
				for( j = 0; j < sud->length; j++ ) {
					free( sud->contains[i][j] );
				}
				free( sud->contains[i] );
			}
		}

		free( sud );
		sud = NULL;
	}
#endif
}

int Sudoku_CreateCopy( struct Sudoku* src, struct Sudoku* dest ) {
	unsigned int i, j;

	//parameter validation
	if( src == NULL || dest == NULL ) return SUDOKUERROR_INVALIDARGS;

	dest->length = src->length;
	dest->length_of_box = src->length_of_box;

	//allocate new sudoku
	if( Sudoku_Allocate( dest ) != 0 ) return SUDOKUERROR_OUTOFMEMORY;

	//setze alle Zellen aus dem src Sudoku in dem src Sudoku
	for( i = 0; i < src->length; i++ ) {
		for( j = 0; j < src->length; j++ ) {
			Sudoku_SetCell( dest, i, j, src->cellvalue[j][i] );
		}
	}

	return 0;
}

int Sudoku_GetNextFreeCell( struct Sudoku* sud, unsigned int* x, unsigned int* y ) {
	unsigned int i, j;
	//parameter validation
	if( sud == NULL || x == NULL || y == NULL ) return SUDOKUERROR_INVALIDARGS;

	//loop through sudoku grid
	for( i = 0; i < sud->length; i++ ) {
		for( j = 0; j < sud->length; j++ ) {
			//if empty cell found
			if( sud->cellvalue[i][j] == 0 ) {
				//copy coordinates
				*x = j;
				*y = i;
				return 0;
			}
		}
	}

	return -1;
}

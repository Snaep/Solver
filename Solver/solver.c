#include "solver.h"
#include "strategies.h"
#include "debugwindow.h"

//------------------------------------------------
//available strategies
static const Strategy availableStrategies[] = {
	&strategy0,
	&strategy1,
	&strategy2,
	&strategy3,
	&strategy4,
	&strategy5,
	&strategy6,
	&strategy7,
	&strategy8,
	&strategy12,
	&strategy13,
	&strategy14,
	&strategy12,
	&strategy13,
	&strategy14,
	&strategy15
};

#define NUM_STRATEGIES (sizeof(availableStrategies) / sizeof(Strategy))
//------------------------------------------------

//available solver variants
static void Solve_Sequential( struct Solver* solver );
static void Solve_CellParallel( struct Solver* solver );
static void Solve_StrategyParallel( struct Solver* solver );
static void Solve_NeighbourParallel( struct Solver* solver );
static void Solve_NeighbourGridParallel( struct Solver* solver );

static const SolverFunq solverfunctions[] = {
	&Solve_Sequential,
	&Solve_CellParallel,
	&Solve_StrategyParallel,
	&Solve_NeighbourParallel,
	&Solve_NeighbourGridParallel
};
//------------------------------------------------

int Solver_Initialize( struct Solver* solver, int strategies, int mode ) {
	int i, j;

	//validate parameters
	if( solver == NULL || strategies == 0 ) return -2;
	
	//count selected strategies
	solver->ctStrategies = __popcnt( strategies );
	//return with error if no strategy selected
	if( solver->ctStrategies == 0 ) return -3;

	//allocate strategy vector
	solver->strategies = ( Strategy* )malloc( sizeof( Strategy ) * solver->ctStrategies );
	if( solver->strategies == NULL ) return -1;

	//create strategy pointer vector for solver
	j = 0;
	for( i = 0; i < NUM_STRATEGIES; i++ ) {
		if( strategies & ( 1 << i ) ) solver->strategies[j++] = availableStrategies[i];
	}

	//store solver function and number of threads
	solver->solverfunq = solverfunctions[mode & 0xFFFF];
	solver->ctThreads = ( ( mode >> 16 ) & 0xFFFF );

	return 0;
}

void Solve_Sequential( struct Solver* solver ) {
	unsigned int change;
	unsigned int y, x, e;
	unsigned int i;
	unsigned int highestrule;

	highestrule = 0;
	change = 0;

	for( e = 0; e < 5; e++ ) {
		//Loop trought rules
		for( i = 0; i < solver->ctStrategies; i++ ) {
			//loop trought x positions
			for( x = 0; x < solver->sudoku->length; x++ ) {
				//Loop trought y positions
				for( y = 0; y < solver->sudoku->length; y++ ) {
					//skip cell if cell is not empty
					if( solver->sudoku->cellvalue[y][x] != 0 ) continue;
					//execute strategy
					if( ( change = solver->strategies[i]( solver->sudoku, x, y ) ) != 0 ) {
#if (defined(_DEBUG) || defined(FORCEDEBUGMESSAGES)) && defined(PRINTCHANGEDCELL)
						wprintf_s( L"_DEBUG: gridloop\nchanged by rule%i\n(x:%iy:%i)=%i\n", i, x, y, solver->sudoku->cellvalue[y][x] );
#ifdef PRINTCHANGEDGRID
						Sudoku_Print( solver->sudoku );
#endif
#endif
#if defined(SUDOKU_UI)
						RefreshDebugWindow();
#endif
						//reset loops to restart with first cell
						highestrule = max( highestrule, i );
						x = ( unsigned int )-1;
						change = 0;
						i = 0;
						break;
					}
				}
			}
		}
	}

#if defined(_DEBUG) || defined(FORCEDEBUGMESSAGES)
	wprintf_s( L"_DEBUG: highest rule: %i\n", highestrule );
#endif
}

void Solve_CellParallel( struct Solver* solver ) {
	unsigned int change;
	unsigned int i, j;
	unsigned int highestrule;
	unsigned int length;

	highestrule = 0;

	length = solver->sudoku->length * solver->sudoku->length;

	//Loop trought rules
	for( i = 0; i < solver->ctStrategies; i++ ) {

#pragma omp parallel for
		for( j = 0; j < length; j++ ) {
			unsigned int y, x;

			x = j % solver->sudoku->length;
			y = j / solver->sudoku->length;

			//skip cell if cell is not empty
			if( solver->sudoku->cellvalue[y][x] != 0 ) continue;

			//execute strategy
			if( solver->strategies[i]( solver->sudoku, x, y ) != 0 ) {
#if (defined(_DEBUG) || defined(FORCEDEBUGMESSAGES)) && defined(PRINTCHANGEDCELL)
				wprintf_s( L"_DEBUG: gridloop\nchanged by rule%i\n(x:%iy:%i)=%i\n", i, x, y, solver->sudoku->cellvalue[y][x] );
#ifdef PRINTCHANGEDGRID
				Sudoku_Print( solver->sudoku );
#endif
#endif
#if defined(SUDOKU_UI)
				RefreshDebugWindow();
#endif

				//signal changed cell(s)
				change |= 1;
			}
		} //end of omp loop

		if( change != 0 ) {
			highestrule = max( highestrule, i );
			i = -1;
			change = 0;
		}
	}

#if defined(_DEBUG) || defined(FORCEDEBUGMESSAGES)
	wprintf_s( L"_DEBUG: highest rule: %i\n", highestrule );
#endif
}

void Solve_StrategyParallel( struct Solver* solver ) {
	unsigned int i;
	unsigned int change;

	do {
		change = 0;

#pragma omp parallel for
		//Loop trought rules
		for( i = 0; i < solver->ctStrategies; i++ ) {
			unsigned int y, x;
			//loop trought x positions
			for( x = 0; x < solver->sudoku->length; x++ ) {
				//Loop trought y positions
				for( y = 0; y < solver->sudoku->length; y++ ) {
					//skip cell if cell is not empty
					if( solver->sudoku->cellvalue[y][x] != 0 ) continue;
					//execute strategy
					if( solver->strategies[i]( solver->sudoku, x, y ) != 0 ) {
#if (defined(_DEBUG) || defined(FORCEDEBUGMESSAGES)) && defined(PRINTCHANGEDCELL)
						wprintf_s( L"_DEBUG: gridloop\nchanged by rule%i\n(x:%iy:%i)=%i\n", i, x, y, solver->sudoku->cellvalue[y][x] );
#ifdef PRINTCHANGEDGRID
						Sudoku_Print( solver->sudoku );
#endif
#endif
#if defined(SUDOKU_UI)
						RefreshDebugWindow();
#endif
						//signal changed cell(s)
						change |= 1;
					}
				}
			}
		} //end omp for


	} while( change != 0 );
}


void Solve_NeighbourParallel( struct Solver* solver ) {
	unsigned int y, x;
	unsigned int i, j;

	static Strategy strategies[6][4];
	unsigned int change;
	change = 0;

	for( i = 0; i < solver->ctStrategies; i++ ) {
		if( solver->strategies[i] == &strategy0 ) strategies[0][3] = &strategy0;
		else if( solver->strategies[i] == &strategy1 ) strategies[1][NEIGHBOURHOOD_ROW] = &strategy1;
		else if( solver->strategies[i] == &strategy2 ) strategies[1][NEIGHBOURHOOD_COL] = &strategy2;
		else if( solver->strategies[i] == &strategy3 ) strategies[1][NEIGHBOURHOOD_BOX] = &strategy3;
		else if( solver->strategies[i] == &strategy4 ) strategies[2][NEIGHBOURHOOD_ROW] = &strategy4;
		else if( solver->strategies[i] == &strategy5 ) strategies[2][NEIGHBOURHOOD_COL] = &strategy5;
		else if( solver->strategies[i] == &strategy6 ) strategies[2][NEIGHBOURHOOD_BOX] = &strategy6;
		else if( solver->strategies[i] == &strategy7 ) strategies[3][NEIGHBOURHOOD_ROW] = &strategy7;
		else if( solver->strategies[i] == &strategy8 ) strategies[3][NEIGHBOURHOOD_COL] = &strategy8;
		else if( solver->strategies[i] == &strategy9 ) strategies[3][NEIGHBOURHOOD_BOX] = &strategy9;
		else if( solver->strategies[i] == &strategy10 ) strategies[4][NEIGHBOURHOOD_ROW] = &strategy10;
		else if( solver->strategies[i] == &strategy11 ) strategies[4][NEIGHBOURHOOD_COL] = &strategy11;
		else if( solver->strategies[i] == &strategy12 ) strategies[4][NEIGHBOURHOOD_BOX] = &strategy12;
		else if( solver->strategies[i] == &strategy13 ) strategies[5][NEIGHBOURHOOD_ROW] = &strategy13;
		else if( solver->strategies[i] == &strategy14 ) strategies[5][NEIGHBOURHOOD_COL] = &strategy14;
		else if( solver->strategies[i] == &strategy15 ) strategies[5][NEIGHBOURHOOD_BOX] = &strategy15;
	}

	//Loop trough strategies
	for( i = 0; i < 6; i++ ) {
		change = 0;
		//loop trought x positions
		for( x = 0; x < solver->sudoku->length; x++ ) {
			//Loop trought y positions
			for( y = 0; y < solver->sudoku->length; y++ ) {
				//skip cell if cell is not empty
				if( solver->sudoku->cellvalue[y][x] != 0 ) continue;
				//execute strategy 0 first (because its neighbourhoodless)
				if( strategies[0][3] != NULL ) {
					if( strategies[0][3]( solver->sudoku, x, y ) != 0 ) {
#if (defined(_DEBUG) || defined(FORCEDEBUGMESSAGES)) && defined(PRINTCHANGEDCELL)
						wprintf_s( L"_DEBUG: gridloop\nchanged by rule%i\n(x:%iy:%i)=%i\n", i, x, y, solver->sudoku->cellvalue[y][x] );
#ifdef PRINTCHANGEDGRID
						Sudoku_Print( solver->sudoku );
#endif
#endif
#if defined(SUDOKU_UI)
						RefreshDebugWindow();
#endif
						change |= 1;
					}
				}

#pragma omp parallel for
				for( j = 0; j < 3; j++ ) {
					if( strategies[i][j] != NULL ) {

						//execute strategy
						if( strategies[i][j]( solver->sudoku, x, y ) != 0 ) {

#if (defined(_DEBUG) || defined(FORCEDEBUGMESSAGES)) && defined(PRINTCHANGEDCELL)
							wprintf_s( L"_DEBUG: gridloop\nchanged by rule%i\n(x:%iy:%i)=%i\n", i, x, y, solver->sudoku->cellvalue[y][x] );
#ifdef PRINTCHANGEDGRID
							Sudoku_Print( solver->sudoku );
#endif
#endif
#if defined(SUDOKU_UI)
							RefreshDebugWindow();
#endif

							//reset loops to restart with first cell
							x = ( unsigned int )-1;
							change |= 1;
							break;
						}
					}
				}
				if( x == ( unsigned int )-1 ) break;
			}
		} //end omp for

		if( change != 0 ) i = ( unsigned int )-1;
	}
}

void Solve_NeighbourGridParallel( struct Solver* solver ) {
	unsigned int i, j;
	static Strategy strategies[6][4];
	unsigned int change;
	change = 0;

	for( i = 0; i < solver->ctStrategies; i++ ) {
		if( solver->strategies[i] == &strategy0 ) strategies[0][3] = &strategy0;
		else if( solver->strategies[i] == &strategy1 ) strategies[1][NEIGHBOURHOOD_ROW] = &strategy1;
		else if( solver->strategies[i] == &strategy2 ) strategies[1][NEIGHBOURHOOD_COL] = &strategy2;
		else if( solver->strategies[i] == &strategy3 ) strategies[1][NEIGHBOURHOOD_BOX] = &strategy3;
		else if( solver->strategies[i] == &strategy4 ) strategies[2][NEIGHBOURHOOD_ROW] = &strategy4;
		else if( solver->strategies[i] == &strategy5 ) strategies[2][NEIGHBOURHOOD_COL] = &strategy5;
		else if( solver->strategies[i] == &strategy6 ) strategies[2][NEIGHBOURHOOD_BOX] = &strategy6;
		else if( solver->strategies[i] == &strategy7 ) strategies[3][NEIGHBOURHOOD_ROW] = &strategy7;
		else if( solver->strategies[i] == &strategy8 ) strategies[3][NEIGHBOURHOOD_COL] = &strategy8;
		else if( solver->strategies[i] == &strategy9 ) strategies[3][NEIGHBOURHOOD_BOX] = &strategy9;
		else if( solver->strategies[i] == &strategy10 ) strategies[4][NEIGHBOURHOOD_ROW] = &strategy10;
		else if( solver->strategies[i] == &strategy11 ) strategies[4][NEIGHBOURHOOD_COL] = &strategy11;
		else if( solver->strategies[i] == &strategy12 ) strategies[4][NEIGHBOURHOOD_BOX] = &strategy12;
		else if( solver->strategies[i] == &strategy13 ) strategies[5][NEIGHBOURHOOD_ROW] = &strategy13;
		else if( solver->strategies[i] == &strategy14 ) strategies[5][NEIGHBOURHOOD_COL] = &strategy14;
		else if( solver->strategies[i] == &strategy15 ) strategies[5][NEIGHBOURHOOD_BOX] = &strategy15;
	}

	//Loop trough strategies
	for( i = 0; i < 6; i++ ) {
		change = 0;
#pragma omp parallel for
		for( j = 0; j < 4; j++ ) {
			if( strategies[i][j] != NULL ) {
				unsigned int y, x;

				//loop trought x positions
				for( x = 0; x < solver->sudoku->length; x++ ) {
					//Loop trought y positions
					for( y = 0; y < solver->sudoku->length; y++ ) {
						//skip cell if cell is not empty
						if( solver->sudoku->cellvalue[y][x] != 0 ) continue;
						//execute strategy
						if( strategies[i][j]( solver->sudoku, x, y ) != 0 ) {
#if (defined(_DEBUG) || defined(FORCEDEBUGMESSAGES)) && defined(PRINTCHANGEDCELL)
							wprintf_s( L"_DEBUG: gridloop\nchanged by strategy%i\n(x:%iy:%i)=%i\n", i, x, y, solver->sudoku->cellvalue[y][x] );
#ifdef PRINTCHANGEDGRID
							Sudoku_Print( solver->sudoku );
#endif
#endif
#if defined(SUDOKU_UI)
							RefreshDebugWindow();
#endif
							//reset loops to restart with first cell
							x = ( unsigned int )-1;
							change |= 1;
							break;
						}
					}
				}
			}
		} //end omp for

		if( change != 0 ) i = ( unsigned int )-1;
	}
}


int Solver_Solve( struct Solver* solver, struct Sudoku* sudoku ) {
	//validate parameter
	if( solver == NULL || sudoku == NULL ) return -1;

	//store sudoku
	solver->sudoku = sudoku;

	//set number of threads
	if( solver->solverfunq != &Solve_Sequential ) {
		omp_set_num_threads( solver->ctThreads );
	}

	//start solver
	solver->solverfunq( solver );

	return 0;
}

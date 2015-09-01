#include "sudokustack.h"
#include "vectorop.h"
#include "debugwindow.h"

int SudokuStack_Initialize( struct SudokuStack* stack, struct Sudoku* original, unsigned int max_depth ) {
	int rc;
	unsigned int i;
	unsigned int x, y;
	SudokuCell candidates;

	stack->_stack = ( struct SudokuStackElement* )malloc( sizeof( struct SudokuStackElement ) * max_depth );
	if( stack->_stack == NULL ) return -1;

	stack->_ctSudokus = 1;
	stack->_maxSudokus = max_depth;


	if( ( rc = Sudoku_GetNextFreeCell( original, &x, &y ) ) != 0 ) return rc;

	candidates = original->grid[y][x];
	for( i = 0; i < original->length; i++ ) {
#ifdef SUDOKU_CELLTYPE_BITVECTOR
		if( ( candidates & ( 1LL << i ) ) != 0 ) {
#else
		if( candidates[i] != 0 ) {
#endif
			break;
		}
	}

	stack->_stack[0].x = x;
	stack->_stack[0].y = y;
	stack->_stack[0]._ctchanges = i;
	stack->_stack[0]._sud = original;
#ifdef SUDOKU_CELLTYPE_BITVECTOR
	stack->_stack[stack->_ctSudokus]._cellcandidates = candidates;
#else
	vcopy( stack->_stack[stack->_ctSudokus]._cellcandidates, candidates, original->length );
#endif

	return 0;
}
void SudokuStack_Free( struct SudokuStack* stack ) {
	int i;

	if( stack->_stack != NULL ) {
		for( i = stack->_ctSudokus - 1; i > 0; i-- ) Sudoku_Free( stack->_stack[i]._sud );
		free( stack->_stack );
		stack->_stack = NULL;
		stack->_ctSudokus = 0;
		stack->_maxSudokus = 0;
	}
}
int SudokuStack_GetNextModification( struct SudokuStack* stack, struct Sudoku** dest ) {
	int rc;
	unsigned int x, y, i;

	struct Sudoku* nextcopy = ( struct Sudoku* )malloc( sizeof( struct Sudoku ) );
	if( nextcopy == NULL ) return -1;
	if( stack->_ctSudokus == stack->_maxSudokus ) return SUDOKUSTACK_FULL;

	if( ( rc = Sudoku_CreateCopy( stack->_stack[stack->_ctSudokus - 1]._sud, nextcopy ) ) != 0 ) {
		free( nextcopy );
		*dest = NULL;
		return rc;
	}

	if( ( rc = Sudoku_GetNextFreeCell( nextcopy, &x, &y ) ) != 0 ) {
		Sudoku_Free( nextcopy );
		*dest = NULL;
		return rc;
	}

	SudokuCell candidates = nextcopy->grid[y][x];
	for( i = 0; i < nextcopy->length; i++ ) {
#ifdef SUDOKU_CELLTYPE_BITVECTOR
		if( ( candidates & ( 1LL << i ) ) != 0 ) {
#else
		if( candidates[i] != 0 ) {
#endif
			break;
		}
	}

	if( i++ == nextcopy->length ) {
		Sudoku_Free( nextcopy );
		return -3;
	}

#ifdef SUDOKU_CELLTYPE_BITVECTOR
	stack->_stack[stack->_ctSudokus]._cellcandidates = candidates;
#else
	vcopy( stack->_stack[stack->_ctSudokus]._cellcandidates, candidates, nextcopy->length );
#endif
	Sudoku_SetCell( nextcopy, x, y, i );

	stack->_stack[stack->_ctSudokus]._sud = nextcopy;
	stack->_stack[stack->_ctSudokus].x = x;
	stack->_stack[stack->_ctSudokus].y = y;
	stack->_stack[stack->_ctSudokus]._ctchanges = i;

	stack->_ctSudokus++;

	*dest = nextcopy;

	return 0;
}
int SudokuStack_GetNextSudoku( struct SudokuStack* stack, struct Sudoku** dest ) {
	int rc;
	unsigned int x, y, i;
	SudokuCell candidates;

	struct Sudoku* nextcopy = ( struct Sudoku* )malloc( sizeof( struct Sudoku ) );
	if( nextcopy == NULL ) return SUDOKUSTACK_OUTOFMEMORY;
	if( stack->_ctSudokus == 1 ) return SUDOKUSTACK_EMPTY;
	stack->_ctSudokus--;

	Sudoku_Free( stack->_stack[stack->_ctSudokus]._sud );

	if( ( rc = Sudoku_CreateCopy( stack->_stack[stack->_ctSudokus - 1]._sud, nextcopy ) ) != 0 ) {
		free( nextcopy );
		*dest = NULL;
		return rc;
	}

	candidates = stack->_stack[stack->_ctSudokus]._cellcandidates;
	for( i = stack->_stack[stack->_ctSudokus]._ctchanges; i < nextcopy->length; i++ ) {
#ifdef SUDOKU_CELLTYPE_BITVECTOR
		if( ( candidates & ( 1LL << i ) ) != 0 ) {
#else
		if( candidates[i] != 0 ) {
#endif
			break;
		}
	}

	if( i++ == nextcopy->length ) {
		Sudoku_Free( nextcopy );
		return SudokuStack_GetNextSudoku( stack, dest );
	}

	x = stack->_stack[stack->_ctSudokus].x;
	y = stack->_stack[stack->_ctSudokus].y;


#ifdef SUDOKU_CELLTYPE_BITVECTOR
	stack->_stack[stack->_ctSudokus]._cellcandidates = candidates;
#else
	vcopy( stack->_stack[stack->_ctSudokus]._cellcandidates, candidates, nextcopy->length );
#endif

	Sudoku_SetCell( nextcopy, x, y, i );

	stack->_stack[stack->_ctSudokus]._sud = nextcopy;
	stack->_stack[stack->_ctSudokus].x = x;
	stack->_stack[stack->_ctSudokus].y = y;
	stack->_stack[stack->_ctSudokus]._ctchanges = i;

	stack->_ctSudokus++;
	*dest = nextcopy;

	return 0;
}

struct Sudoku* SudokuStack_Peek( struct SudokuStack* stack ) {
	return stack->_stack[stack->_ctSudokus - 1]._sud;
}

unsigned int SudokuStack_GetCurrentDepth(struct SudokuStack* stack) {
	return stack->_ctSudokus;
}

#pragma once

#include "sudoku.h"

#define SUDOKUSTACK_OUTOFMEMORY -1
#define SUDOKUSTACK_EMPTY -2
#define SUDOKUSTACK_FULL -3

struct SudokuStackElement {
	int _ctchanges;
	unsigned int x, y;
#ifdef SUDOKU_CELLTYPE_BITVECTOR
	SudokuCell _cellcandidates;
#else
	unsigned int _cellcandidates[64];
#endif
	struct Sudoku* _sud;
};

struct SudokuStack {
	struct SudokuStackElement* _stack;
	unsigned int _ctSudokus, _maxSudokus;
};

int SudokuStack_Initialize( struct SudokuStack* stack, struct Sudoku* original, unsigned int max_depth );
void SudokuStack_Free( struct SudokuStack* stack );
int SudokuStack_GetNextModification( struct SudokuStack* stack, struct Sudoku** dest );
int SudokuStack_GetNextSudoku( struct SudokuStack* stack, struct Sudoku** dest );
struct Sudoku* SudokuStack_Peek( struct SudokuStack* stack );
unsigned int SudokuStack_GetCurrentDepth( struct SudokuStack* stack );
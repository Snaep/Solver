#pragma once
#include "sudoku.h"

#ifndef SUDOKU_CELLTYPE_BITVECTOR
//cout = c1 & c2
void vand( SudokuCell c1, SudokuCell c2, SudokuCell cout, int length );
//cout = c1 | c2
void vor( SudokuCell c1, SudokuCell c2, SudokuCell cout, int length );
//cout = c1 ^ c2
void vxor( SudokuCell c1, SudokuCell c2, SudokuCell cout, int length );
//cout = ~c1
void vnot( SudokuCell c1, SudokuCell cout, int length );
//returns no. of 1 elements
int vpopcnt( SudokuCell c1, int length );
//c == 0
int viszero( SudokuCell c, int length );
//c1 = c2
void vset( SudokuCell c1, SudokuCell c2, int length );
//c1 &= ~c2
void vunset( SudokuCell c1, SudokuCell c2, int length );
//sets all c1 'bits' to v
void vinit( SudokuCell c1, int v, int length );
//c1 = c2
void vcopy( SudokuCell c1, SudokuCell c2, int length );
//c1 == c2
int vequals( SudokuCell c1, SudokuCell c2, int length );

//cout = c1 & c2
#define vandl(c1, c2, cout) vand(c1, c2, cout, sud->length)
//cout = c1 | c2
#define vorl(c1, c2, cout) vor(c1, c2, cout, sud->length)
//cout = c1 ^ c2
#define vxorl(c1, c2, cout) vxor(c1, c2, cout, sud->length)
//cout = ~c1
#define vnotl(c1, cout) vnot(c1, cout, sud->length)
//returns no. of 1 elements
#define vpopcntl(c1) vpopcnt(c1, sud->length)
//c == 0
#define viszerol(c1) viszero(c1, sud->length)
//c1 |= c2
#define vsetl(c1, c2) vset(c1, c2, sud->length)
//c1 &= ~c2
#define vunsetl(c1, c2) vunset(c1, c2, sud->length)
//sets all c1 'bits' to v
#define vinitl( c1, v ) vinit(c1, v, sud->length)
//c1 = c2
#define vcopyl(c1, c2) vcopy(c1, c2, sud->length)
//c1 == c2
#define vequalsl(c1, c2) vequals(c1, c2, sud->length)
#endif
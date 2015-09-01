#pragma once

#include "sudoku.h"
#include "combinator.h"
#include "vectorop.h"

//--------------------------------------------------------------
//summary of all available strategies

//naked single
int strategy0( struct Sudoku* sud, unsigned int x, unsigned int y );


//hidden single row
int strategy1( struct Sudoku* sud, unsigned int x, unsigned int y );

//hidden single col
int strategy2( struct Sudoku* sud, unsigned int x, unsigned int y );

//hidden single box
int strategy3( struct Sudoku* sud, unsigned int x, unsigned int y );


//naked pair row
int strategy4( struct Sudoku* sud, unsigned int x, unsigned int y );

//naked pair col
int strategy5( struct Sudoku* sud, unsigned int x, unsigned int y );

//naked pair box
int strategy6( struct Sudoku* sud, unsigned int x, unsigned int y );


//hidden pair row
int strategy7( struct Sudoku* sud, unsigned int x, unsigned int y );

//hidden pair col
int strategy8( struct Sudoku* sud, unsigned int x, unsigned int y );

//hidden pair box
int strategy9( struct Sudoku* sud, unsigned int x, unsigned int y );


//naked subset row
int strategy10( struct Sudoku* sud, unsigned int x, unsigned int y );

//naked subset col
int strategy11( struct Sudoku* sud, unsigned int x, unsigned int y );

//naked subset box
int strategy12( struct Sudoku* sud, unsigned int x, unsigned int y );


//hidden subset row
int strategy13( struct Sudoku* sud, unsigned int x, unsigned int y );

//hidden subset col
int strategy14( struct Sudoku* sud, unsigned int x, unsigned int y );

//hidden subset box
int strategy15( struct Sudoku* sud, unsigned int x, unsigned int y );

//---------------------------------------------------------------

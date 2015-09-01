#pragma once

#include "defaults.h"
#include "sudoku.h"

#define PARMODE_SEQUENTIAL 0
#define PARMODE_CELL 1
#define PARMODE_STRATEGY 2
#define PARMODE_NEIGHBOUR 3

#define SOLVERERROR_OUTOFMEMORY -1
#define SOLVERERROR_INVALIDARGUMENT -2

typedef int( *Strategy )( struct Sudoku* sud, unsigned int x, unsigned int y );
typedef void( *SolverFunq )( struct Solver* solver );

struct Solver {
	unsigned int ctStrategies;
	Strategy* strategies;

	SolverFunq solverfunq;
	unsigned int ctThreads;

	struct Sudoku* sudoku;
};

///
///<summary>
///Die Funktion initialisiert eine Solver Instanz mit den gegebenen Parametern.
///</summary>
///<param name="solver" direction="in, out">
///Ein Zeiger auf die zu initialisierende Solver-Instanz 
///</param>
///<param name="sudoku" direction="in">
///Ein Zeiger auf ein Sudoku, welches mit den angegebenen Parametern gelöst werden soll
///</param>
///<param name="strategies" direction="in">
///Ein vier Byte langes Bitmuster, welches die zu verwendenden Strategien enthält
///</param>
///<param name="mode" direction="in">
///Die ersten 16 Bit beschreiben den zu verwendenden Parallelisierungsmodus, gültige Werte sind
///PARMODE_SEQUENTIAL
///PARMODE_CELL
///PARMODE_STRATEGY
///PARMODE_NEIGHBOUR
///die obersten 16 Bit beschreiben die Anzahl an Threads, die zur Lösung verwendet werden sollen.
///dieser Wert wird ignoriert, wenn PARMODE_SEQUENTIAL angegeben wurde.
///</param>
///<returns>
///Bei Erfolg wird 0 zurückgegeben.
///Bei Fehlschlag wird ein Wert kleiner 0  zurückgegeben.
///</returns>
///<remarks>
///Die Funktion kann fehlschlagen wenn
///Kein Speicher verfügbar ist( Rückgabewert: -1 )
///Ein oder mehrere Parameter ungültig sind( Rückgabewert: -2 )
///Ausgewählte nichtexistente Strategien werden ohne Fehlermeldung ignoriert.
///</remarks>
///
int Solver_Initialize( struct Solver* solver, int strategies, int mode );

///
///<summary>
///Die Funktion löst ein Sudoku mit den in der Solver-Instanz angegebenen Parametern
///</summary>
///<param name="solver" direction="in, out">
///Ein Zeiger auf eine Solver-Instanz, deren Sudoku gelöst werden soll 
///</param>
///<remarks>
///Die Funktion kann fehlschlagen wenn
///Kein Speicher verfügbar ist( Rückgabewert: -1 )
///Ein oder mehrere Parameter ungültig sind( Rückgabewert: -2 )
///Ausgewählte nichtexistente Strategien werden ohne Fehlermeldung ignoriert.
///</remarks>
///
int Solver_Solve( struct Solver* solver, struct Sudoku* sudoku );
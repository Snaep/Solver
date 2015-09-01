#pragma once

#include "sudoku.h"

#define SUDOKUSTACKERROR_OUTOFMEMORY -6
#define SUDOKUSTACKERROR_EMPTY -7
#define SUDOKUSTACKERROR_FULL -8
#define SUDOKUSTACKERROR_INVALIDARGS -9

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

///
///<summary>
///Initialisiert eine Sudoku-Stack Instanz
///</summary>
///<param name="stack" direction="in, out">
///Die zu initialisierende Sudokustack-Instanz
///</param>
///<param name="original" direction="in">
///Zeiger auf die Sudoku-Instanz, die als Ausgangsinstanz gew�hlt werden soll
///</param>
///<param name="max_depth" direction="in">
///Die maximale Tiefe, die erreicht werden kann.
///Muss gr��er gleich 2 sein.
///</param>
///<returns>
///Bei Erfolg wird 0 zur�ckgegeben.
///Bei Fehlschlag wird ein Wert kleiner 0 zur�ckgegeben.
///</returns>
///<remarks>
///Das Angegebene Originial-Sudoku wird nicht ver�ndert und nach Leeren des Stacks 
///durch Leerlaufen oder SudokuStack_Free nicht freigegeben.
///
///Ist eine der angegebenen Instanzen NULL, schl�gt die Funktion fehl.
///</remarks>
///
int SudokuStack_Initialize( struct SudokuStack* stack, struct Sudoku* original, unsigned int max_depth );

///
///<summary>
///Gibt alle von der Sudokustack-Instanz allokierten Ressourcen frei
///</summary>
///<param name="stack" direction="in, out">
///Zeiger auf eine Sudokustack-Instanz, deren Ressourcen freigegeben werden soll
///</param>
///<remarks>
///Das im Stack hinterlegte originial-Sudoku wird nicht freigegeben.
///</remarks>
///
void SudokuStack_Free( struct SudokuStack* stack );

///
///<summary>
///Erstellt eine Kopie des aktuellen L�sungsfortschritts,
///setzt einen Kandidaten an der n�chst m�glichen Stelle in der Kopie ein
///und kopiert den Zeiger zu der Kopie nach dest.
///</summary>
///<param name="stack" direction="in, out">
///Die Sudokustack-Instanz, deren Stack weiterverfolgt werden soll
///</param>
///<param name="dest" direction="out">
///Ein Zeiger auf die Adresse, auf die die neue Sudoku-Instanz kopiert werden soll
///</param>
///<returns>
///Bei Erfolg wird 0 zur�ckgegeben.
///Bei Fehlschlag wird ein Wert kleiner 0 zur�ckgegeben.
///</returns>
///<remarks>
///Die Returnwerte enthalten defines mit dem SUDOKUSTACKERROR_ oder SUDOKUERROR_
///</remarks>
///
int SudokuStack_GetNextModification( struct SudokuStack* stack, struct Sudoku** dest );

///
///<summary>
///L�scht den aktuellen L�sungsfortschritt
///Erstellt eine neue Kopie des vorherigen L�sungsfortschritts
///setzt den n�chsten Kandidaten in die erste freie Zelle ein.
///ist kein "n�chster" Kandidat vorhanden wird die
///Kopie verworfen und der Vorgang f�r den vorherigen L�sungsschritt ausgef�hrt
///</summary>
///<param name="stack" direction="in, out">
///Die Sudokustack-Instanz, deren Stack verfolgt werden soll
///</param>
///<param name="dest" direction="out">
///Ein Zeiger auf die Adresse, auf die die neue Sudoku-Instanz kopiert werden soll
///</param>
///<returns>
///Bei Erfolg wird 0 zur�ckgegeben.
///Bei Fehlschlag wird ein Wert kleiner 0 zur�ckgegeben.
///</returns>
///<remarks>
///Die Returnwerte enthalten defines mit dem SUDOKUSTACKERROR_ oder SUDOKUERROR_
///</remarks>
///
int SudokuStack_GetNextSudoku( struct SudokuStack* stack, struct Sudoku** dest );

///
///<summary>
///Gibt die Sudoku-Instanz des vorherigen L�sungsfortschrittes zur�ck
///ohne den hinterlegten stack zu beeinflussen / zu ver�ndern
///</summary>
///<param name="stack" direction="in">
///Die Sudokustack-Instanz, deren vorhergehender L�sungsfortschritt zur�ckgegeben werden soll
///</param>
///<returns>
///Bei Erfolg wird der Zeiger auf die Sudoku-Instanz des vorherigen L�sungsfortschritts zur�ckgegeben
///Bei Fehlschlag wird NULL zur�ckgegeben
///</returns>
///<remarks>
///Die Funktion schl�gt fehl, wenn stack NULL ist oder die Anzahl hinterlegter Sudokus 0 ist
///</remarks>
///
struct Sudoku* SudokuStack_Peek( struct SudokuStack* stack );

///
///<summary>
///Gibt die Anzahl der im Sudokustack hinterlegten Schritte zur�ck
///</summary>
///<param name="stack" direction="in">
///Die Sudokustack-Instanz, deren Anzahl enthaltener Schritte zur�ckgegeben werden soll
///Der Parameter darf nicht NULL sein
///</param>
///<returns>
///Anzahl der hinterlegten Schritte
///</returns>
///<remarks />
///
unsigned int SudokuStack_GetCurrentDepth( struct SudokuStack* stack );

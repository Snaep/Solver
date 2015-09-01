#pragma once

#include "defaults.h"

#define CELL_POSSIBLE 1
#define CELL_IMPOSSIBLE 0

#define NEIGHBOURHOOD_ROW 0
#define NEIGHBOURHOOD_COL 1
#define NEIGHBOURHOOD_BOX 2

#define SUDOKU_SUBSET_MIN 3
#define SUDOKU_SUBSET_MAX 4

#define BOXINDEX(sud, x, y) ( ( y / sud->length_of_box ) * sud->length_of_box + x / sud->length_of_box )

#define SUDOKUERROR_FILE (-1)
#define SUDOKUERROR_GRIDSIZE (-2)
#define SUDOKUERROR_PARSER (-3)
#define SUDOKUERROR_OUTOFMEMORY (-4)
#define SUDOKUERROR_INVALIDARGS (-5)

#define VALIDATION_SUCCESS 0
#define VALIDATION_EMPTYCELL -1
#define VALIDATION_CONFLICT -2
#define VALIDATION_PHASE1FAILED -3


#ifdef SUDOKU_CELLTYPE_BITVECTOR
typedef unsigned long long SudokuCell;
#else
typedef unsigned int* SudokuCell;
#endif

struct Sudoku {

	SudokuCell** grid;
	int** cellvalue;

	SudokuCell**** cellbox;
	unsigned int**** cellboxvalue;

	unsigned int length;
	unsigned int length_of_box;

	SudokuCell* contains[3];
};


///
///<summary>
///Die Funktion Sudoku_ParseFile dient zur Initialisierung einer Sudoku-Instanz aus einer Datei.
///</summary>
///<param name="dest" direction="in, out">
///Zeiger auf zu initialisierende Sudoku-Struktur
///</param>
///<param name="file" direction="in">
///0-terminierte Zeichenkette, welche dem absoluten oder relativen Pfad zur Datei entspricht.
///</param>
///<param name="delimiter" direction="in">
///Trennzeichen, welches zur Unterscheidung der Zellen benötigt wird.
///</param>
///<returns>
///Erfolg: 0
///Fehlschlag: <0. Siehe defines mit Präfix "SUDOKUERROR_"
///</returns>
///<remarks>
///Die Zeilen eines einzulesenden Sudokus müssen alle die gleiche Anzahl an Trennzeichen enthalten, 
///da die Größe des Sudokus anhand der Vorkommnisse des Trennzeichens in der ersten Zeile bestimmt wird.
///Jede Zelle muss mit einem Trennzeichen abgeschlossen werden.Stehen zwei Trennzeichen hintereinander
///wird die entsprechende Zelle als leere Zelle angenommen.Alternativ kann eine leere Zelle durch Angabe einer 0 beschrieben werden.
///Jede Zeile muss mit einem einfachen Zeilenumbruch( '\n‘ ) abgeschlossen werden.
///Als Kodierung der Zeichen wird Unicode verwendet.
///</remarks>
///
int Sudoku_ParseFile( struct Sudoku* dest, const wchar_t* file, const wchar_t delimiter );

///
///<summary>
///Die Funktion dient zum Setzen einer Zelle auf einen angegebenen Wert.
///Zugehörige Strukturmember der Sudoku - Instanz werden entsprechend aktualisiert.
///</summary>
///<param name="sud" direction="in, out">
///Zeiger auf Sudoku-Struktur, in der eine Zelle gesetzt werden soll
///</param>
///<param name="x" direction="in">
///X-Koordinate der zu setzenden Zelle
///die X-Koordinate muss kleiner als die Dimension des Sudokus sein.
///</param>
///<param name="y" direction="in">
///Y-Koordinate der zu setzenden Zelle
///die X-Koordinate muss kleiner als die Dimension des Sudokus sein.
///</param>
///<param name="value" direction="in">
///Wert der in die Zelle gesetzt werden soll
///der Wert muss kleiner oder gleich der Dimension des Sudokus und nicht 0 sein
///</param>
///<returns>
///Erfolg: 0
///Fehlschlag: -1
///</returns>
///<remarks>
///Das Define SUDOKU_CELLTYPE_BITVEKTOR ändert die
///Zellzuweisung der Funktion. Ist das Define unbekannt wird SudokuCell als Integervektor interpretiert.
///</remarks>
///
int Sudoku_SetCell( struct Sudoku* sud, unsigned int x, unsigned int y, unsigned int value );

///
///<summary>
///Die Funktion gibt die Zellwerte des Sudokus auf der Konsole aus
///</summary>
///<param name="sud" direction="in">
///Das Sudoku, dessen Zellwerte auf der Konsole angezeigt werden soll
///</param>
///<remarks>
///Die Zeilenbreite der Konsole sollte zur Darstellbarkeit größer als die Breite des Sudokus sein
///</remarks>
///
void Sudoku_Print( struct Sudoku* sud );

///
///<summary>
///Die Funktion prüft ein Sudoku auf Vollständigkeit und Korrektheit
///</summary>
///<param name="sud" direction="in">
///Das Sudoku, welches auf Vollständigkeit und Korrektheit überprüft werden soll
///</param>
///<returns>
///Ist das Sudoku vollständig und widerspruchsfrei wird 0 zurückgegeben, 
///ansonsten - 1.
///</returns>
///<remarks />
///
int Sudoku_ValidatePhase1( struct Sudoku* sud );

///
///<summary>
///Die Funktion prüft ein Sudoku auf Vollständigkeit und gibt die Art
///der Vollständigskeitsverlezung zurück
///</summary>
///<param name="sud" direction="in">
///Das Sudoku, welches auf Vollständigkeit überprüft werden soll
///</param>
///<returns>
///Ist das Sudoku vollständig wird 0 zurückgegeben, 
///ansonsten ein Wert kleiner 0 definiert als
///VALIDATION_EMPTYCELL //Mindestens eine Zelle ist frei, könnte jedoch konfliktfrei gesetzt werden
///VALIDATION_CONFLICT //Mindestens eine Zelle ist frei, kann jedoch nicht konfliktfrei gesetzt werden
///</returns>
///<remarks />
///
int Sudoku_ValidatePhase2( struct Sudoku* sud );
///
///<summary>
///Die Funktion prüft ein Sudoku auf Vollständigkeit und Korrektheit
///</summary>
///<param name="sud" direction="in">
///Das Sudoku, welches auf Vollständigkeit und Korrektheit überprüft werden soll
///</param>
///<returns>
///Ist das Sudoku vollständig und widerspruchsfrei wird 0 zurückgegeben, 
///ansonsten - 1.
///</returns>
///<remarks>
///Die Funktion führt eine vollständige 
///2-Phasen Validation durch
///</remarks>
int Sudoku_Validate( struct Sudoku* sud );

///
///<summary>
///Die Funktion gibt den Speicher des angegebenen Sudokus frei
///</summary>
///<param name="sud" direction="in">
///Das Sudoku, dessen allokierter Speicher freigegeben werden soll
///</param>
///<remarks />
///
void Sudoku_Free( struct Sudoku* sud );

///
///<summary>
///Erzeugt eine Kopie einer Sudoku-Instanz
///</summary>
///<param name="src" direction="in">
///Das Sudoku, welches kopiert werden soll
///</param>
///<param name="src" direction="in, out">
///Das Sudoku, dessen Zeiger überschrieben werden sollen
///Allokierter Speicher des Sudokus wird nicht freigegeben.
///</param>
///<returns>
///Erfolgreich: 0
///Bei Fehlschlag wird ein Wert kleiner 0 zurückgegeben
///<remarks />
///
int Sudoku_CreateCopy( struct Sudoku* src, struct Sudoku* dest );

///
///<summary>
///Sucht im angegebenen Sudoku nach der ersten freien Zelle
///und kopiert die Zellkoordinaten an die Adresse,
///welche durch die x und y Parameter angegeben wurde.
///</summary>
///<param name="sud" direction="in">
///Das Sudoku, welches untersucht werden soll
///</param>
///<param name="x" direction="out">
///Zeiger auf eine int-variable, die die x-Koordinate
///erhalten soll
///</param>
///<param name="y" direction="out">
///Zeiger auf eine int-variable, die die y-Koordinate
///erhalten soll
///</param>
///<returns>
///Erfolgreich: 0
///Bei Fehlschlag wird ein Wert kleiner 0 zurückgegeben
///<remarks>
///Die Feunktion schlägt fehl, 
///</remarks>
///
int Sudoku_GetNextFreeCell( struct Sudoku* sud, unsigned int* x, unsigned int* y );

#pragma once

#include "defaults.h"

struct Stopwatch {
	LARGE_INTEGER si, li;
	double fq;
};

///
///<summary>
///Startet eine angegebene Stopwatch-Instanz
///</summary>
///<param name="timer" direction="in, out">
///Die zu startende Sudoku-Instanz
///</param>
///<remarks>
///Die Stopwatch verwendet Performancecounter.
///Weitere Informationen zu dem Thema Performancecounter unter Windows sind unter
///https://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx
///https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx
///verfügbar. 
///
///Die Performancecounter-Frequenz wird beim Starten ermittelt und bei GetTime verwendet.
///</remarks>
///
void Stopwatch_Start( struct Stopwatch* timer );

///
///<summary>
///Misst die Zeit, die seit der Initialisierung vergangen ist.
///</summary>
///<param name="timer" direction="in, out">
///Stopwatch-Instanz, dessen Zeitmessung verwendet werden soll.
///</param>
///<returns>
///vergangene Zeit in Performance-Ticks
///</returns>
///<remarks>
///Durch Angabe des Defines STOPWATCH_MODE_SECONDS kann die Ausgabe
///auf die Anzahl Sekunden, die vergangen sind umgestellt werden.
///Die Rückgabe der Sekunden erfolgt als Gleitkommazahl.
///Die Zur Berechnung der vergangenen Sekunden verwendete Performancecounter-Frequenz
///wird zum Startzeitpunkt bestimmt.
///Ist die Performancecounter-Frequenz auf dem Testsystem nicht konstant kann das Ergebnis verfälscht werden.
///In dem Fall sollte die Zeitmessung in Performancecounter-Ticks ermittelt werden.
///</remarks>
///
#ifdef STOPWATCH_MODE_SECONDS
double Stopwatch_GetTime( struct Stopwatch* timer );
#else
unsigned long long Stopwatch_GetTime( struct Stopwatch* timer );
#endif

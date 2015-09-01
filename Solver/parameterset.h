#pragma once
#include "defaults.h"

#define PARAMERROR_NOSTRATEGIES (-1)
#define PARAMERROR_NOTHREADCOUNT (-2)
#define PARAMERROR_NOFILE (-3)
#define PARAMERROR_INVALIDARG (-4)

#define PARAMWARNING_NODELIMITER (1)


struct ParameterSet {

	//Bitmuster für bis zu 32 verschiedene Strategien
	unsigned int strategies;

	//Anzahl Threads | Art des Lösungsvorgangs
	//    16Bit      |         16Bit
	unsigned int solvertype;

	//Trennzeichen zwischen Zeichen im Sudoku
	//muss sich nach jedem Zeichen befinden
	wchar_t delimiter;

	//gibt an ob die Zeit für den Lösungsvorgang gestoppt werden soll
	unsigned int timer;

	//Dateipfad zum Sudoku
	wchar_t filepath[64];
};

///
///<summary>
///interpretiert die Parameter aus dem argv
///und führt eifnache Überprüfung aus
///</summary>
///<param name="set" direction="in, out">
///ParameterSet Instanz, die die Ergebnisse der interpretierten Argumente erhalten soll
///</param>
///<param name="argv" direction="in">
///Kommandozeilen-Array welches an das Programm übergeben wurde
///und Interpretiert werden soll
///</param>
///<returns>
///Bei Erfolg wird 0 zurückgegeben
///Bei Fehlschlag wird ein Wert kleiner 0 zurückgegeben
///Mögliche Fehlerwerte sind:
///PARAMERROR_NOFILE (Die Sudoku-Datei wurde nicht angegeben oder es kann nicht darauf zugegriffen werden)
///PARAMERROR_NOTHREADCOUNT (Als Parallelisierungsart wurde eine Multithreaded Variante ausgewählte, jedoch die Anzahl der zu verwendenden Threads ist 0 oder nicht angegeben)
///PARAMERROR_NOSTRATEGIES (Keine Strategie wurde zum Lösen ausgewählt)
///
///Wird eine Warnung erzeugt, wird ein Wert größer 0 zurückgegeben
///Mögliche Warnungen sind:
///PARAMWARNING_NODELIMITER (Es wurde kein Trennzeichen zum Parsen des Sudokus angegeben. Default ' ' wird verwendet)
///
///</returns>
///<remarks>
///Kommandozeilenargumente können mit
///dem define FAKE_ARGV in defaults.h simuliert werden.
///ist FAKE_ARGV defininiert, wird der argv Parameter ignoriert.
///
///Mögliche Parameter:
///-st[0-9]{1-2}  Aktivierung der Strategie
///-par=[0-9] Auswahl der zu verwendenden Lösungsvariante
///  0: Sequentiell
///  1: Zellparallelisierung
///  2: Strategieparallelisierung
///  3: Nachbarschaftsparallelisierung
///  4: Nachbarschafts-Gridparallelisierung
///-time aktiviert die Zeitmessung
///-del=[wchar_t] Angabe des Trennzeichens für das ausgewählte Sudoku
///-sud={PATH}.sud relativer oder absoluter Pfad zum zu lösenden Sudoku. Der Pfad darf keine Leerzeichen enthalten
///-tc=[0-9]{1-2} Angabe der zu verwendenden Threads. Bei Angabe von par=0 wird der Parameter tc ignoriert.
///
///ungültige Parameter werden ignoriert / können undefiniertes Verhalten hervorrufen
///</remarks>
///
int ParameterSet_Parse( struct ParameterSet* set, wchar_t** argv );

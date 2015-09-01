#pragma once
#include "defaults.h"

#define PARAMERROR_NOSTRATEGIES (-1)
#define PARAMERROR_NOTHREADCOUNT (-2)
#define PARAMERROR_NOFILE (-3)
#define PARAMERROR_INVALIDARG (-4)

#define PARAMWARNING_NODELIMITER (1)


struct ParameterSet {

	//Bitmuster f�r bis zu 32 verschiedene Strategien
	unsigned int strategies;

	//Anzahl Threads | Art des L�sungsvorgangs
	//    16Bit      |         16Bit
	unsigned int solvertype;

	//Trennzeichen zwischen Zeichen im Sudoku
	//muss sich nach jedem Zeichen befinden
	wchar_t delimiter;

	//gibt an ob die Zeit f�r den L�sungsvorgang gestoppt werden soll
	unsigned int timer;

	//Dateipfad zum Sudoku
	wchar_t filepath[64];
};

///
///<summary>
///interpretiert die Parameter aus dem argv
///und f�hrt eifnache �berpr�fung aus
///</summary>
///<param name="set" direction="in, out">
///ParameterSet Instanz, die die Ergebnisse der interpretierten Argumente erhalten soll
///</param>
///<param name="argv" direction="in">
///Kommandozeilen-Array welches an das Programm �bergeben wurde
///und Interpretiert werden soll
///</param>
///<returns>
///Bei Erfolg wird 0 zur�ckgegeben
///Bei Fehlschlag wird ein Wert kleiner 0 zur�ckgegeben
///M�gliche Fehlerwerte sind:
///PARAMERROR_NOFILE (Die Sudoku-Datei wurde nicht angegeben oder es kann nicht darauf zugegriffen werden)
///PARAMERROR_NOTHREADCOUNT (Als Parallelisierungsart wurde eine Multithreaded Variante ausgew�hlte, jedoch die Anzahl der zu verwendenden Threads ist 0 oder nicht angegeben)
///PARAMERROR_NOSTRATEGIES (Keine Strategie wurde zum L�sen ausgew�hlt)
///
///Wird eine Warnung erzeugt, wird ein Wert gr��er 0 zur�ckgegeben
///M�gliche Warnungen sind:
///PARAMWARNING_NODELIMITER (Es wurde kein Trennzeichen zum Parsen des Sudokus angegeben. Default ' ' wird verwendet)
///
///</returns>
///<remarks>
///Kommandozeilenargumente k�nnen mit
///dem define FAKE_ARGV in defaults.h simuliert werden.
///ist FAKE_ARGV defininiert, wird der argv Parameter ignoriert.
///
///M�gliche Parameter:
///-st[0-9]{1-2}  Aktivierung der Strategie
///-par=[0-9] Auswahl der zu verwendenden L�sungsvariante
///  0: Sequentiell
///  1: Zellparallelisierung
///  2: Strategieparallelisierung
///  3: Nachbarschaftsparallelisierung
///  4: Nachbarschafts-Gridparallelisierung
///-time aktiviert die Zeitmessung
///-del=[wchar_t] Angabe des Trennzeichens f�r das ausgew�hlte Sudoku
///-sud={PATH}.sud relativer oder absoluter Pfad zum zu l�senden Sudoku. Der Pfad darf keine Leerzeichen enthalten
///-tc=[0-9]{1-2} Angabe der zu verwendenden Threads. Bei Angabe von par=0 wird der Parameter tc ignoriert.
///
///ung�ltige Parameter werden ignoriert / k�nnen undefiniertes Verhalten hervorrufen
///</remarks>
///
int ParameterSet_Parse( struct ParameterSet* set, wchar_t** argv );

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

	//Art des Lösungsvorgangs
	//Sequentiell
	//Parallel Zelle
	//Parallel Entity ...
	unsigned int solvertype;

	//Trennzeichen zwischen Zeichen im Sudoku
	//muss sich nach jedem Zeichen befinden
	wchar_t delimiter;

	//gibt an ob die Zeit für den Lösungsvorgang gestoppt werden soll
	unsigned int timer;

	//Dateipfad zum Sudoku
	wchar_t filepath[64];
};

//parst die Parameter, die im argv array übergeben werden
//prüft Parametersatz auf vollständigkeit
//argv muss mit NULL abgeschlossen werden
//return:
//0: success
//1: kein delimiter -> default zu L' '
//-1: keine strategien
//-2: kein solvertype
//-3: keine datei angegeben
int ParameterSet_Parse( struct ParameterSet* dest, wchar_t** argv );
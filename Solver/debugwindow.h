#pragma once
#include "defaults.h"

//quick and dirty
//only used for debug purposes
//may or may not work
//define SUDOKU_UI in defaults.h to enable DebugWindow

//by default each refresh wait for user input to display changes
//this mode can be changed to a 50ms delay by defining SUDOKU_UI_NOBLOCK in defaults.h

DWORD WINAPI CreateDebugWindow( struct Sudoku* sudoku );
void CloseDebugWindow();

void RefreshDebugWindow();
void ChangeDebugWindow( struct Sudoku* sud );

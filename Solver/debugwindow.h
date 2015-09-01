#pragma once
#include "defaults.h"

void RefreshDebugWindow();
void CloseDebugWindow();
DWORD WINAPI CreateDebugWindow( struct Sudoku* sudoku );
void ChangeDebugWindow( struct Sudoku* sud );
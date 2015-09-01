#pragma once


#define UNICODE
#define _UNICODE

#if !defined(_WIN32) && !defined(_WIN64) 
#error solution limited to windows x86 or x64
#endif

#include <tchar.h>
#include <Windows.h>

#include <stdlib.h>
#include <stdio.h>

#include <intrin.h>
#include <omp.h>

#include <math.h>

//define to simulate argv
#define FAKE_ARGV { L"-sud=\"Sudokus\\4\\medium\\med02.sud\"", L"-tc=1", L"-par=0", L"-time", L"-st0", L"-st1", L"-st2", L"-st3", L"-st4", L"-st5", L"-st6", L"-st7", L"-st8", L"-st9", L"-st10", L"-st11", L"-st12", L"-st13", L"-st14", L"-st15" }

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//please note that every enabled output to the console
//or the ui invalidates the measured timings.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//define to print changes made to grid by strategy
#define PRINTCHANGEDCELL

//define to print grid after every change
//#define PRINTCHANGEDGRID

//define to print debug messages
//#define FORCEDEBUGMESSAGES

//define to enable debug window
//#define SUDOKU_UI
//define to remove step confirmation for ui refresh
//#define SUDOKU_UI_NOBLOCK

//define this to use bit parallelized solver
//#define SUDOKU_CELLTYPE_BITVECTOR

//define to set the timing mode to seconds
//if undefined, PerformanceCounter output will be printed
#define STOPWATCH_MODE_SECONDS

//define to execute solver multiple times
//disabled debug output (define FORCEDEBUGMESSAGES to reenable)
//#define MULTIRUN 2

//define to enable backtracking layer
#define TRY_AND_ERROR
//defines maximum depth of try and error
#define TRY_AND_ERROR_DEPTH 64
//defines maximum number of try and error steps
#define TRY_AND_ERROR_MAXSTEPCOUNT 100

//width of the console buffer
#define CONSOLE_WIDTH 80

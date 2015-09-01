#include "defaults.h"
#include "parameterset.h"
#include "solver.h"
#include "sudoku.h"
#include "stopwatch.h"
#include "debugwindow.h"
#include "sudokustack.h"

#ifdef STOPWATCH_MODE_SECONDS
static double timesum;
#else
static unsigned long long timesum;
#endif

//execute main loop multiple time
#if defined( MULTIRUN ) && MULTIRUN > 1
//create prototype for main programm
int run( int arc, wchar_t* argv[] );
//entry point
int wmain( int argc, wchar_t* argv[] ) {
	int i;
	int rc;

	timesum = 0.0;

	//execute main loop multiple times
	//as defined in MULTIRUN
	for( i = 0; i < MULTIRUN; i++ ) {
		rc = run( argc, argv );
		wprintf_s( L"\nsolver returned: %i\n", rc );
	}

	//print time to solve sudoku in ms
	timesum *= 1000;
	wprintf_s( L"Time avg: (%i runs): %.3fms", i, ( timesum / i ) );
	getchar();
	return rc;
}
//begin run function
int run( int argc, wchar_t* argv[] ) {
#elif defined _DEBUG
//create prototype for main programm
int run( int arc, wchar_t* argv[] );
//entry point
int wmain( int argc, wchar_t* argv[] ) {
	int rc;
	//execute solver single time
	rc = run( argc, argv );
	wprintf_s( L"\nsolver returned: %i\n", rc );
#ifdef STOPWATCH_MODE_SECONDS
	wprintf_s( L"time run: %.3fms\n", timesum * 1000 );
#else
	wprintf_s( L"time run: %I64d\n", timesum );
#endif
	//wait for confimation
	getchar();
	return rc;
}
//begin run function
int run( int argc, wchar_t* argv[] ) {
#else
//release mode
//only wmain
//no output overhead
int wmain( int argc, wchar_t* argv[] ) {
#endif
	struct ParameterSet params;
	struct Solver solver;
	struct Sudoku sudoku, *sudoku_current;
	struct Stopwatch stopwatch;
	int rc;
#ifdef TRY_AND_ERROR
	struct SudokuStack sudokustack;
	int continue_try_and_error;
	unsigned int ctTryAndErrorSteps;
#ifdef STOPWATCH_MODE_SECONDS
	double timerun_added;
#else 
	unsigned long long timerun_added;
#endif
#endif

#ifdef STOPWATCH_MODE_SECONDS
	double timerun;
#else 
	unsigned long long timerun;
#endif

	//parse command line arguments
	//command line can be faked by using 
	//the define FAKE_ARGV in defaults.h
	switch( ParameterSet_Parse( &params, argv ) ) {
	case PARAMERROR_NOFILE:
		wprintf_s( L"E: no sudoku file specified\n" );
		return EXIT_FAILURE;
	case PARAMERROR_NOTHREADCOUNT:
		wprintf_s( L"E: parallelization mode with 0 threads specified\n" );
		return EXIT_FAILURE;
	case PARAMERROR_NOSTRATEGIES:
		wprintf_s( L"E: no strategies specified\n" );
		return EXIT_FAILURE;
	case PARAMWARNING_NODELIMITER:
		wprintf_s( L"W: no delimiter specified, fallback to ' '\n" );
		break;
	case 0:
		break;
	default:
		wprintf_s( L"E: unable to parse parameter\n" );
		return EXIT_FAILURE;
	}

#if (defined( _DEBUG ) && !defined(MULTIRUN)) || defined(FORCEDEBUGMESSAGES)
	wprintf_s( L"_DEBUG:parameter parser succeeded\n" );
#endif

	//create sudoku from given filepath in argv
	//define SUDOKU_CELLTYPE_BITVECTOR to use bit parallelized structure
	switch( Sudoku_ParseFile( &sudoku, params.filepath, params.delimiter ) ) {
	case SUDOKUERROR_FILE:
		wprintf_s( L"E: unable to open/read file '%s'\n", params.filepath );
		return EXIT_FAILURE;
	case SUDOKUERROR_GRIDSIZE:
		wprintf_s( L"E: invalid gridsize\n" );
		return EXIT_FAILURE;
	case SUDOKUERROR_PARSER:
		wprintf_s( L"E: parsing failed\n" );
		return EXIT_FAILURE;
	case SUDOKUERROR_OUTOFMEMORY:
		wprintf_s( L"E: out of memory\n" );
		return EXIT_FAILURE;
	case 0:
		break;
	default:
		wprintf_s( L"E: unable to parse sudoku file\n" );
		return EXIT_FAILURE;
	}
#if (defined( _DEBUG ) && !defined(MULTIRUN)) || defined(FORCEDEBUGMESSAGES)
	wprintf_s( L"_DEBUG:sudoku parser succeeded\n_DEBUG: parsed file:\n" );
	Sudoku_Print( &sudoku );

#endif

	//validate parsed sudoku
	//solver will not try to solve
	//completed or conflicted sudokus
	switch( Sudoku_Validate( &sudoku ) ) {
	case VALIDATION_CONFLICT:
		wprintf_s( L"E: invalid sudoku specified.\n" );
		return EXIT_FAILURE;
	case VALIDATION_SUCCESS:
		wprintf_s( L"E: completed sudoku specified.\n" );
		return EXIT_FAILURE;
	default:
		break;
	}

#if defined(SUDOKU_UI)
	//start ui thread
	//automatically terminates when the window closes
	CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )&CreateDebugWindow, ( LPVOID )&sudoku, 0, NULL );
	wprintf_s( L"please confirm responsive SUDOKU_UI" );
	getchar();
#endif

	//initialize new solver object
	//solver can be used to solve a given sudoku
	switch( Solver_Initialize( &solver, params.strategies, params.solvertype ) ) {
	case SOLVERERROR_OUTOFMEMORY:
		wprintf_s( L"E: out of memory\n" );
		return EXIT_FAILURE;
	case SOLVERERROR_INVALIDARGUMENT:
		wprintf_s( L"E: at least one invalid argument was passed to solver initialization\n" );
		return EXIT_FAILURE;
	case 0:
		break;
	default:
		wprintf_s( L"E: unable to initialize Solver\n" );
		return EXIT_FAILURE;
	}
#if defined(_DEBUG) && !defined(MULTIRUN) || defined(FORCEDEBUGMESSAGES)
	wprintf_s( L"_DEBUG: solver initialization successful\n" );
#endif
	
	//select the parsed sudoku as current
	sudoku_current = &sudoku;
#ifdef TRY_AND_ERROR
	//initialize SudokuStack
	//used to get next try and error iteration
	switch( SudokuStack_Initialize( &sudokustack, &sudoku, TRY_AND_ERROR_DEPTH ) ) {
	case -1:
		wprintf_s( L"E: out of memory" );
		return EXIT_FAILURE;
	case 0:
		break;
	}
#if defined(_DEBUG) && !defined(MULTIRUN) || defined(FORCEDEBUGMESSAGES)
	wprintf_s( L"_DEBUG: SudokuStack initialization successful\n" );
#endif

	ctTryAndErrorSteps = -1;
	timerun_added = 0;
	do {
		//count no of try and error iterations 
		//maximum number of iterations (steps) can 
		//be limited by TRY_AND_ERROR_MAXSTEPCOUNT in defaults.h
		ctTryAndErrorSteps++; 

		//repeat flag
		//need to be set to 1 to continue try and error iterations
		continue_try_and_error = 0;
		

		//start timer to track solution time
		if( params.timer != 0 ) Stopwatch_Start( &stopwatch );

		//solving of the sudoku with in argv specified strategies
		Solver_Solve( &solver, sudoku_current );

		//stop timer and print elapsed time to console
		//this time includes
		//-creation of sudoku copies
		//-validation
		//-solving
		if( params.timer != 0 ) {
			timerun = Stopwatch_GetTime( &stopwatch );
			//add current time to overall timesum
			//used to get more consistent
			timerun_added += timerun;
		}


		//validate sudoku
		//validation returns type of error on failure
		rc = Sudoku_Validate( sudoku_current );
		switch( rc ) {
		case VALIDATION_EMPTYCELL:
			if( ctTryAndErrorSteps >= TRY_AND_ERROR_MAXSTEPCOUNT ) {
				wprintf_s( L"_DEBUG: maximum number of try and error steps reached.\nvalidation failed.\n" );
				break;
			}
			//there are empty cells left, create sudoku copy and fill first free cell with first candidate
			switch( SudokuStack_GetNextModification( &sudokustack, &sudoku_current ) ) {
			case SUDOKUSTACKERROR_FULL:
				wprintf_s( L"E: sudokustack is full\nvalidation failed.\n" );
				break;
			case SUDOKUSTACKERROR_OUTOFMEMORY:
			case SUDOKUERROR_OUTOFMEMORY:
				wprintf_s( L"E: out of memory\nvalidation failed.\n" );
				return EXIT_FAILURE;
			case 0:
#if defined(SUDOKU_UI)
				//GetNextModification creates new Sudoku
				//pass new ptr to DebugWindow so it will be displayed
				ChangeDebugWindow( sudoku_current );
#endif
				continue_try_and_error = 1;
				break;
			default:
				wprintf_s( L"E: unknown sudokustack error\nvalidation failed.\n" );
				break;
			}
			break;
		case VALIDATION_CONFLICT:
			if( ctTryAndErrorSteps >= TRY_AND_ERROR_MAXSTEPCOUNT ) {
				wprintf_s( L"_DEBUG: maximum number of try and error steps reached.\nvalidation failed.\n" );
				break;
			}
#if defined(SUDOKU_UI)
			//GetNextSudoku deletes the last (current) sudoku
			//debugwindow sudoku will be invalid
			//tell debugwindow not to draw current sudoku by setting it to NULL
			ChangeDebugWindow( NULL );
#endif
			switch( SudokuStack_GetNextSudoku( &sudokustack, &sudoku_current ) ) {
			case SUDOKUSTACKERROR_OUTOFMEMORY:
			case SUDOKUERROR_OUTOFMEMORY:
				wprintf_s( L"E: out of memory\nvalidation failed.\n" );
				return EXIT_FAILURE;
			case SUDOKUSTACKERROR_EMPTY:
				break;
			case 0:
#if defined(SUDOKU_UI)
				//pass the new Sudoku to the DebugWindow to reenable display of sudoku
				ChangeDebugWindow( sudoku_current );
#endif
				continue_try_and_error = 1;
				break;
			default:
				wprintf_s( L"E: unknown sudokustack error\nvalidation failed.\n" );
				break;
			}
			break;
		case VALIDATION_SUCCESS:
#if !defined(MULTIRUN) || defined(FORCEDEBUGMESSAGES)
			wprintf_s( L"validation successful.\n" );
#endif
			break;
		}
		
		//continue while a case set continue_try_and_error to 1
	} while( continue_try_and_error == 1 );


#if (defined( _DEBUG ) && !defined(MULTIRUN)) || defined(FORCEDEBUGMESSAGES)
	wprintf_s( L"_DEBUG: %i try and error steps.\n_DEBUG: current try and error depth: %i\n", ctTryAndErrorSteps, SudokuStack_GetCurrentDepth( &sudokustack ) );
#endif

	if( params.timer ) {
#ifdef STOPWATCH_MODE_SECONDS
		wprintf_s( L"time solver: %.3fms\n", timerun_added * 1000 );
#else
		wprintf_s( L"time solver: %I64d\n", timerun_added );
#endif
#if defined(_DEBUG) || defined(MULTIRUN)
		//add time of current run to overall time
		timesum += timerun_added;
#endif
	}

#else //solve without try and error

	//start timer to track solution time
	if( params.timer != 0 ) Stopwatch_Start( &stopwatch );

	//solve the sudoku
	Solver_Solve( &solver, sudoku_current );

	//stop timer and print elapsed time to console
	//this time includes
	//-solving
	if( params.timer != 0 ) {
		timerun = Stopwatch_GetTime( &stopwatch );
#ifdef STOPWATCH_MODE_SECONDS
		wprintf_s( L"time solver: %.3fms\n", timerun * 1000 );
#else
		wprintf_s( L"time solver: %I64d\n", timerun );
#endif
#if defined(_DEBUG) || defined(MULTIRUN)
		timesum += timerun;
#endif
	}

	//validate the returned solution
	//only distinguish success and failure
	rc = Sudoku_Validate( &sudoku );
#if !defined(MULTIRUN) || defined(FORCEDEBUGMESSAGES)
	if( rc == VALIDATION_SUCCESS ) wprintf_s( L"validation successful.\n" );
	else wprintf_s( L"validation failed.\n" );
#endif

#endif
	
	//display solved sudoku to console
#if (defined( _DEBUG ) && !defined(MULTIRUN)) || defined(FORCEDEBUGMESSAGES)
	wprintf_s( L"_DEBUG: solver returned following grid:\n" );
	Sudoku_Print( sudoku_current );
#endif

#if defined(SUDOKU_UI)
	//show solved sudoku on window
	RefreshDebugWindow();
	//wait for user to confirm solution
	getchar();
	//close window
	//(terminates ui thread)
	CloseDebugWindow();
#elif !defined(_DEBUG) && defined(FORCE_KEEPALIVE) && !defined(MULTIRUN)
	getchar();
#endif

	return rc;
}

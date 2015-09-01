#include "parameterset.h"
#include "defaults.h"

int ParameterSet_Parse( struct ParameterSet* set, wchar_t** argv ) {
	unsigned int i, j, k;

	if( set == NULL ) return PARAMERROR_INVALIDARG;

	set->solvertype = 0;
	set->strategies = 0;
	set->filepath[0] = 0;
	set->delimiter = 0;
	set->timer = 0;

#ifdef FAKE_ARGV
	wchar_t* fargv[] = FAKE_ARGV;
	argv = ( wchar_t** )malloc( sizeof( fargv ) + sizeof(wchar_t*));
	for( i = 0; i < sizeof( fargv ) / sizeof( wchar_t* ); i++ ) argv[i] = fargv[i];
	argv[i] = NULL;
#endif

	i = ( unsigned int )-1;
	while( argv[++i] != NULL ) {
		if( argv[i][0] != L'-' ) continue;

		//Parameter:
		//strategies:
		// -st[0-9]
		//parallelization:
		// -par=[0-9]
		//no of threads:
		//-tc=[0-16]
		//sudoku filepath:
		// -sud=path
		//trennzeichen: (optional)
		//-del=[wchar_t]
		//timing:
		//-time 

		switch( argv[i][2] ) {
		case L'a': //par
			set->solvertype |= argv[i][5] - L'0';
			break;
		case L'u': //sud
			if( argv[i][5] == '\"' ) k = 6;
			else k = 5;

			j = k;
			while( argv[i][j] ) {
				set->filepath[j - k] = argv[i][j];
				j++;
			}
			if( set->filepath[j - k - 1] == '\"' ) set->filepath[j - k - 1] = 0;
			else set->filepath[j - k] = 0;
			break;
		case L'd':
			set->delimiter = argv[i][5];
			break;
		case L'i':
			set->timer = 1;
			break;
		case L't': //st
			j = argv[i][3] - L'0';
			if( argv[i][4] >= L'0' && argv[i][4] <= L'9' ) { //2. Stelle
				j *= 10;
				j += argv[i][4] - _T( '0' );
			}

			set->strategies |= ( 1 << j );
			break;
		case L'c': //tc
			j = argv[i][4] - L'0';
			if( argv[i][5] >= L'0' && argv[i][5] <= L'9' ) { //2. Stelle
				j *= 10;
				j += argv[i][5] - _T( '0' );
			}

			set->solvertype |= ( j << 16 );
			break;
		default:
			wprintf( L"Unknown parameter: '%s'\nfound at position: %i", argv[i], i );
			break;
		}
	}

#ifdef FAKE_ARGV
	free( argv );
#endif

	if( set->filepath[0] == 0 ) return PARAMERROR_NOFILE;
	if( (set->solvertype & 0xFFFF) != 0 && (set->solvertype & 0xFFFF0000 ) == 0) return PARAMERROR_NOTHREADCOUNT;
	if( set->strategies == 0 ) return PARAMERROR_NOSTRATEGIES;
	if( set->delimiter == 0 ) {
		set->delimiter = L' ';
		return PARAMWARNING_NODELIMITER;
	}

	return 0;
}

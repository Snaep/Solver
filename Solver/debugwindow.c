#include "debugwindow.h"
#include "sudoku.h"

static struct Sudoku* gsud;
static HWND hWnd;
static HANDLE hSemWindow;

static LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
static void onPaint( WPARAM wParam, LPARAM lParam );

DWORD WINAPI CreateDebugWindow( struct Sudoku* sud ) {
	ATOM cid = 0;
	MSG msg = { 0 };
	WNDCLASS wndclass = { 0 };
	HINSTANCE hInstance = GetModuleHandle( 0 );

	hSemWindow = CreateSemaphore( NULL, 0, 1, NULL );

	gsud = sud;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = ( HICON )LoadIcon( 0, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = CreateSolidBrush( RGB( 240, 240, 240 ) );
	wndclass.lpszClassName = _T( "MyWindowClass" );

	cid = RegisterClass( &wndclass );
	if( cid == 0 ) {
		MessageBox( NULL, _T( "RegisterClass has failed" ), NULL, MB_OK | MB_ICONERROR );
		DestroyIcon( wndclass.hIcon );
		return EXIT_FAILURE;
	}

	hWnd = CreateWindow( ( LPTSTR )cid, _T( "SUDOKU_UI" ), WS_OVERLAPPEDWINDOW, 0, 0, 720, 720, NULL, NULL, hInstance, NULL );
	
	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	while( GetMessage( &msg, NULL, 0, 0 ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	UnregisterClass( ( LPTSTR )cid, hInstance );

	ReleaseSemaphore( hSemWindow, 1, NULL );

	return ( int )msg.wParam;
}

void onPaint( WPARAM wParam, LPARAM lParam ) {
	unsigned int x, i, width, height, ix, iy, p;
	RECT rcClient;
	HANDLE hOldPen;
	wchar_t buffer[512];
	HDC hdcWindow, hdc;
	HBITMAP hBmpBuffer;
	HPEN hBoxLine;
	HFONT f;
	HFONT hOldFont;
	HBRUSH brWhite;

	if( gsud == NULL ) return;
	brWhite = CreateSolidBrush( RGB( 0xFF, 0xFF, 0xFF ) );
	f = CreateFont( 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"consolas" );
	GetClientRect( hWnd, &rcClient );
	hdcWindow = GetDC( hWnd );
	hBmpBuffer = CreateCompatibleBitmap( hdcWindow, rcClient.right, rcClient.bottom );
	hdc = CreateCompatibleDC( hdcWindow );
	SelectObject( hdc, hBmpBuffer );

	hBoxLine = CreatePen( PS_SOLID, 2, 0 );

	hOldFont = SelectObject( hdc, f );
	FillRect( hdc, &rcClient, brWhite );

	width = rcClient.right / gsud->length;
	height = rcClient.bottom / gsud->length;

	SetBkMode( hdc, TRANSPARENT );

	for( i = 0; i < gsud->length; i++ ) {
		for( x = 0; x < gsud->length; x++ ) {
			iy = 0;
			ix = 0;
			if( gsud->cellvalue[i][x] != 0 ) {
				SetTextColor( hdc, RGB( 0, 150, 0 ) );
				wsprintfW( buffer, L"%i", gsud->cellvalue[i][x] );
				TextOut( hdc, x * width + width / 2 - 5, i * height + height / 2 - 5, buffer, lstrlenW( buffer ) );
			} else {

				for( p = 0; p < gsud->length; p++ ) {


					if( p != 0 && p % gsud->length_of_box == 0 ) {
						iy += height / gsud->length_of_box;
						ix = 0;
					} else if( p ) ix += width / gsud->length_of_box;
#ifdef SUDOKU_CELLTYPE_BITVECTOR
					if( gsud->grid[i][x] & ( 1LL << p ) ) {
#else
					if( gsud->grid[i][x][p] ) {
#endif
						SetTextColor( hdc, 0 );
					} else SetTextColor( hdc, RGB( 200, 200, 200 ) );

					wsprintfW( buffer, L"%i", p + 1 );
					TextOutW( hdc, x * width + ix, i * height + iy, buffer, lstrlenW( buffer ) );
					}
			}

		}

		if( i != 0 && i % gsud->length_of_box == 0 ) {
			hOldPen = SelectObject( hdc, hBoxLine );
			MoveToEx( hdc, 0, i * height, NULL );
			LineTo( hdc, rcClient.right, i * height );
			MoveToEx( hdc, i * width, 0, NULL );
			LineTo( hdc, i * width, rcClient.bottom );
			SelectObject( hdc, hOldPen );
		} else {
			MoveToEx( hdc, 0, i * height, NULL );
			LineTo( hdc, rcClient.right, i * height );
			MoveToEx( hdc, i * width, 0, NULL );
			LineTo( hdc, i * width, rcClient.bottom );
		}
	}

	SelectObject( hdc, hOldFont );

	DeleteObject( f );
	DeleteObject( hBoxLine );
	BitBlt( hdcWindow, 0, 0, rcClient.right, rcClient.bottom, hdc, 0, 0, SRCCOPY );
	ReleaseDC( hWnd, hdcWindow );
	DeleteObject( hBmpBuffer );
	DeleteObject( hdc );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	static HANDLE hProcess;

	switch( msg ) {
	case WM_CREATE:
		SetTimer( hWnd, 0, 1000, NULL );
		return 0;
	case WM_TIMER:
		InvalidateRect( hWnd, NULL, TRUE );
		return 0;
	case WM_PAINT:
		onPaint( wParam, lParam );
		ValidateRect( hWnd, NULL ); //creates infinite loop without validation
		return 0;
	case WM_KEYDOWN:
		return 0;
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void RefreshDebugWindow() {
	//yes, sleep inside critical section.
#pragma omp critical "debugwindow"
{
	onPaint( ( WPARAM )NULL, ( LPARAM )NULL );
#ifdef SUDOKU_UI_NOBLOCK
	Sleep( 50 );
#else
	getchar();
#endif
}
}

void CloseDebugWindow() {
	if( hWnd != NULL ) {
		SendMessage( hWnd, WM_CLOSE, 0, 0 );
		WaitForSingleObject( hSemWindow, INFINITE );
		CloseHandle( hSemWindow );
	}
}


DWORD WINAPI RefreshLoop( LPVOID param ) {
	while( TRUE ) {
		onPaint( 0, 0 );
	}
}
void ChangeDebugWindow( struct Sudoku* sud ) {
	gsud = sud;
	if( sud == NULL ) return;
	RefreshDebugWindow();
}
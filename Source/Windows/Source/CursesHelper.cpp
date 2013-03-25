#include <CursesHelper.hpp>
#include <Utility.hpp>
#include <curses.h>

static bool CursesInit = false;

int StartCURSES( const bool p_Raw, const bool p_Echo,
	const bool p_FunctionKeys )
{
	if( CursesInit )
	{
		return 1;
	}

	initscr( );

	if( p_Raw )
	{
		raw( );
	}

	if( p_Echo )
	{
		echo( );
	}
	else
	{
		noecho( );
	}

	if( p_FunctionKeys )
	{
		keypad( stdscr, true );
	}

	if( has_colors( ) == FALSE )
	{
		printw( "[INFO] No support for colours\n" );
		refresh( );
		getch( );
	}
	else
	{
		start_color( );
		// Error messages
		init_pair( 1, COLOR_RED, COLOR_BLACK );
		// Warning messages
		init_pair( 2, COLOR_YELLOW, COLOR_BLACK );
		// Information messages
		init_pair( 3, COLOR_GREEN, COLOR_BLACK );
		// Normal messages
		init_pair( 4, COLOR_WHITE, COLOR_BLACK );
	}

	CursesInit = true;

	return 0;
}

int StopCURSES( const bool p_Wait, const bool p_PrintMessage )
{
	if( p_PrintMessage )
	{
		printw( "Press any key to exit" );
	}
	refresh( );
	if( p_Wait )
	{
		getch( );
	}
	endwin( );
	CursesInit = false;
	return 0;
}

void Print( const MESSAGE_TYPE p_Message, WINDOW *p_pWindow,
	const char *p_pMessage, ... )
{
	if( p_Message < MSG_NORMAL || p_Message > MSG_MAX )
	{
		if( p_pWindow )
		{
			attron( COLOR_PAIR( 4 ) );
			waddstr( p_pWindow, "Error.  Message type not recognised\n" );
			attroff( COLOR_PAIR( 4 ) );
		}
		else
		{
			waddstr( stdscr, "Error.  Message type not recognised\n" );
		}
		return;
	}

	if( p_pWindow )
	{
		char *pCompleteMessage;
		int RetVal;

		va_list ArgPtr;
		va_start( ArgPtr, p_pMessage );
		int StrLen = vprintf( p_pMessage, ArgPtr ) + 1;
		pCompleteMessage = new char[ StrLen ];
		RetVal = vsnprintf( pCompleteMessage, StrLen, p_pMessage, ArgPtr );
		va_end( ArgPtr );
		
		switch( p_Message )
		{
		case MSG_NORMAL:
			{
				wattron( p_pWindow, COLOR_PAIR( 4 ) );
				waddstr( p_pWindow, pCompleteMessage );
				wattroff( p_pWindow, COLOR_PAIR( 4 ) );
				break;
			}
		case MSG_INFO:
			{
				wattron( p_pWindow, COLOR_PAIR( 3 ) );
				waddstr( p_pWindow, pCompleteMessage );
				wattroff( p_pWindow, COLOR_PAIR( 3 ) );
				break;
			}
		case MSG_WARNING:
			{
				wattron( p_pWindow, COLOR_PAIR( 2 ) );
				waddstr( p_pWindow, pCompleteMessage );
				wattroff( p_pWindow, COLOR_PAIR( 2 ) );
				break;
			}
		case MSG_ERROR:
			{
				wattron( p_pWindow, COLOR_PAIR( 1 ) );
				waddstr( p_pWindow, pCompleteMessage );
				wattroff( p_pWindow, COLOR_PAIR( 1 ) );
				break;
			}
		default:
			break;
		}

		refresh( );
		
		SAFE_DELETE_ARRAY( pCompleteMessage );
	}
}

void MVPrint( const int p_Column, const int p_Row,
	const MESSAGE_TYPE p_Message,
	WINDOW *p_pWindow, const char *p_pMessage, ... )
{
	if( p_Message < MSG_NORMAL || p_Message > MSG_MAX )
	{
		if( p_pWindow )
		{
			attron( COLOR_PAIR( 4 ) );
			waddstr( p_pWindow, "Error.  Message type not recognised\n" );
			attroff( COLOR_PAIR( 4 ) );
		}
		else
		{
			waddstr( stdscr, "Error.  Message type not recognised\n" );
		}
		return;
	}

	if( p_pWindow )
	{
		char *pCompleteMessage;
		// HACK!
		// For some reason, PDCURSES is printing the string as if the cursor
		// were not moved at all, so these blank characters will need to be
		// inserted
		// !HACK
		char *pBlankStr;
		int RetVal;

		va_list ArgPtr;
		va_start( ArgPtr, p_pMessage );
		int StrLen = vprintf( p_pMessage, ArgPtr ) + 1;
		pCompleteMessage = new char[ StrLen ];
		pBlankStr = new char[ StrLen+1 ];
		for( size_t i = 0; i < StrLen; ++i )
		{
			pBlankStr[ i ] = ' ';
		}
		pBlankStr[ StrLen ] = '\0';
		RetVal = vsnprintf( pCompleteMessage, StrLen, p_pMessage, ArgPtr );
		va_end( ArgPtr );
		
		switch( p_Message )
		{
		case MSG_NORMAL:
			{
				wattron( p_pWindow, COLOR_PAIR( 4 ) );
				mvwaddstr( p_pWindow, p_Column, p_Row, pCompleteMessage );
				wattroff( p_pWindow, COLOR_PAIR( 4 ) );
				break;
			}
		case MSG_INFO:
			{
				wattron( p_pWindow, COLOR_PAIR( 3 ) );
				waddstr( p_pWindow, pBlankStr );
				mvwaddstr( p_pWindow, p_Column, p_Row, pCompleteMessage );
				wattroff( p_pWindow, COLOR_PAIR( 3 ) );
				break;
			}
		case MSG_WARNING:
			{
				wattron( p_pWindow, COLOR_PAIR( 2 ) );
				mvwaddstr( p_pWindow, p_Column, p_Row, pCompleteMessage );
				wattroff( p_pWindow, COLOR_PAIR( 2 ) );
				break;
			}
		case MSG_ERROR:
			{
				wattron( p_pWindow, COLOR_PAIR( 1 ) );
				mvwaddstr( p_pWindow, p_Column, p_Row, pCompleteMessage );
				wattroff( p_pWindow, COLOR_PAIR( 1 ) );
				break;
			}
		default:
			break;
		}

		refresh( );
		
		SAFE_DELETE_ARRAY( pBlankStr );
		SAFE_DELETE_ARRAY( pCompleteMessage );
	}
}


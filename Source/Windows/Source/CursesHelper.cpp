#include <CursesHelper.hpp>
#include <curses.h>

static bool CursesInit = false;

int StartCURSES( const bool p_Raw, const bool p_Echo, const bool p_FunctionKeys )
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

	CursesInit = true;

	return 0;
}

int StopCURSES( const bool p_Wait )
{
	refresh( );
	if( p_Wait )
	{
		getch( );
	}
	endwin( );
	CursesInit = false;
	return 0;
}

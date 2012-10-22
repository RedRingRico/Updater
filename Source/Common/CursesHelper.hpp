#ifndef __UPDATER_CURSESHELPER_HPP__
#define __UPDATER_CURSESHELPER_HPP__

#include <curses.h>

typedef enum __MESSAGE_TYPE
{
	MSG_NORMAL,
	MSG_INFO,
	MSG_WARNING,
	MSG_ERROR,
	MSG_MAX,
}MESSAGE_TYPE;

int StartCURSES( const bool p_Raw = true, const bool p_Echo = false,
	const bool p_FunctionKeys = true );
int StopCURSES( const bool p_Wait = true, const bool p_PrintMessage = true );

void Print( MESSAGE_TYPE p_Message, WINDOW *p_pWindow,
	const wchar_t *p_pMessage, ... );

#endif

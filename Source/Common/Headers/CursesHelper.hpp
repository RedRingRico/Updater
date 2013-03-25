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

void Print( const MESSAGE_TYPE p_Message, WINDOW *p_pWindow,
	const char *p_pMessage, ... );
void MVPrint( const int p_Column, const int p_Row,
	const MESSAGE_TYPE p_Message,
	WINDOW *p_pWindow, const char *p_pMessage, ... );

#endif

#ifndef __UPDATER_CURSESHELPER_HPP__
#define __UPDATER_CURSESHELPER_HPP__

int StartCURSES( const bool p_Raw = true, const bool p_Echo = false,
	const bool p_FunctionKeys = true );
int StopCURSES( const bool p_Wait = true );

#endif

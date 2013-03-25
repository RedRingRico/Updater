#ifndef __UPDATER_LOG_HPP__
#define __UPDATER_LOG_HPP__

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include <exception>
#include <string>

class Log
{
public:
	Log( );
	Log( const char *p_pLogFileName );
	~Log( );

	// Set a new location for the log file and optionally back up the log file
	// if there is a name conflict when loading the file
	int SetLogFile( const char *p_pLogFileName, const bool p_Backup = true );

	int Print( const char *p_pMessage, ... );

private:
	int OpenLogFile( const bool p_Backup );
	void CloseLogFile( );

#ifdef PLATFORM_LINUX
	FILE *m_pLogFile;
	std::string m_LogFileName;
#elif PLATFORM_WINDOWS
	HANDLE m_LogFileHandle;
	std::wstring m_LogFileName;
#endif
};

#endif

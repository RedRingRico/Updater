#include <Log.hpp>
#include <iostream>
#include <Utility.hpp>
#include <cstring>
#include <stdarg.h>

Log::Log( )
{
#ifdef PLATFORM_WINDOWS
	m_LogFileHandle = INVALID_HANDLE_VALUE;
#else
	m_pLogFile = NULL;
#endif
}

Log::Log( const char *p_pLogFileName )
{
#ifdef PLATFORM_WINDOWS
	m_LogFileHandle = INVALID_HANDLE_VALUE;

	size_t NameLength = strlen( p_pLogFileName );

	if( NameLength != 0 )
	{
		wchar_t *pLogFileName = NULL;
		::ConvertCharToWide( p_pLogFileName, &pLogFileName );
		m_LogFileName.clear( );
		m_LogFileName.append( pLogFileName );

		SAFE_DELETE_ARRAY( pLogFileName );
	}
#else
	m_LogFileName.clear( );
	m_LogFileName.append( p_pLogFileName );
#endif
}

Log::~Log( )
{
#ifdef PLATFORM_WINDOWS
	if( m_LogFileHandle != INVALID_HANDLE_VALUE )
	{
		this->CloseLogFile( );
	}
#else
	if( m_pLogFile )
	{
		fclose( m_pLogFile );
		m_pLogFile = NULL;
	}
#endif
}

int Log::SetLogFile( const char *p_pLogFileName, const bool p_Backup )
{
#ifdef PLATFORM_WINDOWS
	size_t NameLength = strlen( p_pLogFileName );
	if( NameLength == 0 )
	{
		return 1;
	}

	wchar_t *pLogFileName = NULL;
	ConvertCharToWide( p_pLogFileName, &pLogFileName );

	m_LogFileName.clear( );
	m_LogFileName.append( pLogFileName );

	SAFE_DELETE_ARRAY( pLogFileName );
#else
	m_LogFileName.clear( );
	m_LogFileName.append( p_pLogFileName );
#endif
	

	return this->OpenLogFile( p_Backup );
}

int Log::Print( const char *p_pMessage, ... )
{
#ifdef PLATFORM_WINDOWS
	if( m_LogFileHandle == INVALID_HANDLE_VALUE )
	{
		if( OpenLogFile( false ) != 0 )
		{
			return 1;
		}
	}

	DWORD BytesWritten;
#endif
	char CompleteMessage[ 4096 ];
	int RetVal;

	va_list ArgPtr;
	va_start( ArgPtr, p_pMessage );
	RetVal = vsnprintf( CompleteMessage,
		sizeof( CompleteMessage )*sizeof( char ), p_pMessage,
		ArgPtr );
	va_end( ArgPtr );

#ifdef PLATFORM_WINDOWS
	WriteFile( m_LogFileHandle, CompleteMessage, strlen( CompleteMessage ),
		&BytesWritten, NULL );
#else
	fwrite( CompleteMessage, sizeof( char ), strlen( CompleteMessage ),
		m_pLogFile );
#endif

	return 0;
}

int Log::OpenLogFile( const bool p_Backup )
{
	// Make sure the log filename is valid and then attempt to back up a file
	// with the same name if necessary
	if( m_LogFileName.empty( ) )
	{
		return 1;
	}

	// Use a simple number system of logname_number.extension
	if( p_Backup )
	{
#ifdef PLATFORM_WINDOWS
		// Get the extension of the log file so that everything after it can be
		// replaced with an asterisk
		size_t DotPos = m_LogFileName.find_last_of( L'.' ) + 1;
		wchar_t CurrentDirectory[ MAX_PATH ] = { L'\0' };
		GetCurrentDirectory( MAX_PATH, CurrentDirectory );

		std::wstring SearchStr;
		SearchStr.append( CurrentDirectory );
		SearchStr.append( L"\\" );
		SearchStr.append( m_LogFileName.substr( 0, DotPos ) );
		SearchStr.append( L"*" );

		WIN32_FIND_DATA FindData;
		
		HANDLE Find = FindFirstFile( SearchStr.c_str( ), &FindData );

		if( Find == INVALID_HANDLE_VALUE )
		{
			return 1;
		}

		// Running highest value for log.number.ext
		long LogNumber = 0;

		do
		{
			std::wstring NewLog = FindData.cFileName;

			// Get the value just before the .ext
			size_t Dot = NewLog.find_last_of( L"." );
			NewLog.resize( Dot );
			if( ( Dot = NewLog.find_last_of( L"." ) ) == std::string::npos )
			{
				// No number, skip
				continue;
			}

			++Dot;
			long Number =wcstol( NewLog.substr( Dot, NewLog.size( ) ).c_str( ),
				NULL, 10 );
			if( Number > LogNumber )
			{
				LogNumber = Number;
			}
		} while( FindNextFile( Find, &FindData ) != 0 );

		FindClose( Find );

		// Finally, create the new file
		std::wstring LogFile;
		wchar_t StrLogNumber[ 16 ];
		++LogNumber;
		_itow( LogNumber, StrLogNumber, 10 );

		LogFile.append(
			m_LogFileName.substr( 0, m_LogFileName.find_first_of( L"." )+1 ) );
		LogFile.append( StrLogNumber );
		LogFile.append( L".log" );
		MoveFile( m_LogFileName.c_str( ), LogFile.c_str( ) );
#else
#endif
	}

#ifdef PLATFORM_WINDOWS
	m_LogFileHandle = CreateFile( m_LogFileName.c_str( ), GENERIC_WRITE,
		FILE_SHARE_READ, NULL, CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL, NULL );

	if( m_LogFileHandle == INVALID_HANDLE_VALUE )
	{
		return 1;
	}
#else
	m_pLogFile = fopen( m_LogFileName.c_str( ), "wb" );
#endif
	
	return 0;
}

void Log::CloseLogFile( )
{
#ifdef PLATFORM_WINDOWS
	CloseHandle( m_LogFileHandle );
	m_LogFileHandle = INVALID_HANDLE_VALUE;
#else
#endif
}


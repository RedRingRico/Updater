#include <Log.hpp>
#include <iostream>

Log::Log( )
{
	m_LogFileHandle = INVALID_HANDLE_VALUE;
}

Log::Log( const char *p_pLogFileName )
{
	m_LogFileHandle = INVALID_HANDLE_VALUE;

	size_t NameLength = strlen( p_pLogFileName );

	if( NameLength != 0 )
	{
		this->ConvertCharToWide( p_pLogFileName, m_LogFileName );
	}
}

Log::~Log( )
{
	if( m_LogFileHandle != INVALID_HANDLE_VALUE )
	{
		this->CloseLogFile( );
	}
}

int Log::SetLogFile( const char *p_pLogFileName, const bool p_Backup )
{
	size_t NameLength = strlen( p_pLogFileName );
	if( NameLength == 0 )
	{
		return 1;
	}

	this->ConvertCharToWide( p_pLogFileName, m_LogFileName );

	return OpenLogFile( p_Backup );
}

int Log::Print( const char *p_pMessage, ... )
{
	if( m_LogFileHandle == INVALID_HANDLE_VALUE )
	{
		if( OpenLogFile( false ) != 0 )
		{
			return 1;
		}
	}

	DWORD BytesWritten;

	WriteFile( m_LogFileHandle, p_pMessage, strlen( p_pMessage ),
		&BytesWritten, NULL );

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
	}

	m_LogFileHandle = CreateFile( m_LogFileName.c_str( ), GENERIC_WRITE,
		FILE_SHARE_READ, NULL, CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL, NULL );

	if( m_LogFileHandle == INVALID_HANDLE_VALUE )
	{
		return 1;
	}
	
	return 0;
}

void Log::CloseLogFile( )
{
	CloseHandle( m_LogFileHandle );
	//m_LogFileHandle = INVALID_HANDLE_VALUE;
}

int Log::ConvertCharToWide( const char *p_pChar, std::wstring &p_Wide )
{
	size_t StrLen = strlen( p_pChar );

	if( StrLen == 0 )
	{
		return 1;
	}
	
	wchar_t *pWide = new wchar_t[ StrLen+1 ];

	int Result = MultiByteToWideChar( CP_UTF8, 0,
		p_pChar, StrLen, pWide, StrLen );
	if( Result != StrLen )
	{
		DWORD Error = GetLastError( );

		switch( Error )
		{
		case ERROR_INSUFFICIENT_BUFFER:
			{
				std::cout << "[ERROR] A supplied buffer was not large enough "
					"or set to NULL" << std::endl;
				break;
			}
		case ERROR_INVALID_FLAGS:
			{
				std::cout << "[ERROR] The values supplied by the flags were not"
					" valid" << std::endl;
				break;
			}
		case ERROR_INVALID_PARAMETER:
			{
				std::cout << "[ERROR] Any of the parameter values were invalid"
					<< std::endl;
				break;
			}
		case ERROR_NO_UNICODE_TRANSLATION:
			{
				std::cout << "[ERROR] Invalid Unicode was found in a string"
					<< std::endl;
				break;
			}
		default:
			{
				std::cout << "Unknown error" << std::endl;
				break;
			}
		}

		delete [ ] pWide;
		pWide = NULL;

		return 1;
	}

	pWide[ StrLen ] = L'\0';
	p_Wide.clear( );
	p_Wide = pWide;

	delete [ ] pWide;
	pWide = NULL;

	return 0;
}

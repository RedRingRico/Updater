#include <ConfigFile.hpp>
#include <Utility.hpp>
#include <list>
#include <string>
#include <iostream>

ConfigFile::ConfigFile( )
{
	m_FileHandle = INVALID_HANDLE_VALUE;
}

ConfigFile::ConfigFile( const wchar_t *p_pConfigFile )
{
	m_FileHandle = INVALID_HANDLE_VALUE;

	m_FileName.empty( );
	m_FileName.append( p_pConfigFile );
}

ConfigFile::~ConfigFile( )
{
}

int ConfigFile::Parse( CONFIG_PARAMETERS *p_pParameters )
{
	if( m_FileName.empty( ) || p_pParameters == NULL )
	{
		return 1;
	}

	m_FileHandle = CreateFile( m_FileName.c_str( ), GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( m_FileHandle == INVALID_HANDLE_VALUE )
	{
		return 1;
	}

	DWORD FileSize = GetFileSize( m_FileHandle, NULL );
	DWORD ReadSize = 0;

	char *pFileContents = new char [ FileSize ];

	ReadFile( m_FileHandle, pFileContents, FileSize, &ReadSize, NULL );

	if( ReadSize != FileSize )
	{
		SAFE_DELETE_ARRAY( pFileContents );
		CloseHandle( m_FileHandle );
		m_FileHandle = INVALID_HANDLE_VALUE;

		return 1;
	}

	size_t LineCount = 0, CharStart = 0, CharEnd = 0;
	std::list< std::string > FileLines;

	// First sweep, check for lines beginning with comments and lines without
	// delimiters, copy only lines that are potentially valid
	for( size_t i = 0; i < FileSize; ++i )
	{
		if( pFileContents[ i ] == '\n' ||
			( ( i == ( FileSize-1 ) ) && ( pFileContents[ i ] != '\n' ) ) )
		{
			if( LineCount != 0 )
			{
				CharStart++;
			}

			char *pLine = new char [ ( CharEnd - CharStart ) + 1 ];
			strncpy( pLine, pFileContents+CharStart, CharEnd-CharStart );
			pLine[ ( CharEnd - CharStart ) ] = '\0';

			// Is this just a carrige return or the start of a comment?
			if( pLine[ 0 ] == 0xD )
			{
				SAFE_DELETE_ARRAY( pLine );
				CharStart = CharEnd;
				++CharEnd;
				continue;
			}

			if( pLine[ 0 ] == '#' )
			{
				SAFE_DELETE_ARRAY( pLine );
				if( LineCount == 0 )
				{
					CharStart = ++CharEnd;
				}
				else
				{
					CharStart = CharEnd;
					CharEnd++;
				}
				continue;
			}

			// Is this line just whitespace?
			bool AllWhitespace = true;
			bool Comment = false;
			for( size_t Char = 0; Char < strlen( pLine ); ++Char )
			{
				if( ( pLine[ Char ] == '#' ) && ( AllWhitespace == true ) )
				{
					Comment = true;
					break;
				}

				if( pLine[ Char ] != ' ' && pLine[ Char ] != '\t' &&
					pLine[ Char ] != '\r' && pLine[ Char ] != '\n' )
				{
					size_t Truncate = strcspn( pLine, "#" );

					if( Truncate != strlen( pLine ) )
					{
						char *pNewLine = new char [ Truncate+1 ];
						strncpy( pNewLine, pLine, Truncate );
						pNewLine[ Truncate ] = '\0';
						strcpy( pLine, pNewLine );
						SAFE_DELETE_ARRAY( pNewLine );
					}

					AllWhitespace = false;
					break;
				}
			}

			if( AllWhitespace )
			{
				SAFE_DELETE_ARRAY( pLine );
				if( Comment )
				{
					CharStart = ++CharEnd;
				}
				else
				{
					CharStart = CharEnd;
					++CharEnd;
				}
				continue;
			}

			FileLines.push_back( pLine );
			SAFE_DELETE_ARRAY( pLine );
			CharStart = CharEnd;
			++LineCount;
		}
		++CharEnd;
	}

	// Process the key-value pairs assumedly left
	std::list< std::string >::iterator Itr = FileLines.begin( );

	LineCount = 0;
	for( ; Itr != FileLines.end( ); ++Itr )
	{
		size_t Delimiter = ( *Itr ).find_first_of( "=" );
		if( Delimiter != std::string::npos )
		{
			std::string Key, Value;

			Key = ( *Itr ).substr( 0, Delimiter );
			Value = ( *Itr ).substr( Delimiter+1 );

			std::cout << "[" << LineCount << "] " << "Key: " << Key.c_str( ) <<
				"\t\t\tValue: " << Value.c_str( ) << std::endl;
			++LineCount;
		}
	}

	SAFE_DELETE_ARRAY( pFileContents );

	CloseHandle( m_FileHandle );
	m_FileHandle = INVALID_HANDLE_VALUE;

	return 0;
}

#include <ConfigFile.hpp>
#include <Utility.hpp>
#include <list>
#include <string>
#include <iostream>

ConfigFile::ConfigFile( )
{
	m_FileHandle = INVALID_HANDLE_VALUE;
}

ConfigFile::ConfigFile( const char *p_pConfigFile )
{
	m_FileHandle = INVALID_HANDLE_VALUE;

	wchar_t *pCfgFile = NULL;
	ConvertCharToWide( p_pConfigFile, &pCfgFile );

	m_FileName.empty( );
	m_FileName.append( pCfgFile );

	SAFE_DELETE_ARRAY( pCfgFile );
}

ConfigFile::~ConfigFile( )
{
}

int ConfigFile::SetConfigFile( const char *p_pConfigFile )
{
	wchar_t *pTmpFile = NULL;

	if( ConvertCharToWide( p_pConfigFile, &pTmpFile ) != 0 )
	{
		SAFE_DELETE_ARRAY( pTmpFile );
		return 1;
	}

	m_FileName.empty( );
	m_FileName.append( pTmpFile );

	SAFE_DELETE_ARRAY( pTmpFile );

	return 0;
}

int ConfigFile::Parse( )
{
	if( m_FileName.empty( ) )
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

			if( i == FileSize-1 )
			{
				++CharEnd;
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

	for( ; Itr != FileLines.end( ); ++Itr )
	{
		size_t Delimiter = ( *Itr ).find_first_of( "=" );
		if( Delimiter != std::string::npos )
		{
			std::string Key, Value;

			Key = ( *Itr ).substr( 0, Delimiter );
			Value = ( *Itr ).substr( Delimiter+1 );

			this->RemoveWhitespace( Key );
			this->RemoveWhitespace( Value );

			m_OptionValue[ Key ] = Value;
		}
	}

	SAFE_DELETE_ARRAY( pFileContents );

	CloseHandle( m_FileHandle );
	m_FileHandle = INVALID_HANDLE_VALUE;

	return 0;
}

int ConfigFile::GetValue( const std::string &p_Option, std::string &p_Value )
{
	OptionValueItr Itr = m_OptionValue.find( p_Option );

	if( Itr != m_OptionValue.end( ) )
	{
		p_Value = Itr->second;

		return 0;
	}

	return 1;
}

void ConfigFile::GetOptionValue( const size_t p_Index, std::string &p_Option,
	std::string &p_Value )
{
	if( p_Index > 0 && p_Index < m_OptionValue.size( ) )
	{
		OptionValueItr Itr = m_OptionValue.begin( );

		for( size_t i = 0; i < p_Index; ++i )
		{
			++Itr;
		}

		p_Option = Itr->first;
		p_Value = Itr->second;
	}
}

void ConfigFile::RemoveWhitespace( std::string &p_String, const bool p_Front,
	const bool p_Back )
{
	if( !p_String.empty( ) )
	{
		size_t WhitePos = 0;

		// First, make sure the line's carrige return is handled
		size_t RC = p_String.find_first_of( "\r" );
		if( RC != std::string::npos )
		{
			p_String.erase( RC );
		}

		// Remove leading whitespace
		if( p_Front )
		{
			WhitePos = p_String.find_first_not_of( " \t" );

			if( WhitePos != std::string::npos )
			{
				p_String.erase( 0, WhitePos );
			}
		}

		// Remove trailing whitespace
		if( p_Back )
		{
			std::string::reverse_iterator RItr = p_String.rbegin( );
			size_t CurPos = 0;

			if( *RItr == ' ' || *RItr == '\t' )
			{
				// Keep going until a non-whitespace character is encountered
				++RItr;
				for( ; RItr != p_String.rend( );++ RItr )
				{
					++CurPos;
					if( *RItr != ' ' && *RItr != '\t' )
					{
						WhitePos = p_String.size( ) - CurPos;
						break;
					}
				}

				p_String.erase( WhitePos );
			}
		}
	}
}

#include <Utility.hpp>
#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#include <cstring>

int ConvertCharToWide( const char *p_pChar, wchar_t **p_ppWide )
{
	if( p_pChar == NULL )
	{
		return 1;
	}

	size_t StrLen = strlen( p_pChar );

	if( StrLen == 0 )
	{
		return 1;
	}

	( *p_ppWide ) = new wchar_t[ StrLen+1 ];

	int Result = MultiByteToWideChar( CP_UTF8, 0,
		p_pChar, StrLen, ( *p_ppWide ), StrLen );

	if( Result != StrLen )
	{
		DWORD Error = GetLastError( );

		SAFE_DELETE_ARRAY( *p_ppWide );
		SAFE_DELETE_ARRAY( p_ppWide );

		return 1;
	}

	( *p_ppWide )[ StrLen ] = L'\0';

	return 0;
}
#endif


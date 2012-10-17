#include <ConfigFile.hpp>
#include <Utility.hpp>

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

	SAFE_DELETE_ARRAY( pFileContents );

	CloseHandle( m_FileHandle );
	m_FileHandle = INVALID_HANDLE_VALUE;

	return 0;
}

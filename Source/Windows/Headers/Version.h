// Credit for code to Zach Burlingame
// http://www.zachburlingame.com/2011/02

#ifndef __UPDATER_VERSION_HPP__ 
#define __UPDATER_VERSION_HPP__ 

#include "HgVersion.h"

#define STRINGISE2( s ) #s
#define STRINGISE( s ) STRINGISE2( s )

#define VERSION_MAJOR		1
#define VERSION_MINOR		0
#define VERSION_REVISION	0
#define VERSION_BUILD		HG_REVISION

#if HG_LOCAL_MODIFICATIONS
#define VERSION_MODIFIER "M"
#else
#define VERSION_MODIFIER
#endif

#define VER_FILE_DESCRIPTION_STR	HG_CHANGESET
#define VER_FILE_VERSION			VERSION_MAJOR, VERSION_MINOR, \
									VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_STR		STRINGISE( VERSION_MAJOR ) "."\
									STRINGISE( VERSION_MINOR ) "."\
									STRINGISE( VERSION_REVISION ) "."\
									STRINGISE( VERSION_BUILD ) \
									VERSION_MODIFIER
#define VER_PRODUCT_NAME_STR		"Updater"
#define VER_PRODUCT_VERSION			VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR		VER_FILE_VERSION_STR
#define VER_ORIGINAL_FILENAME_STR	VER_PRODUCT_NAME_STR ".exe"
#define VER_INTERNAL_NAME_STR		VER_ORIGINAL_FILENAME_STR
#define VER_LEGAL_COPYRIGHT_STR		"Copyleft Interactive Laboratories 2012"
#define VER_COMPANY_NAME_STR		"Interactive Laboratories"

#ifdef _DEBUG
#define VER_VER_DEBUG	VS_FF_DEBUG
#else
#define VER_VER_DEBUG	0
#endif

#define VER_FILEOS		VOS_NT_WINDOWS32
#define VER_FILEFLAGS	VER_VER_DEBUG
#define	VER_FILETYPE	VFT_APP

#endif 
 

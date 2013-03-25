#ifndef __UPDATER_COMPILER_GCC_HPP__
#define __UPDATER_COMPILER_GCC_HPP__

#define GCCVER ( __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + \
	__GNUC_PATCHLEVEL__ )

#if GCCVER > 40600
#define CPP11
#endif

#endif


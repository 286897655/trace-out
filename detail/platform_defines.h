#pragma once


#if __cplusplus >= 201103L || _MSC_VER >= 1800
	#define PRETTY_OUTPUT_CPP11
#endif


#if defined(__clang__)
	#define PRETTY_OUTPUT_CLANG
	#define PRETTY_OUTPUT_POSIX
#elif defined(__GNUC__) || defined(__GNUG__)
	#define PRETTY_OUTPUT_GCC
	#define PRETTY_OUTPUT_POSIX
#elif defined(_MSC_VER)
	#define PRETTY_OUTPUT_MVS
	#define PRETTY_OUTPUT_WINDOWS
#endif


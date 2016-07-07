#pragma once


#if __cplusplus >= 201103L || _MSC_VER >= 1800
	#define TRACE_OUT_CPP11
#endif


#if defined(__clang__)
	#define TRACE_OUT_CLANG
	#define TRACE_OUT_POSIX
#elif defined(__MINGW32__)
	#define TRACE_OUT_MINGW
	#define TRACE_OUT_WINDOWS
#elif defined(__GNUC__) || defined(__GNUG__)
	#define TRACE_OUT_GCC
	#define TRACE_OUT_POSIX
#elif defined(_MSC_VER)
	#define TRACE_OUT_MVS
	#define TRACE_OUT_WINDOWS
#else
	#error Failed to detect compiler and platform
#endif


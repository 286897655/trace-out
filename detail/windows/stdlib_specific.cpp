#include "../platform_defines.hpp"
#if defined(TRACE_OUT_WINDOWS)

#include <cstdio>

#include "../stdlib_specific.hpp"


namespace trace_out { namespace detail
{

#if defined(TRACE_OUT_MVS)

	int vsnprintf(char *buffer, size_t size, const char *format, va_list arguments);

#endif // defined(TRACE_OUT_MVS)

}
}


namespace trace_out { namespace detail
{

	size_t printf_string_length(const char *format, va_list arguments)
	{
		int retval = _vscprintf(format, arguments);
		return static_cast<size_t>(retval);
	}


	size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments)
	{
		int retval = vsnprintf(buffer, size, format, arguments);
		return static_cast<size_t>(retval);
	}


#if defined(TRACE_OUT_MVS)

	int vsnprintf(char *buffer, size_t size, const char *format, va_list arguments)
	{
		return _vsnprintf_s(buffer, size, _TRUNCATE, format, arguments);
	}

#endif // defined(TRACE_OUT_MVS)

}
}


#endif // defined(TRACE_OUT_WINDOWS)


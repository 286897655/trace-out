#include "../platform_defines.hpp"
#if defined(TRACE_OUT_POSIX)

#include <cstdio>

#include "../stdlib_specific.hpp"


namespace trace_out { namespace detail
{

	size_t printf_string_length(const char *format, va_list arguments)
	{
		int retval = vsnprintf(NULL, 0, format, arguments);
		return static_cast<size_t>(retval);
	}


	size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments)
	{
		int retval = vsnprintf(buffer, size, format, arguments);
		return static_cast<size_t>(retval);
	}

}
}


#endif // defined(TRACE_OUT_POSIX)


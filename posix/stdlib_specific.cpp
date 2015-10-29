#if defined(PRETTY_OUTPUT_POSIX)

#include <cstdio>

#include "../implementation/stdlib_specific.h"


namespace pretty_output
{

	namespace impl
	{

		size_t printf_string_length(const char *format, va_list arguments)
		{
			return vsnprintf(NULL, 0, format, arguments);
		}


		size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments)
		{
			return vsnprintf(buffer, size, format, arguments);
		}

	}

}

#endif


#pragma once

#include <cstddef>
#include <cstdarg>


namespace pretty_output { namespace detail
{

	size_t printf_string_length(const char *format, va_list arguments);
	size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments);

}
}


#pragma once

#include <cstddef>


namespace pretty_output
{

	namespace impl
	{

#if defined(PRETTY_OUTPUT_INDENTATION)
		const char INDENTATION[] = PRETTY_OUTPUT_INDENTATION;
#else
		const char INDENTATION[] = "    ";
#endif

		const char THREAD_HEADER_SEPARATOR = '~';
		const char FILENAME_FIELD_EXCESS_PADDING[] = "~";
		const size_t FILENAME_FIELD_EXCESS_PADDING_SIZE = sizeof(FILENAME_FIELD_EXCESS_PADDING);
		const size_t FILENAME_FIELD_WIDTH = 20;
		const size_t LINE_FIELD_WIDTH = 4;
		const char DELIMITER[] = " |  ";
		const size_t INDENTATION_WIDTH = sizeof(INDENTATION) - 1;

	}

}


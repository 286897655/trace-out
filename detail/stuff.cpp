#include "platform_specific.h"
#include "constants.h"
#include "stuff.h"


namespace pretty_output { namespace detail
{

	const std::string filename_from_path(const char *path)
	{
		std::string file_path(path);
		return file_path.substr(file_path.rfind(FILE_PATH_COMPONENT_DELIMITER) + 1);
	}


	const std::string filename_line_field(const std::string &file, unsigned int line)
	{
		std::stringstream stream;
		stream.fill(' ');

		stream.width(FILENAME_FIELD_WIDTH);
		stream.flags(std::ios::right);
		std::string filename = file;
		if (filename.length() > FILENAME_FIELD_WIDTH)
		{
			filename = filename.substr(0, FILENAME_FIELD_WIDTH - FILENAME_FIELD_EXCESS_PADDING_SIZE);
			filename += FILENAME_FIELD_EXCESS_PADDING;
		}

		stream << filename;

		stream.width(0);
		stream << ":";

		stream.width(LINE_FIELD_WIDTH);
		stream.flags(std::ios::left);
		stream << line;

		return stream.str();
	}


	void apply_io_manipulators(std::ostream &stream, va_list manipulators)
	{
		for (;;)
		{
			manipulator_t manipulator = va_arg(manipulators, manipulator_t);
			if (manipulator == NULL)
			{
				break;
			}

			stream << manipulator;
		}
	}

}
}


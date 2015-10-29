#if defined(PRETTY_OUTPUT_WINDOWS)

#include <windows.h>

#include "../implementation/console.h"


namespace pretty_output
{

	namespace impl
	{

		int console_width()
		{
			CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

			BOOL retval = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen_buffer_info);
			if (retval == 0)
			{
				return -1;
			}

			int width = static_cast<int>(screen_buffer_info.srWindow.Right - screen_buffer_info.srWindow.Left);
			return width;
		}

	}

}

#endif


#include "../platform_defines.hpp"
#if defined(TRACE_OUT_WINDOWS)

#include <windows.h>

#include "../system_console.hpp"


namespace trace_out { namespace detail
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


#endif // defined(TRACE_OUT_WINDOWS)


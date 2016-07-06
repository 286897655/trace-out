#include "../platform_defines.hpp"
#if defined(TRACE_OUT_POSIX)

#include <sys/ioctl.h>
#include <unistd.h>

#include "../system_console.hpp"


namespace trace_out { namespace detail
{

	int console_width()
	{
		winsize window_size;
		int retval = ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);
		if (retval == -1)
		{
			return -1;
		}

		return static_cast<int>(window_size.ws_col) - 1;
	}

}
}


#endif // defined(TRACE_OUT_POSIX)


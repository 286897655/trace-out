#include "../platform_defines.h"
#if defined(PRETTY_OUTPUT_POSIX)

#include <sys/ioctl.h>
#include <unistd.h>

#include "../system_console.h"


namespace pretty_output { namespace detail
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


#endif


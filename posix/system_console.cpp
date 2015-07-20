//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include <sys/ioctl.h>
#include <unistd.h>

#include "../implementation/console.h"


namespace pretty_output
{

	namespace impl
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


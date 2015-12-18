#include "../platform_defines.h"
#if defined(PRETTY_OUTPUT_POSIX)

#include <cstddef>
#include <cassert>
#include <sys/time.h>

#include "../system_time.h"


namespace pretty_output { namespace detail
{

	uint64_t time_in_milliseconds()
	{
		struct timeval time_value;
		int retval = gettimeofday(&time_value, NULL);
		assert(retval == 0);

		uint64_t microseconds = time_value.tv_usec;
		uint64_t seconds = time_value.tv_sec;
		uint64_t milliseconds = seconds * 1000 + microseconds / 1000;

		return milliseconds;
	}

}
}


#endif


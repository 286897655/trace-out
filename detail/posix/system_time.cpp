#include "../platform_defines.hpp"
#if defined(TRACE_OUT_POSIX)

#include <cstddef>
#include <cassert>
#include <sys/time.h>

#include "../system_time.hpp"


namespace trace_out { namespace detail
{

	uint64_t time_in_milliseconds()
	{
		struct timeval time_value;
		int retval = gettimeofday(&time_value, NULL);
		assert(retval == 0);

		uint64_t microseconds = static_cast<uint64_t>(time_value.tv_usec);
		uint64_t seconds = static_cast<uint64_t>(time_value.tv_sec);
		uint64_t milliseconds = seconds * 1000 + microseconds / 1000;

		return milliseconds;
	}

}
}


#endif // defined(TRACE_OUT_POSIX)


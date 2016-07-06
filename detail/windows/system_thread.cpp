#include "../platform_defines.hpp"
#if defined(TRACE_OUT_WINDOWS)

#include <windows.h>

#include "../system_thread.hpp"


namespace trace_out { namespace detail
{

	uint64_t current_thread_id()
	{
		return static_cast<uint64_t>(GetCurrentThreadId());
	}

}
}


#endif // defined(TRACE_OUT_WINDOWS)


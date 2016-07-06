#include "../platform_defines.hpp"
#if defined(TRACE_OUT_POSIX)

#include <cstdint>
#include <pthread.h>

#include "../system_thread.hpp"


namespace trace_out { namespace detail
{

	uint64_t current_thread_id()
	{
		return reinterpret_cast<uint64_t>(pthread_self());
	}

}
}


#endif // defined(TRACE_OUT_POSIX)


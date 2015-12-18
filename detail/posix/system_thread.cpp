#include "../platform_defines.h"
#if defined(PRETTY_OUTPUT_POSIX)

#include <cstdint>
#include <pthread.h>

#include "../system_thread.h"


namespace pretty_output { namespace detail
{

	uint64_t current_thread_id()
	{
		return reinterpret_cast<uint64_t>(pthread_self());
	}

}
}


#endif


#if defined(PRETTY_OUTPUT_POSIX)

#include <stdint.h>
#include <pthread.h>

#include "../implementation/thread.h"


namespace pretty_output
{

	namespace impl
	{

		uint64_t current_thread_id()
		{
			return reinterpret_cast<uint64_t>(pthread_self());
		}

	}

}

#endif


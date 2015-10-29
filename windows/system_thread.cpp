#if defined(PRETTY_OUTPUT_WINDOWS)

#include <windows.h>

#include "../implementation/thread.h"


namespace pretty_output
{

	namespace impl
	{

		uint64_t current_thread_id()
		{
			return static_cast<uint64_t>(GetCurrentThreadId());
		}

	}

}

#endif


#include "../platform_defines.h"
#if defined(PRETTY_OUTPUT_WINDOWS)

#include <windows.h>

#include "../system_thread.h"


namespace pretty_output { namespace detail
{

	uint64_t current_thread_id()
	{
		return static_cast<uint64_t>(GetCurrentThreadId());
	}

}
}


#endif // defined(PRETTY_OUTPUT_WINDOWS)


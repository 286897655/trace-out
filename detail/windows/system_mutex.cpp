#include "../platform_defines.hpp"
#if defined(TRACE_OUT_WINDOWS)

#include <windows.h>

#include "../system_mutex.hpp"


namespace trace_out { namespace detail
{

	struct _mutex
	{
		CRITICAL_SECTION value;
	};


	mutex_t mutex_new()
	{
		mutex_t mutex = new _mutex;
		InitializeCriticalSection(&mutex->value);

		return mutex;
	}


	void mutex_delete(mutex_t mutex)
	{
		DeleteCriticalSection(&mutex->value);
	}


	void mutex_lock(mutex_t mutex)
	{
		EnterCriticalSection(&mutex->value);
	}


	void mutex_unlock(mutex_t mutex)
	{
		LeaveCriticalSection(&mutex->value);
	}

}
}


#endif // defined(TRACE_OUT_WINDOWS)


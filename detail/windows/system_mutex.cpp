#include "../platform_defines.h"
#if defined(PRETTY_OUTPUT_WINDOWS)

#include <windows.h>

#include "../system_mutex.h"


namespace pretty_output { namespace detail
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


#endif


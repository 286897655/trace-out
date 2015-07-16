//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include <windows.h>

#include "../implementation/mutex.h"


namespace pretty_output
{

	namespace impl
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


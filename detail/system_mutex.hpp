#pragma once

#include "platform_defines.hpp"
#include "resource.hpp"


namespace trace_out { namespace detail
{

	typedef struct _mutex *mutex_t;

	mutex_t mutex_new();
	void mutex_delete(mutex_t mutex);
	void mutex_lock(mutex_t mutex);
	void mutex_unlock(mutex_t mutex);


	class mutex
	{
	public:
		mutex();

		void lock();
		void unlock();

	private:
		mutex(const mutex &another);
		mutex &operator =(const mutex &another);

#if defined(TRACE_OUT_CPP11)

		mutex(mutex &&another);
		mutex &operator =(mutex &&another);

#endif // defined(TRACE_OUT_CPP11)

		resource<mutex_t> _handle;
	};

}
}


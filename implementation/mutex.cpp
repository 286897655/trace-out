#include "mutex.h"


namespace pretty_output
{

	namespace impl
	{

		mutex::mutex()
			: _handle(mutex_new(), mutex_delete)
		{
		}


		void mutex::lock()
		{
			mutex_lock(_handle.get());
		}


		void mutex::unlock()
		{
			mutex_unlock(_handle.get());
		}

	}

}


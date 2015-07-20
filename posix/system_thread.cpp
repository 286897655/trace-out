//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include <cstdint>
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


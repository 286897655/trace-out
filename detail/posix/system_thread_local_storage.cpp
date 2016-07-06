#include "../platform_defines.hpp"
#if defined(TRACE_OUT_POSIX)

#include <cstddef>
#include <cassert>
#include <pthread.h>

#include "../system_thread_local_storage.hpp"


namespace trace_out { namespace detail
{

	struct _tlskey
	{
		pthread_key_t value;
	};


	tlskey_t tls_new_key()
	{
		tlskey_t key = new _tlskey;
		int retval = pthread_key_create(&key->value, NULL);
		assert(retval == 0);

		return key;
	}


	void tls_delete_key(tlskey_t key)
	{
		int retval = pthread_key_delete(key->value);
		assert(retval == 0);

		delete key;
	}


	void *tls_get(tlskey_t key)
	{
		return pthread_getspecific(key->value);
	}


	void tls_set(tlskey_t key, void *data)
	{
		int retval = pthread_setspecific(key->value, data);
		assert(retval == 0);
	}

}
}


#endif // defined(TRACE_OUT_POSIX)


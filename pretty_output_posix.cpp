//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include <cstdio>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>

#include "pretty_output.h"


namespace pretty_output
{

	namespace impl
	{

		//
		// Platform specific

		uint64_t current_thread_id()
		{
			return reinterpret_cast<uint64_t>(pthread_self());
		}



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



		struct _mutex
		{
			pthread_mutex_t value;
		};


		mutex_t mutex_new()
		{
			mutex_t mutex = new _mutex;
			int retval = pthread_mutex_init(&mutex->value, NULL);
			assert(retval == 0);

			return mutex;
		}


		void mutex_delete(mutex_t mutex)
		{
			int retval = pthread_mutex_destroy(&mutex->value);
			assert(retval == 0);

			delete mutex;
		}


		void mutex_lock(mutex_t mutex)
		{
			int retval = pthread_mutex_lock(&mutex->value);
			assert(retval == 0);
		}


		void mutex_unlock(mutex_t mutex)
		{
			int retval = pthread_mutex_unlock(&mutex->value);
			assert(retval == 0);
		}


		size_t printf_string_length(const char *format, va_list arguments)
		{
			return vsnprintf(NULL, 0, format, arguments);
		}


		size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments)
		{
			return vsnprintf(buffer, size, format, arguments);
		}



		uint64_t time_in_milliseconds()
		{
			struct timeval time_value;
			int retval = gettimeofday(&time_value, NULL);
			assert(retval == 0);

			uint64_t microseconds = time_value.tv_usec;
			uint64_t seconds = time_value.tv_sec;
			uint64_t milliseconds = seconds * 1000 + microseconds / 1000;

			return milliseconds;
		}

	}

}


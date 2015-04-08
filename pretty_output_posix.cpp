//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include <cstdio>
#include <pthread.h>
#include <assert.h>

#include "pretty_output.h"


namespace pretty_output
{

	namespace impl
	{

		uint64_t current_thread_id()
		{
			return reinterpret_cast<uint64_t>(pthread_self());
		}



		struct tlskey_t
		{
			pthread_key_t value;
		};


		tlskey_t *tls_new_key()
		{
			tlskey_t *key = new tlskey_t;
			int retval = pthread_key_create(&key->value, NULL);
			assert(retval == 0);

			return key;
		}


		void tls_delete_key(tlskey_t *key)
		{
			int retval = pthread_key_delete(key->value);
			assert(retval == 0);

			delete key;
		}


		void *tls_get(tlskey_t *key)
		{
			return pthread_getspecific(key->value);
		}


		void tls_set(tlskey_t *key, void *data)
		{
			int retval = pthread_setspecific(key->value, data);
			assert(retval == 0);
		}



		struct mutex_t
		{
			pthread_mutex_t value;
		};


		mutex_t *mutex_new()
		{
			mutex_t *mutex = new mutex_t;
			pthread_mutex_init(&mutex->value, NULL);

			return mutex;
		}


		void mutex_delete(mutex_t *mutex)
		{
			pthread_mutex_destroy(&mutex->value);
			delete mutex;
		}


		void mutex_lock(mutex_t *mutex)
		{
			pthread_mutex_lock(&mutex->value);
		}


		void mutex_unlock(mutex_t *mutex)
		{
			pthread_mutex_unlock(&mutex->value);
		}


		size_t printf_string_length(const char *format, va_list arguments)
		{
			return vsnprintf(NULL, 0, format, arguments);
		}


		size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments)
		{
			return vsnprintf(buffer, size, format, arguments);
		}

	}

}


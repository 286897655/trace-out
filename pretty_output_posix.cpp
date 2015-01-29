#include <pthread.h>
#include <assert.h>

#include "pretty_output.h"


namespace pretty_output
{

	struct tlskey_t
	{
		pthread_key_t value;
	};


	tlskey_t *_tls_new_key()
	{
		tlskey_t *key = new tlskey_t;
		int retval = pthread_key_create(&key->value, NULL);
		assert(retval == 0);

		return key;
	}


	void _tls_delete_key(tlskey_t *key)
	{
		int retval = pthread_key_delete(key->value);
		assert(retval == 0);

		delete key;
	}


	void *_tls_get(tlskey_t *key)
	{
		return pthread_getspecific(key->value);
	}


	void _tls_set(tlskey_t *key, void *data)
	{
		int retval = pthread_setspecific(key->value, data);
		assert(retval == 0);
	}



	struct _mutex_t
	{
		pthread_mutex_t value;
	};


	mutex::mutex()
	{
		_handle = new _mutex_t;
		pthread_mutex_init(&_handle->value, NULL);
	}


	mutex::~mutex()
	{
		pthread_mutex_destroy(&_handle->value);
	}


	void mutex::lock()
	{
		pthread_mutex_lock(&_handle->value);
	}


	void mutex::unlock()
	{
		pthread_mutex_unlock(&_handle->value);
	}

}


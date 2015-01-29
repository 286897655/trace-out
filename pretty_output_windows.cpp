#include <windows.h>
#include <assert.h>


namespace pretty_output
{

	struct tlskey_t
	{
		DWORD value;
	};


	tlskey_t *_tls_new_key()
	{
		tlskey_t *key = new tlskey_t;
		DWORD retval = TlsAlloc();
		assert(retval != TLS_OUT_OF_INDEXES);

		key->value = retval;
		return key;
	}


	void _tls_delete_key(tlskey_t *key)
	{
		int retval = TlsFree(key->value);
		assert(retval != 0);

		delete key;
	}


	void *_tls_get(tlskey_t *key)
	{
		return TlsGetValue(key->value);
	}


	void _tls_set(tlskey_t *key, void *data)
	{
		BOOL retval = TlsSetValue(key->value, data);
		assert(retval != 0);
	}



	struct _mutex_t
	{
		CRITICAL_SECTION value;
	};


	mutex::mutex()
	{
		_handle = new _mutex_t;
		InitializeCriticalSection(&_handle->value, NULL);
	}


	mutex::~mutex()
	{
		DeleteCriticalSection(&_handle->value);
	}


	void mutex::lock()
	{
		EnterCriticalSection(&_handle->value);
	}


	void mutex::unlock()
	{
		LeaveCriticalSection(&_handle->value);
	}

}


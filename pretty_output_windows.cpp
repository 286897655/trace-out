#include <stdio.h>
#include <windows.h>
#include <assert.h>

#include "pretty_output.h"


namespace pretty_output
{

	uint64_t current_thread_id()
	{
		return static_cast<uint64_t>(GetCurrentThreadId());
	}



	struct tlskey_t
	{
		DWORD value;
	};


	tlskey_t *tls_new_key()
	{
		tlskey_t *key = new tlskey_t;
		DWORD retval = TlsAlloc();
		assert(retval != TLS_OUT_OF_INDEXES);

		key->value = retval;
		return key;
	}


	void tls_delete_key(tlskey_t *key)
	{
		int retval = TlsFree(key->value);
		assert(retval != 0);

		delete key;
	}


	void *tls_get(tlskey_t *key)
	{
		return TlsGetValue(key->value);
	}


	void tls_set(tlskey_t *key, void *data)
	{
		BOOL retval = TlsSetValue(key->value, data);
		assert(retval != 0);
	}



	struct mutex_t
	{
		CRITICAL_SECTION value;
	};


	mutex_t *mutex_new()
	{
		mutex_t *mutex = new mutex_t;
		InitializeCriticalSection(&mutex->value);

		return mutex;
	}


	void mutex_delete(mutex_t *mutex)
	{
		DeleteCriticalSection(&mutex->value);
	}


	void mutex_lock(mutex_t *mutex)
	{
		EnterCriticalSection(&mutex->value);
	}


	void mutex_unlock(mutex_t *mutex)
	{
		LeaveCriticalSection(&mutex->value);
	}


	size_t printf_string_length(const char *format, va_list arguments)
	{
		int retval = _vscprintf(format, arguments);
		return static_cast<size_t>(retval);
	}


	size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments)
	{
		int retval = _vsnprintf_s(buffer, size, _TRUNCATE, format, arguments);
		return static_cast<size_t>(retval);
	}

}


#include <windows.h>
#include <assert.h>

#include "pretty_output.h"


namespace pretty_output
{

	std::uint64_t current_thread_id()
	{
		return (std::uint64_t)GetCurrentThreadId();
	}



	struct _tlskey_t
	{
		DWORD value;
	};


	_tlskey_t *_tls_new_key()
	{
		_tlskey_t *key = new _tlskey_t;
		DWORD retval = TlsAlloc();
		assert(retval != TLS_OUT_OF_INDEXES);

		key->value = retval;
		return key;
	}


	void _tls_delete_key(_tlskey_t *key)
	{
		int retval = TlsFree(key->value);
		assert(retval != 0);

		delete key;
	}


	void *_tls_get(_tlskey_t *key)
	{
		return TlsGetValue(key->value);
	}


	void _tls_set(_tlskey_t *key, void *data)
	{
		BOOL retval = TlsSetValue(key->value, data);
		assert(retval != 0);
	}



	struct _mutex_t
	{
		CRITICAL_SECTION value;
	};


	_mutex_t *_mutex_new()
	{
		_mutex_t *mutex = new _mutex_t;
		InitializeCriticalSection(&mutex->value);

		return mutex;
	}


	void _mutex_delete(_mutex_t *mutex)
	{
		DeleteCriticalSection(&mutex->value);
	}


	void _mutex_lock(_mutex_t *mutex)
	{
		EnterCriticalSection(&mutex->value);
	}


	void _mutex_unlock(_mutex_t *mutex)
	{
		LeaveCriticalSection(&mutex->value);
	}


	std::size_t printf_string_length(const char *format, va_list arguments)
	{
		return _vscprintf(format, arguments);
	}


	std::size_t printf_to_string(char *buffer, std::size_t size, const char *format, va_list arguments)
	{
		return _vsnprintf_s(buffer, size, _TRUNCATE, format, arguments);
	}

}


//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include <stdio.h>
#include <windows.h>
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
			return static_cast<uint64_t>(GetCurrentThreadId());
		}



		struct _tlskey_t
		{
			DWORD value;
		};


		tlskey_t tls_new_key()
		{
			tlskey_t key = new _tlskey_t;
			DWORD retval = TlsAlloc();
			assert(retval != TLS_OUT_OF_INDEXES);

			key->value = retval;
			return key;
		}


		void tls_delete_key(tlskey_t key)
		{
			int retval = TlsFree(key->value);
			assert(retval != 0);

			delete key;
		}


		void *tls_get(tlskey_t key)
		{
			return TlsGetValue(key->value);
		}


		void tls_set(tlskey_t key, void *data)
		{
			BOOL retval = TlsSetValue(key->value, data);
			assert(retval != 0);
		}



		struct _mutex_t
		{
			CRITICAL_SECTION value;
		};


		mutex_t mutex_new()
		{
			mutex_t mutex = new _mutex_t;
			InitializeCriticalSection(&mutex->value);

			return mutex;
		}


		void mutex_delete(mutex_t mutex)
		{
			DeleteCriticalSection(&mutex->value);
		}


		void mutex_lock(mutex_t mutex)
		{
			EnterCriticalSection(&mutex->value);
		}


		void mutex_unlock(mutex_t mutex)
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



		uint64_t time_in_milliseconds()
		{
			FILETIME file_time;
			ULARGE_INTEGER large_integer;

			GetSystemTimeAsFileTime(&file_time);
			large_integer.LowPart = file_time.dwLowDateTime;
			large_integer.HighPart = file_time.dwHighDateTime;

			uint64_t milliseconds = (large_integer.QuadPart - 116444736000000000LL) / 10000;

			return milliseconds;
		}

	}

}


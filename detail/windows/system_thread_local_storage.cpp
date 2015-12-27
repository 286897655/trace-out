#include "../platform_defines.h"
#if defined(PRETTY_OUTPUT_WINDOWS)

#include <cassert>
#include <windows.h>

#include "../system_thread_local_storage.h"


namespace pretty_output { namespace detail
{

	struct _tlskey
	{
		DWORD value;
	};


	tlskey_t tls_new_key()
	{
		tlskey_t key = new _tlskey;
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

}
}


#endif // defined(PRETTY_OUTPUT_WINDOWS)


#include <assert.h>

#include "thread_local_storage.h"
#include "pretty_output.h"


namespace pretty_output
{

	static pthread_once_t once_control = PTHREAD_ONCE_INIT;
	static pthread_key_t indentation_pthread_key;


	tls<std::string> _indentation;


	const std::string &indentation()
	{
		return _indentation.get();
	}


	void indentation_add()
	{
		_indentation.set(_indentation.get() + INDENTATION);
	}


	void indentation_remove()
	{
		const std::string &old_indentation = _indentation.get();
		_indentation.set(old_indentation.substr(0, old_indentation.length() - INDENTATION_SIZE));
	}

}


#include <assert.h>
#include <mutex>

#include "thread_local_storage.h"
#include "pretty_output.h"


namespace pretty_output
{

	static pthread_once_t once_control = PTHREAD_ONCE_INIT;
	static pthread_key_t indentation_pthread_key;


	tls<std::string> _indentation;
	uint64_t _current_thread_id;
	std::mutex _output_mutex;


	uint64_t current_thread_id()
	{
		return (uint64_t)pthread_self();
	}


	bool is_running_same_thread()
	{
		if (_current_thread_id != current_thread_id())
		{
			_current_thread_id = current_thread_id();
			return false;
		}

		return true;
	}


	void lock_output()
	{
		_output_mutex.lock();
	}


	void unlock_output()
	{
		_output_mutex.unlock();
	}


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


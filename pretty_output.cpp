#include "pretty_output.h"


namespace pretty_output
{

	template <typename T>
	class tls
	{
	public:
		tls()
		{
			_key = tls_new_key();
		}


		~tls()
		{
			tls_delete_key(_key);
		}


		void set(const T &value)
		{
			T *old_value = (T*)tls_get(_key);
			if (old_value != NULL)
			{
				delete old_value;
			}

			T *new_value = new T(value);
			tls_set(_key, new_value);
		}


		T &get() const
		{
			T *value = (T*)tls_get(_key);
			if (value == NULL)
			{
				value = new T;
				tls_set(_key, value);
			}

			return *value;
		}

	private:
		tlskey_t *_key;
	};



	class mutex
	{
	public:
		mutex()
		{
			_handle = mutex_new();
		}


		~mutex()
		{
			mutex_delete(_handle);
		}


		void lock()
		{
			mutex_lock(_handle);
		}


		void unlock()
		{
			mutex_unlock(_handle);
		}

	private:
		mutex_t *_handle;
	};



	tls<std::string> _indentation;
	uint64_t _current_thread_id;
	tls<std::string> _thread_name;
	mutex _output_mutex;


	const std::string current_thread_name()
	{
		return _thread_name.get();
	}


	void set_current_thread_name(const std::string &name)
	{
		_thread_name.set(name);
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
#if !defined(PRETTY_OUTPUT_NO_OUTPUT_SYNC)
		_output_mutex.lock();
#endif // PRETTY_OUTPUT_NO_OUTPUT_SYNC
	}


	void unlock_output()
	{
#if !defined(PRETTY_OUTPUT_NO_OUTPUT_SYNC)
		_output_mutex.unlock();
#endif // PRETTY_OUTPUT_NO_OUTPUT_SYNC
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
		_indentation.set(old_indentation.substr(0, old_indentation.length() - INDENTATION_WIDTH));
	}

}


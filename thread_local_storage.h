#include <pthread.h>
#include <assert.h>


namespace pretty_output
{

	template <typename T>
	class tls
	{
	public:
		tls()
		{
			int retval = pthread_key_create(&_key, NULL);
			assert(retval == 0);
		}


		~tls()
		{
		}


		void set(const T &value)
		{
			T *old_value = (T*)pthread_getspecific(_key);
			if (old_value != NULL)
			{
				delete old_value;
			}

			T *new_value = new T(value);
			int retval = pthread_setspecific(_key, new_value);
			assert(retval == 0);
		}


		T &get() const
		{
			T *value = (T*)pthread_getspecific(_key);
			if (value == NULL)
			{
				value = new T;
				pthread_setspecific(_key, value);
			}

			return *value;
		}


	private:
		pthread_key_t _key;
	};

}


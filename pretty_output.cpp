#include <assert.h>

#include "pretty_output.h"


namespace pretty_output
{

	static pthread_once_t once_control = PTHREAD_ONCE_INIT;
	static pthread_key_t indentation_pthread_key;


	void _make_indentation_key()
	{
		int retval = pthread_key_create(&indentation_pthread_key, NULL);
		assert(retval == 0);
	}


	const std::string &indentation()
	{
		pthread_once(&once_control, _make_indentation_key);

		std::string *indentation = (std::string*)pthread_getspecific(indentation_pthread_key);
		if (indentation == NULL)
		{
			indentation = new std::string("");
		}

		return *indentation;
	}


	void indentation_add()
	{
		pthread_once(&once_control, _make_indentation_key);

		std::string *indentation = (std::string*)pthread_getspecific(indentation_pthread_key);
		if (indentation == NULL)
		{
			indentation = new std::string("");
		}

		int retval = pthread_setspecific(indentation_pthread_key, new std::string(*indentation + INDENTATION));
		assert(retval == 0);
	}


	void indentation_remove()
	{
		pthread_once(&once_control, _make_indentation_key);

		std::string *indentation = (std::string*)pthread_getspecific(indentation_pthread_key);
		if (indentation == NULL)
		{
			indentation = new std::string("");
		}

		int retval = pthread_setspecific(indentation_pthread_key, new std::string(indentation->substr(0, indentation->length() - INDENTATION_SIZE)));
		assert(retval == 0);
	}

}


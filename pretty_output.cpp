#include "pretty_output.h"


namespace pretty_output
{

	std::string _indentation = "";


	const std::string &indentation()
	{
		return _indentation;
	}


	void indentation_add()
	{
		_indentation += INDENTATION;
	}


	void indentation_remove()
	{
		_indentation = _indentation.substr(0, _indentation.length() - INDENTATION_SIZE);
	}

}


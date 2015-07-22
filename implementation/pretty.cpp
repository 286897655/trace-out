//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include "pretty.h"


namespace pretty_output
{

	namespace impl
	{

#if defined(PRETTY_OUTPUT_CPP11)

		pretties<>::pretties(const char *)
		{
		}


		pretties<>::pretties(const pretties &)
		{
		}

#endif // defined(PRETTY_OUTPUT_CPP11)

	}

}


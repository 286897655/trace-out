//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include "pretty.h"


namespace pretty_output
{

	namespace impl
	{

		pretty<const char *> make_pretty(const char *const &value)
		{
			return pretty<const char *>(value);
		}


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


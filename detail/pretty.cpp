#include "pretty.h"


namespace pretty_output { namespace detail
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


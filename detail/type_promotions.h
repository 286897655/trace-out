#pragma once

#include "platform_defines.h"
#include "stuff.h"


namespace pretty_output { namespace detail
{

	template <typename Type_t, bool Is_fundamental>
	struct promote_to_fundamental;


	template <typename Type_t>
	struct promote_to_fundamental<Type_t, false>
	{
		typedef void type;
	};


	template <typename Type_t>
	struct promote_to_fundamental<Type_t, true>
	{
		typedef Type_t type;
	};



	template <typename Type_t>
	struct promote
	{
		typedef Type_t type;
	};


	template <typename Type_t>
	struct promote<const Type_t *>
	{
		typedef const typename promote_to_fundamental<const Type_t, is_fundamental<Type_t>::value>::type *type;
	};

}
}


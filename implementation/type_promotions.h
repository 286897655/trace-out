#pragma once

#include "defines.h"


namespace pretty_output
{

	namespace impl
	{

		template <typename Type_t>
		struct is_fundamental
		{
			static const bool value = false;
		};


#define PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(type) \
			template <> \
			struct is_fundamental<type> \
			{ \
				static const bool value = true; \
			}


		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(char);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed char);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned char);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed short int);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned short int);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed int);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned int);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed long int);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned long int);

#if defined(PRETTY_OUTPUT_CPP11)

		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed long long);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned long long);

#endif // defined(PRETTY_OUTPUT_CPP11)

		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(float);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(double);
		PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(long double);

#undef PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL


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


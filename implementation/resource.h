#pragma once

#include "defines.h"


namespace pretty_output
{

	namespace impl
	{

		template <typename Type_t>
		class resource
		{
		public:
			typedef void (*deleter_t)(Type_t);

			resource(Type_t handle, deleter_t deleter);
			~resource();

			const Type_t &get() const;

		private:
			resource();
			resource(const resource &);
			resource &operator =(const resource &);

#if defined(PRETTY_OUTPUT_CPP11)

			resource(resource &&another);
			resource &operator =(resource &&another);

#endif // defined(PRETTY_OUTPUT_CPP11)

			Type_t _handle;
			deleter_t _deleter;
		};

	}

}


namespace pretty_output
{

	namespace impl
	{

		template <typename Type_t>
		resource<Type_t>::resource(Type_t handle, deleter_t deleter)
			: _handle(handle), _deleter(deleter)
		{
		}


		template <typename Type_t>
		resource<Type_t>::~resource()
		{
			_deleter(_handle);
		}


		template <typename Type_t>
		const Type_t &resource<Type_t>::get() const
		{
			return _handle;
		}

	}

}


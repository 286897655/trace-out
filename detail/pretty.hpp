#pragma once

#include <string>

#include "platform_defines.hpp"
#include "stuff.hpp"
#include "type_promotions.hpp"


namespace trace_out { namespace detail
{

	template <typename Type_t>
	class pretty
	{
	public:
		pretty(const Type_t &data);
		pretty(const pretty &another);

		const Type_t &get() const;
		const Type_t &unsafe_get() const;

	private:
		pretty &operator =(const pretty &another); // = delete

#if defined(TRACE_OUT_CPP11)

		pretty &operator =(pretty &&another); // = delete

#endif // defined(TRACE_OUT_CPP11)

		const Type_t &_data;
	};



	template <typename Type_t>
	class pretty_condition
	{
	public:
		pretty_condition(const Type_t &data);
		pretty_condition(const pretty_condition &another);

		const Type_t &get() const;

	private:
		pretty_condition &operator =(const pretty_condition &another); // = delete

#if defined(TRACE_OUT_CPP11)

		pretty_condition &operator =(pretty_condition &&another); // = delete

#endif // defined(TRACE_OUT_CPP11)

		const Type_t &_data;
	};


	template <typename Type_t>
	pretty_condition<Type_t> make_pretty_condition(const Type_t &value);


	template <typename Type_t>
	class pretty_compound
	{
	public:
		pretty_compound(const Type_t &data);
		pretty_compound(const pretty_compound &another);

		const Type_t &get() const;
		const Type_t &unsafe_get() const;

	private:
		pretty_compound &operator =(const pretty_compound &another); // = delete

#if defined(TRACE_OUT_CPP11)

		pretty_compound &operator =(pretty_compound &&another); // = delete

#endif // defined(TRACE_OUT_CPP11)

		const Type_t &_data;
	};



	template <typename Type_t>
	typename enable_if<is_dimensional<Type_t>::value, pretty_compound<typename promote<Type_t>::type> >::type make_pretty(const Type_t &value)
	{
		typedef typename promote<Type_t>::type promoted_t;

		return pretty_compound<promoted_t>(reinterpret_cast<const promoted_t &>(value));
	}


	template <typename Type_t>
	typename enable_if<!is_dimensional<Type_t>::value, pretty<typename promote<Type_t>::type> >::type make_pretty(const Type_t &value)
	{
		typedef typename promote<Type_t>::type promoted_t;

		return pretty<promoted_t>(reinterpret_cast<const promoted_t &>(value));
	}

}
}


namespace trace_out { namespace detail
{

	template <typename Type_t>
	pretty<Type_t>::pretty(const Type_t &data)
		:
		_data(data)
	{
	}


	template <typename Type_t>
	pretty<Type_t>::pretty(const pretty &another)
		:
		_data(another._data)
	{
	}


	template <typename Type_t>
	const Type_t &pretty<Type_t>::get() const
	{
		crash_on_bad_memory(_data);

		return _data;
	}


	template <typename Type_t>
	const Type_t &pretty<Type_t>::unsafe_get() const
	{
		return _data;
	}



	template <typename Type_t>
	pretty_condition<Type_t>::pretty_condition(const Type_t &data)
		:
		_data(data)
	{
	}


	template <typename Type_t>
	pretty_condition<Type_t>::pretty_condition(const pretty_condition &another)
		:
		_data(another._data)
	{
	}


	template <typename Type_t>
	const Type_t &pretty_condition<Type_t>::get() const
	{
		return _data;
	}


	template <typename Type_t>
	pretty_condition<Type_t> make_pretty_condition(const Type_t &value)
	{
		return pretty_condition<Type_t>(value);
	}



	template <typename Type_t>
	pretty_compound<Type_t>::pretty_compound(const Type_t &data)
		:
		_data(data)
	{
	}


	template <typename Type_t>
	pretty_compound<Type_t>::pretty_compound(const pretty_compound &another)
		:
		_data(another._data)
	{
	}


	template <typename Type_t>
	const Type_t &pretty_compound<Type_t>::get() const
	{
		crash_on_bad_memory(_data);

		return _data;
	}


	template <typename Type_t>
	const Type_t &pretty_compound<Type_t>::unsafe_get() const
	{
		return _data;
	}

}
}


#pragma once

#include "platform_defines.h"
#include "stuff.h"
#include "type_promotions.h"


namespace pretty_output { namespace detail
{

	template <typename Type_t>
	class pretty
	{
	public:
		pretty(const Type_t &data);
		pretty(const pretty &another);

		const Type_t &get() const;

	private:
		pretty &operator =(const pretty &another); // = delete

#if defined(PRETTY_OUTPUT_CPP11)

		pretty &operator =(pretty &&another); // = delete

#endif // defined(PRETTY_OUTPUT_CPP11)

		const Type_t &_data;
	};


	template <typename Type_t>
	pretty<typename promote<Type_t>::type> make_pretty(const Type_t &value);


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename ...>
	class pretties;


	template <typename Type_t, typename ...RestTypes_t>
	class pretties<Type_t, RestTypes_t...>
	{
	public:
		pretties(const char *delimiter, const Type_t &first, const RestTypes_t &...rest);
		pretties(const pretties &another);

		const char *delimiter() const;
		const pretty<Type_t> &first() const;
		const pretties<RestTypes_t...> &rest() const;

	private:
		pretties &operator =(const pretties &another); // = delete
		pretties &operator =(pretties &&another); // = delete

		const char *_delimiter;
		const pretty<Type_t> _first;
		pretties<RestTypes_t...> _rest;
	};


	template <typename Type_t>
	class pretties<Type_t>
	{
	public:
		pretties(const char *delimiter, const Type_t &first);
		pretties(const pretties &another);

		const char *delimiter() const;
		const pretty<Type_t> &first() const;

	private:
		pretties &operator =(const pretties &another); // = delete
		pretties &operator =(pretties &&another); // = delete

		const char *_delimiter;
		const pretty<Type_t> _first;
	};


	template <>
	class pretties<>
	{
	public:
		pretties(const char *delimiter);
		pretties(const pretties &another);

	private:
		pretties &operator =(const pretties &another); // = delete
		pretties &operator =(pretties &&another); // = delete
	};


	template <typename ...Types_t>
	pretties<Types_t...> make_pretties(const char *delimiter, const Types_t &...values);

#endif // defined(PRETTY_OUTPUT_CPP11)


	template <typename Type_t>
	class pretty_bool
	{
	public:
		pretty_bool(const Type_t &data);
		pretty_bool(const pretty_bool &another);

		const Type_t &get() const;

	private:
		pretty_bool &operator =(const pretty_bool &another); // = delete

#if defined(PRETTY_OUTPUT_CPP11)

		pretty_bool &operator =(pretty_bool &&another); // = delete

#endif // defined(PRETTY_OUTPUT_CPP11)

		const Type_t &_data;
	};


	template <typename Type_t>
	pretty_bool<Type_t> make_pretty_bool(const Type_t &value);

}
}


namespace pretty_output { namespace detail
{

	template <typename Type_t>
	pretty<Type_t>::pretty(const Type_t &data) :
		_data(data)
	{
	}


	template <typename Type_t>
	pretty<Type_t>::pretty(const pretty &another) :
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
	pretty<typename promote<Type_t>::type> make_pretty(const Type_t &value)
	{
		typedef typename promote<Type_t>::type promoted_t;

		return pretty<promoted_t>(reinterpret_cast<const promoted_t &>(value));
	}


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t, typename ...RestTypes_t>
	pretties<Type_t, RestTypes_t...>::pretties(const char *delimiter, const Type_t &first, const RestTypes_t &...rest) :
		_delimiter(delimiter),
		_first(first),
		_rest(delimiter, rest...)
	{
	}


	template <typename Type_t, typename ...RestTypes_t>
	pretties<Type_t, RestTypes_t...>::pretties(const pretties &another) :
		_delimiter(another._delimiter),
		_first(another._first),
		_rest(another._rest)
	{
	}


	template <typename Type_t, typename ...RestTypes_t>
	const char *pretties<Type_t, RestTypes_t...>::delimiter() const
	{
		return _delimiter;
	}


	template <typename Type_t, typename ...RestTypes_t>
	const pretty<Type_t> &pretties<Type_t, RestTypes_t...>::first() const
	{
		return _first;
	}


	template <typename Type_t, typename ...RestTypes_t>
	const pretties<RestTypes_t...> &pretties<Type_t, RestTypes_t...>::rest() const
	{
		return _rest;
	}



	template <typename Type_t>
	pretties<Type_t>::pretties(const char *, const Type_t &first) :
		_delimiter(""),
		_first(first)
	{
	}


	template <typename Type_t>
	pretties<Type_t>::pretties(const pretties &another) :
		_delimiter(another._delimiter),
		_first(another._first)
	{
	}


	template <typename Type_t>
	const char *pretties<Type_t>::delimiter() const
	{
		return _delimiter;
	}


	template <typename Type_t>
	const pretty<Type_t> &pretties<Type_t>::first() const
	{
		return _first;
	}



	template <typename ...Types_t>
	pretties<Types_t...> make_pretties(const char *delimiter, const Types_t &...values)
	{
		return pretties<Types_t...>(delimiter, values...);
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


	template <typename Type_t>
	pretty_bool<Type_t>::pretty_bool(const Type_t &data) :
		_data(data)
	{
	}


	template <typename Type_t>
	pretty_bool<Type_t>::pretty_bool(const pretty_bool &another) :
		_data(another._data)
	{
	}


	template <typename Type_t>
	const Type_t &pretty_bool<Type_t>::get() const
	{
		return _data;
	}


	template <typename Type_t>
	pretty_bool<Type_t> make_pretty_bool(const Type_t &value)
	{
		return pretty_bool<Type_t>(value);
	}

}
}


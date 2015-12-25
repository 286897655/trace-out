#pragma once

#include <string>

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
	class watches;


	template <typename Type_t, typename ...Types_t>
	class watches<Type_t, Types_t...>
	{
	public:
		watches(const std::string &names, const Type_t &first, const Types_t &...rest);
		watches(const watches<Type_t, Types_t...> &another);

		const std::string &first_name() const;
		const pretty<Type_t> &first_pretty() const;
		const watches<Types_t...> &rest() const;

	private:
		std::string _first_name;
		pretty<Type_t> _first_pretty;
		watches<Types_t...> _rest;

		watches<Type_t, Types_t...> &operator =(const watches<Type_t, Types_t...> &) = delete;
		watches<Type_t, Types_t...> &operator =(watches<Type_t, Types_t...> &&) = delete;
	};


	template <typename Type_t>
	class watches<Type_t>
	{
	public:
		watches(const std::string &name, const Type_t &first);
		watches(const watches<Type_t> &another);

		const std::string &first_name() const;
		const pretty<Type_t> &first_pretty() const;

	private:
		std::string _first_name;
		pretty<Type_t> _first_pretty;

		watches<Type_t> &operator =(const watches<Type_t> &) = delete;
		watches<Type_t> &operator =(watches<Type_t> &&) = delete;
	};


	template <typename ...Types_t>
	watches<Types_t...> make_watches(const std::string &names, const Types_t &...arguments);

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

	template <typename Type_t, typename ...Types_t>
	watches<Type_t, Types_t...>::watches(const std::string &names, const Type_t &first, const Types_t &...rest) :
		_first_name(first_token(names)),
		_first_pretty(first),
		_rest(rest_tokens(names), rest...)
	{
	}


	template <typename Type_t, typename ...Types_t>
	watches<Type_t, Types_t...>::watches(const watches<Type_t, Types_t...> &another) :
		_first_name(another._first_name),
		_first_pretty(another._first_pretty),
		_rest(another._rest)
	{
	}


	template <typename Type_t, typename ...Types_t>
	const std::string &watches<Type_t, Types_t...>::first_name() const
	{
		return _first_name;
	}


	template <typename Type_t, typename ...Types_t>
	const pretty<Type_t> &watches<Type_t, Types_t...>::first_pretty() const
	{
		return _first_pretty;
	}


	template <typename Type_t, typename ...Types_t>
	const watches<Types_t...> &watches<Type_t, Types_t...>::rest() const
	{
		return _rest;
	}



	template <typename Type_t>
	watches<Type_t>::watches(const std::string &name, const Type_t &first) :
		_first_name(name),
		_first_pretty(first)
	{
	}


	template <typename Type_t>
	watches<Type_t>::watches(const watches<Type_t> &another) :
		_first_name(another._first_name),
		_first_pretty(another._first_pretty)
	{
	}


	template <typename Type_t>
	const std::string &watches<Type_t>::first_name() const
	{
		return _first_name;
	}


	template <typename Type_t>
	const pretty<Type_t> &watches<Type_t>::first_pretty() const
	{
		return _first_pretty;
	}



	template <typename ...Types_t>
	watches<Types_t...> make_watches(const std::string &names, const Types_t &...arguments)
	{
		return watches<Types_t...>(names, arguments...);
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


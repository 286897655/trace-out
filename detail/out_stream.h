#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "platform_defines.h"
#if defined(PRETTY_OUTPUT_CPP11)
	#include <tuple>
#endif

#include "stuff.h"
#include "pretty.h"


namespace pretty_output { namespace detail
{

#define PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(field_name) \
			template <typename Struct_t> \
			struct has_##field_name \
			{ \
				struct fallback \
				{ \
					int field_name; \
				}; \
				\
				struct dummy \
				{ \
				}; \
				\
				struct derived \
					: conditional<!is_primitive<Struct_t>::value, Struct_t, dummy>::type, fallback \
				{ \
				}; \
				\
				template <typename Type_t, Type_t> \
				struct check; \
				\
				template <typename Type_t> \
				static char (&function(check<int fallback::*, &Type_t::field_name> *))[1]; \
				\
				template <typename Type_t> \
				static char (&function(...))[2]; \
				\
				enum \
				{ \
					value = sizeof(function<derived>(0)) == 2 \
				}; \
			}


	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(x);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(y);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(z);

	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(width);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(height);

	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(origin);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(size);

	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(X);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(Y);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(Z);

	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(WIDTH);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(HEIGHT);

	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(ORIGIN);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(SIZE);

	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(Width);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(Height);

	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(Origin);
	PRETTY_OUTPUT_PRIVATE__DEFINE_HAS_FIELD(Size);

#undef PRETTY_OUTPUT_PRIVATE_DEFINE_HAS_FIELD


	const std::string &indentation();
	void indentation_add();
	void indentation_remove();

	void set_current_thread_name(const std::string &name);

	class newline_manipulator;
	class endline_manipulator;
	class flush_manipulator;

	extern const newline_manipulator NEWLINE;
	extern const endline_manipulator ENDLINE;
	extern const flush_manipulator FLUSH;


	class out_stream
	{
	public:
		out_stream(const std::string &filename_line);
		out_stream();
		~out_stream();

		out_stream &operator <<(char character);
		out_stream &operator <<(const char *string);
		out_stream &operator <<(const std::string &string);
		out_stream &operator <<(const newline_manipulator &);
		out_stream &operator <<(const endline_manipulator &);
		out_stream &operator <<(const flush_manipulator &);
		size_t width_left() const;
		void printf(const char *format, ...);
		void flush();

		static size_t width();

	private:
		size_t _current_line_length;
	};


	out_stream &operator <<(out_stream &stream, const pretty<bool> &value);
	out_stream &operator <<(out_stream &stream, const pretty<char> &value);
	out_stream &operator <<(out_stream &stream, const pretty<unsigned char> &value);
	out_stream &operator <<(out_stream &stream, const pretty<const char *> &value);
	out_stream &operator <<(out_stream &stream, const pretty<std::string> &value);
	out_stream &operator <<(out_stream &stream, const pretty<short> &value);
	out_stream &operator <<(out_stream &stream, const pretty<unsigned short> &value);
	out_stream &operator <<(out_stream &stream, const pretty<int> &value);
	out_stream &operator <<(out_stream &stream, const pretty<unsigned int> &value);
	out_stream &operator <<(out_stream &stream, const pretty<long> &value);
	out_stream &operator <<(out_stream &stream, const pretty<unsigned long> &value);

#if defined(PRETTY_OUTPUT_CPP11)

	out_stream &operator <<(out_stream &stream, const pretty<long long> &value);
	out_stream &operator <<(out_stream &stream, const pretty<unsigned long long> &value);

#endif // defined(PRETTY_OUTPUT_CPP11)

	out_stream &operator <<(out_stream &stream, const pretty<float> &value);
	out_stream &operator <<(out_stream &stream, const pretty<double> &value);
	out_stream &operator <<(out_stream &stream, const pretty<long double> &value);
	out_stream &operator <<(out_stream &stream, const pretty<const void *> &value);

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<const Type_t *> &value);

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<Type_t *> &value);

	template <typename Type_t>
	typename enable_if<has_x<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value);

	template <typename Type_t>
	typename enable_if<has_width<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value);

	template <typename Type_t>
	typename enable_if<has_origin<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value);

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::auto_ptr<Type_t> > &value);

#if defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::shared_ptr<Type_t> > &value);

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::unique_ptr<Type_t> > &value);

#endif // defined(PRETTY_OUTPUT_CPP11)

	template <typename First_t, typename Second_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::pair<First_t, Second_t> > &value);

#if defined(PRETTY_OUTPUT_CPP11)

	template <typename ...Types_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::tuple<Types_t ...> > &value);

	template <template <typename ...> class Container, typename ...Parameters_t>
	out_stream &operator <<(out_stream &stream, const pretty<Container<Parameters_t ...> > &value);

#endif // defined(PRETTY_OUTPUT_CPP11)


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const watches<Type_t> &values);

	template <typename ...Types_t>
	out_stream &operator <<(out_stream &stream, const watches<Types_t ...> &values);

#endif // defined(PRETTY_OUTPUT_CPP11)

	out_stream &operator <<(out_stream &stream, const pretty_bool<bool> &value);

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty_bool<Type_t> &value);

}
}


namespace pretty_output { namespace detail
{

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<const Type_t *> &value)
	{
		stream << FLUSH;
		stream << make_pretty(static_cast<const void *>(value.get())) << " ";
		if (value.get() != NULL)
		{
			stream << FLUSH;
			stream << "-> " << make_pretty(*(value.get()));
		}

		return stream;
	}


	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<Type_t *> &value)
	{
		stream << FLUSH;
		return stream << make_pretty(static_cast<const Type_t *>(value.get()));
	}


	template <typename Type_t>
	typename enable_if<has_z<Type_t>::value, out_stream &>::type print_z(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &point = value.unsafe_get();
		return stream << ", " << make_pretty(point.z) << ")";
	}


	template <typename Type_t>
	typename enable_if<has_Z<Type_t>::value, out_stream &>::type print_z(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &point = value.unsafe_get();
		return stream << ", " << make_pretty(point.Z) << ")";
	}


	template <typename Type_t>
	typename enable_if<!(has_z<Type_t>::value || has_Z<Type_t>::value), out_stream &>::type print_z(out_stream &stream, const pretty<Type_t> &)
	{
		return stream << ")";
	}


	template <typename Type_t>
	typename enable_if<has_y<Type_t>::value, out_stream &>::type print_y(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &point = value.unsafe_get();
		stream << ", " << make_pretty(point.y);
		return print_z(stream, value);
	}


	template <typename Type_t>
	typename enable_if<has_Y<Type_t>::value, out_stream &>::type print_y(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &point = value.unsafe_get();
		stream << ", " << make_pretty(point.Y);
		return print_z(stream, value);
	}


	template <typename Type_t>
	typename enable_if<has_x<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &point = value.get();
		stream << "(" << make_pretty(point.x);
		return print_y(stream, value);
	}


	template <typename Type_t>
	typename enable_if<has_X<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &point = value.get();
		stream << "(" << make_pretty(point.X);
		return print_y(stream, value);
	}


	template <typename Type_t>
	typename enable_if<has_height<Type_t>::value, out_stream &>::type print_height(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &size = value.unsafe_get();
		return stream << " * " << make_pretty(size.height) << ")";
	}


	template <typename Type_t>
	typename enable_if<has_Height<Type_t>::value, out_stream &>::type print_height(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &size = value.unsafe_get();
		return stream << " * " << make_pretty(size.Height) << ")";
	}


	template <typename Type_t>
	typename enable_if<has_HEIGHT<Type_t>::value, out_stream &>::type print_height(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &size = value.unsafe_get();
		return stream << " * " << make_pretty(size.HEIGHT) << ")";
	}


	template <typename Type_t>
	typename enable_if<has_width<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &size = value.get();
		stream << "(" << make_pretty(size.width);
		return print_height(stream, value);
	}


	template <typename Type_t>
	typename enable_if<has_Width<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &size = value.get();
		stream << "(" << make_pretty(size.Width);
		return print_height(stream, value);
	}


	template <typename Type_t>
	typename enable_if<has_WIDTH<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &size = value.get();
		stream << "(" << make_pretty(size.WIDTH);
		return print_height(stream, value);
	}


	template <typename Type_t>
	typename enable_if<has_size<Type_t>::value, out_stream &>::type print_size(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &rect = value.unsafe_get();
		return stream << " " << make_pretty(rect.size);
	}


	template <typename Type_t>
	typename enable_if<has_Size<Type_t>::value, out_stream &>::type print_size(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &rect = value.unsafe_get();
		return stream << " " << make_pretty(rect.Size);
	}


	template <typename Type_t>
	typename enable_if<has_SIZE<Type_t>::value, out_stream &>::type print_size(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &rect = value.unsafe_get();
		return stream << " " << make_pretty(rect.SIZE);
	}


	template <typename Type_t>
	typename enable_if<has_origin<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &rect = value.get();
		stream << make_pretty(rect.origin);
		return print_size(stream, value);
	}


	template <typename Type_t>
	typename enable_if<has_Origin<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &rect = value.get();
		stream << make_pretty(rect.Origin);
		return print_size(stream, value);
	}


	template <typename Type_t>
	typename enable_if<has_ORIGIN<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
	{
		stream << FLUSH;
		const Type_t &rect = value.get();
		stream << make_pretty(rect.ORIGIN);
		return print_size(stream, value);
	}


	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::auto_ptr<Type_t> > &value)
	{
		stream << FLUSH;
		const std::auto_ptr<Type_t> &pointer = value.get();
		return stream << make_pretty(pointer.get());
	}


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::shared_ptr<Type_t> > &value)
	{
		stream << FLUSH;
		const std::shared_ptr<Type_t> &pointer = value.get();
		stream << make_pretty(static_cast<const void *>(pointer.get())) << ", use_count = " << to_string(pointer.use_count());
		if (pointer.get() != NULL)
		{
			stream << " -> " << FLUSH;
			stream << make_pretty(*(value.get()));
		}

		return stream;
	}


	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::unique_ptr<Type_t> > &value)
	{
		stream << FLUSH;
		const std::unique_ptr<Type_t> &pointer = value.get();
		return stream << make_pretty(pointer.get());
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


	template <typename First_t, typename Second_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::pair<First_t, Second_t> > &value)
	{
		stream << FLUSH;
		const std::pair<First_t, Second_t> &pair = value.get();
		stream << "{";
		stream << make_pretty(pair.first) << ": ";
		stream << make_pretty(pair.second) << "}";
		return stream;
	}


#if defined(PRETTY_OUTPUT_CPP11)

	template <std::size_t Index, typename ...Types_t>
	typename enable_if<Index == sizeof_pack<Types_t...>::value - 1, out_stream &>::type print_tuple(out_stream &stream, const std::tuple<Types_t...> &tuple)
	{
		return stream << make_pretty(std::get<Index>(tuple)) << "}";
	}


#if defined(PRETTY_OUTPUT_MVS)

	#pragma warning(push)
	#pragma warning(disable:4296) // "expression is always false"

#endif

	template <std::size_t Index, typename ...Types_t>
	typename enable_if<Index < sizeof_pack<Types_t...>::value - 1, out_stream &>::type print_tuple(out_stream &stream, const std::tuple<Types_t...> &tuple)
	{
		stream << make_pretty(std::get<Index>(tuple)) << ", ";
		return print_tuple<Index + 1>(stream, tuple);
	}

#if defined(_MSC_VER)

	#pragma warning(pop)

#endif


	template <typename ...Types_t>
	out_stream &operator <<(out_stream &stream, const pretty<std::tuple<Types_t ...> > &value)
	{
		stream << FLUSH;
		const std::tuple<Types_t ...> &tuple = value.get();
		stream << "{";
		return print_tuple<0>(stream, tuple);
	}


	// not sure if all C++11 standard library versions have std::next
	template <typename Type_t>
	Type_t next_itr(Type_t iterator)
	{
		++iterator;
		return iterator;
	}


	template <template <typename ...> class Container, typename ...Parameters_t>
	out_stream &operator <<(out_stream &stream, const pretty<Container<Parameters_t...> > &value)
	{
		stream << FLUSH;
		const auto &container = value.get();

		stream << "[";
		auto iterator = std::begin(container);
		for ( ; next_itr(iterator) != std::end(container); ++iterator)
		{
			stream << make_pretty(*iterator) << ", ";
		}

		stream << make_pretty(*iterator) << "]";

		return stream;
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const watches<Type_t> &values)
	{
		stream << FLUSH;
		return stream << values.first_name() << " = " << values.first_pretty();
	}


	template <typename ...Types_t>
	out_stream &operator <<(out_stream &stream, const watches<Types_t...> &values)
	{
		stream << FLUSH;
		return stream << values.first_name() << " = " << values.first_pretty() << NEWLINE << values.rest();
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, const pretty_bool<Type_t> &value)
	{
		stream << FLUSH;
		stream << (value.get() ? "true" : "false") << " (" << FLUSH;
		stream << make_pretty(value.get()) << ")";
		return stream;
	}

}
}


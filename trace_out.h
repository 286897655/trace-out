#ifndef TRACE_OUT_H
#define TRACE_OUT_H

//#define TRACE_OUT_REDIRECTION trace_out_to_stdout
#define TRACE_OUT_REDIRECTION trace_out_to_wdo


#if __cplusplus >= 201103L || _MSC_VER >= 1800
#define TRACE_OUT_CPP11
#endif

#if defined(__clang__)
#define TRACE_OUT_CLANG
#define TRACE_OUT_POSIX
#elif defined(__MINGW32__)
#define TRACE_OUT_MINGW
#define TRACE_OUT_WINDOWS
#elif defined(__GNUC__) || defined(__GNUG__)
#define TRACE_OUT_GCC
#define TRACE_OUT_POSIX
#elif defined(_MSC_VER)
#define TRACE_OUT_MVS
#define TRACE_OUT_WINDOWS
#else
#error Failed to detect compiler and platform
#endif

#if defined(__GNUG__) || defined(__clang__)
#define TRACE_OUT_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define TRACE_OUT_FUNCTION_SIGNATURE __FUNCSIG__
#else
#error Cannot find function signature macro for current compiler. Try to add one manualy to this block.
#endif

#if defined(TRACE_OUT_CLANG)
#pragma clang diagnostic pop
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
#pragma clang diagnostic ignored "-Wvariadic-macros"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#elif defined(TRACE_OUT_GCC)
#pragma GCC diagnostic pop
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros" // doesn't work with MinGW and probably also with GCC

#endif


#define TRACE_OUT_PRIVATE__CONCAT_IMPL(a, b) \
					a##b

#define TRACE_OUT_PRIVATE__CONCAT(a, b) \
					TRACE_OUT_PRIVATE__CONCAT_IMPL(a, b)

#define TRACE_OUT_PRIVATE__UNIFY(identifier_base) \
					TRACE_OUT_PRIVATE__CONCAT(identifier_base, __COUNTER__)

#define TRACE_OUT_FILENAME_LINE \
					(trace_out::detail::filename_line_field(trace_out::detail::filename_from_path(__FILE__), __LINE__))


#include <memory>
#include <string>
#include <cstdarg>
#include <sstream>
#include <ctime>

namespace trace_out {
	namespace detail
	{

#if defined(TRACE_OUT_INDENTATION)
		const char INDENTATION[] = TRACE_OUT_INDENTATION;
#else
		const char INDENTATION[] = "    ";
#endif

		const char THREAD_HEADER_SEPARATOR = '~';
		const char FILENAME_FIELD_EXCESS_PADDING[] = "~";
		const size_t FILENAME_FIELD_EXCESS_PADDING_SIZE = sizeof(FILENAME_FIELD_EXCESS_PADDING);
		const size_t FILENAME_FIELD_WIDTH = 20;
		const size_t LINE_FIELD_WIDTH = 4;
		const char DELIMITER[] = " |  ";
		const size_t INDENTATION_WIDTH = sizeof(INDENTATION) - 1;

	}
}

namespace trace_out {
	namespace detail
	{

		const std::string filename_from_path(const char *path);
		const std::string filename_line_field(const std::string &file, unsigned int line);

		template <typename Type_t>
		Type_t &reference(Type_t &object);

		template <typename Type_t>
		const Type_t &reference(const Type_t &object);

		template <typename Type_t>
		Type_t &reference(Type_t *object);

		template <typename Type_t>
		const Type_t &reference(const Type_t *object);

		template <typename Type_t>
		void crash_on_bad_memory(const Type_t &variable);


		typedef std::ios_base &(*manipulator_t)(std::ios_base &);

		void apply_io_manipulators(std::ostream &stream, va_list manipulators);

		template <typename Type_t>
		const std::string to_string(const Type_t &value, manipulator_t first_manipulator = NULL, ...);

		std::string first_token(const std::string &tokens);
		std::string rest_tokens(const std::string &tokens);


		template <typename Type_t>
		struct is_fundamental
		{
			enum
			{
				value = false
			};
		};


#define TRACE_OUT__DEFINE_IS_FUNDAMENTAL(type) \
					template <> \
					struct is_fundamental<type> \
							{ \
						enum \
								{ \
							value = true \
								}; \
							}


		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(char);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(signed char);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(unsigned char);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(signed short int);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(unsigned short int);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(signed int);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(unsigned int);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(signed long int);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(unsigned long int);

#if defined(TRACE_OUT_CPP11)

		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(signed long long);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(unsigned long long);

#endif // defined(TRACE_OUT_CPP11)

		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(float);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(double);
		TRACE_OUT__DEFINE_IS_FUNDAMENTAL(long double);

#undef TRACE_OUT__DEFINE_IS_FUNDAMENTAL


		template <typename Type_t>
		struct is_pointer
		{
			enum
			{
				value = false
			};
		};


		template <typename Type_t>
		struct is_pointer<Type_t *>
		{
			enum
			{
				value = true
			};
		};



		template <typename Type_t>
		struct is_primitive
		{
			enum
			{
				value = is_fundamental<Type_t>::value || is_pointer<Type_t>::value
			};
		};



		template <bool Condition, typename True_t, typename False_t>
		struct conditional
		{
			typedef False_t type;
		};


		template <typename True_t, typename False_t>
		struct conditional<true, True_t, False_t>
		{
			typedef True_t type;
		};



		template <bool Condition, typename Type_t>
		struct enable_if;


		template <typename Type_t>
		struct enable_if<true, Type_t>
		{
			typedef Type_t type;
		};


#if defined(TRACE_OUT_CPP11)

		// need this to fix printing of std::tuple
		template <typename ...Types_t>
		struct sizeof_pack
		{
			enum
			{
				value = sizeof...(Types_t)
			};
		};

#endif // defined(TRACE_OUT_CPP11)

	}
}


namespace trace_out {
	namespace detail
	{

		template <typename Type_t>
		Type_t &reference(Type_t &object)
		{
			return object;
		}


		template <typename Type_t>
		const Type_t &reference(const Type_t &object)
		{
			return object;
		}


		template <typename Type_t>
		Type_t &reference(Type_t *object)
		{
			return *object;
		}


		template <typename Type_t>
		const Type_t &reference(const Type_t *object)
		{
			return *object;
		}


		template <typename Type_t>
		void crash_on_bad_memory(const Type_t &variable)
		{
			uint8_t buffer[sizeof(variable)];
			std::memcpy(buffer, &variable, sizeof(buffer));
		}


		template <typename Type_t>
		const std::string to_string(const Type_t &value, manipulator_t first_manipulator, ...)
		{
			std::stringstream string_stream;

			if (first_manipulator != NULL)
			{
				va_list rest_manipulators;
				va_start(rest_manipulators, first_manipulator);

				string_stream << first_manipulator;
				apply_io_manipulators(string_stream, rest_manipulators);

				va_end(rest_manipulators);
			}

			string_stream << value;

			return string_stream.str();
		}

	}
}


namespace trace_out {
	namespace detail
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



namespace trace_out {
	namespace detail
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
		pretty<typename promote<Type_t>::type> make_pretty(const Type_t &value);


#if defined(TRACE_OUT_CPP11)

		template <typename ...>
		class watches;


		template <typename Type_t, typename ...Types_t>
		class watches<Type_t, Types_t ...>
		{
		public:
			watches(const std::string &names, const Type_t &first, const Types_t &...rest);
			watches(const watches<Type_t, Types_t ...> &another);

			const std::string &first_name() const;
			const pretty<Type_t> &first_pretty() const;
			const watches<Types_t ...> &rest() const;

		private:
			std::string _first_name;
			pretty<Type_t> _first_pretty;
			watches<Types_t ...> _rest;

			watches<Type_t, Types_t ...> &operator =(const watches<Type_t, Types_t ...> &) = delete;
			watches<Type_t, Types_t ...> &operator =(watches<Type_t, Types_t ...> &&) = delete;
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
		watches<Types_t ...> make_watches(const std::string &names, const Types_t &...arguments);

#endif // defined(TRACE_OUT_CPP11)


		template <typename Type_t>
		class pretty_bool
		{
		public:
			pretty_bool(const Type_t &data);
			pretty_bool(const pretty_bool &another);

			const Type_t &get() const;

		private:
			pretty_bool &operator =(const pretty_bool &another); // = delete

#if defined(TRACE_OUT_CPP11)

			pretty_bool &operator =(pretty_bool &&another); // = delete

#endif // defined(TRACE_OUT_CPP11)

			const Type_t &_data;
		};


		template <typename Type_t>
		pretty_bool<Type_t> make_pretty_bool(const Type_t &value);

	}
}


namespace trace_out {
	namespace detail
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
		pretty<typename promote<Type_t>::type> make_pretty(const Type_t &value)
		{
			typedef typename promote<Type_t>::type promoted_t;

			return pretty<promoted_t>(reinterpret_cast<const promoted_t &>(value));
		}


#if defined(TRACE_OUT_CPP11)

		template <typename Type_t, typename ...Types_t>
		watches<Type_t, Types_t ...>::watches(const std::string &names, const Type_t &first, const Types_t &...rest)
			:
			_first_name(first_token(names)),
			_first_pretty(first),
			_rest(rest_tokens(names), rest...)
		{
		}


		template <typename Type_t, typename ...Types_t>
		watches<Type_t, Types_t ...>::watches(const watches<Type_t, Types_t ...> &another)
			:
			_first_name(another._first_name),
			_first_pretty(another._first_pretty),
			_rest(another._rest)
		{
		}


		template <typename Type_t, typename ...Types_t>
		const std::string &watches<Type_t, Types_t ...>::first_name() const
		{
			return _first_name;
		}


		template <typename Type_t, typename ...Types_t>
		const pretty<Type_t> &watches<Type_t, Types_t ...>::first_pretty() const
		{
			return _first_pretty;
		}


		template <typename Type_t, typename ...Types_t>
		const watches<Types_t ...> &watches<Type_t, Types_t ...>::rest() const
		{
			return _rest;
		}



		template <typename Type_t>
		watches<Type_t>::watches(const std::string &name, const Type_t &first)
			:
			_first_name(name),
			_first_pretty(first)
		{
		}


		template <typename Type_t>
		watches<Type_t>::watches(const watches<Type_t> &another)
			:
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
		watches<Types_t ...> make_watches(const std::string &names, const Types_t &...arguments)
		{
			return watches<Types_t ...>(names, arguments...);
		}

#endif // defined(TRACE_OUT_CPP11)


		template <typename Type_t>
		pretty_bool<Type_t>::pretty_bool(const Type_t &data)
			:
			_data(data)
		{
		}


		template <typename Type_t>
		pretty_bool<Type_t>::pretty_bool(const pretty_bool &another)
			:
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



namespace trace_out {
	namespace detail
	{

#define TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(name) \
				template <typename Struct_t> \
				struct has_data_member_##name \
					{ \
					struct fallback \
						{ \
						int name; \
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
					static char (&function(check<int fallback::*, &Type_t::name> *))[1]; \
					\
					template <typename Type_t> \
					static char (&function(...))[2]; \
					\
					enum \
						{ \
						value = sizeof(function<derived>(0)) == 2 \
						}; \
					}


		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(x);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(y);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(z);

		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(width);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(height);

		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(origin);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(size);

		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(X);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(Y);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(Z);

		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(WIDTH);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(HEIGHT);

		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(ORIGIN);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(SIZE);

		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(Width);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(Height);

		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(Origin);
		TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER(Size);

#undef TRACE_OUT_PRIVATE__DEFINE_HAS_DATA_MEMBER


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

#if defined(TRACE_OUT_CPP11)

		out_stream &operator <<(out_stream &stream, const pretty<long long> &value);
		out_stream &operator <<(out_stream &stream, const pretty<unsigned long long> &value);

#endif // defined(TRACE_OUT_CPP11)

		out_stream &operator <<(out_stream &stream, const pretty<float> &value);
		out_stream &operator <<(out_stream &stream, const pretty<double> &value);
		out_stream &operator <<(out_stream &stream, const pretty<long double> &value);
		out_stream &operator <<(out_stream &stream, const pretty<const void *> &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<const Type_t *> &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<Type_t *> &value);

		template <typename Type_t>
		typename enable_if<has_data_member_x<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value);

		template <typename Type_t>
		typename enable_if<has_data_member_width<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value);

		template <typename Type_t>
		typename enable_if<has_data_member_origin<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::auto_ptr<Type_t> > &value);

#if defined(TRACE_OUT_CPP11)

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::unique_ptr<Type_t> > &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::shared_ptr<Type_t> > &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::weak_ptr<Type_t> > &value);

#endif // defined(TRACE_OUT_CPP11)

		template <typename First_t, typename Second_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::pair<First_t, Second_t> > &value);

#if defined(TRACE_OUT_CPP11)

		template <typename ...Types_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::tuple<Types_t ...> > &value);

		template <template <typename ...> class Container, typename ...Parameters_t>
		out_stream &operator <<(out_stream &stream, const pretty<Container<Parameters_t ...> > &value);

#endif // defined(TRACE_OUT_CPP11)


#if defined(TRACE_OUT_CPP11)

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const watches<Type_t> &values);

		template <typename ...Types_t>
		out_stream &operator <<(out_stream &stream, const watches<Types_t ...> &values);

#endif // defined(TRACE_OUT_CPP11)

		out_stream &operator <<(out_stream &stream, const pretty_bool<bool> &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty_bool<Type_t> &value);

	}
}


namespace trace_out {
	namespace detail
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
		typename enable_if<has_data_member_z<Type_t>::value, out_stream &>::type print_z(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &point = value.unsafe_get();
			return stream << ", " << make_pretty(point.z) << ")";
		}


		template <typename Type_t>
		typename enable_if<has_data_member_Z<Type_t>::value, out_stream &>::type print_z(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &point = value.unsafe_get();
			return stream << ", " << make_pretty(point.Z) << ")";
		}


		template <typename Type_t>
		typename enable_if<!(has_data_member_z<Type_t>::value || has_data_member_Z<Type_t>::value), out_stream &>::type print_z(out_stream &stream, const pretty<Type_t> &)
		{
			return stream << ")";
		}


		template <typename Type_t>
		typename enable_if<has_data_member_y<Type_t>::value, out_stream &>::type print_y(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &point = value.unsafe_get();
			stream << ", " << make_pretty(point.y);
			return print_z(stream, value);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_Y<Type_t>::value, out_stream &>::type print_y(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &point = value.unsafe_get();
			stream << ", " << make_pretty(point.Y);
			return print_z(stream, value);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_x<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &point = value.get();
			stream << "(" << make_pretty(point.x);
			return print_y(stream, value);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_X<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &point = value.get();
			stream << "(" << make_pretty(point.X);
			return print_y(stream, value);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_height<Type_t>::value, out_stream &>::type print_height(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &size = value.unsafe_get();
			return stream << " * " << make_pretty(size.height) << ")";
		}


		template <typename Type_t>
		typename enable_if<has_data_member_Height<Type_t>::value, out_stream &>::type print_height(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &size = value.unsafe_get();
			return stream << " * " << make_pretty(size.Height) << ")";
		}


		template <typename Type_t>
		typename enable_if<has_data_member_HEIGHT<Type_t>::value, out_stream &>::type print_height(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &size = value.unsafe_get();
			return stream << " * " << make_pretty(size.HEIGHT) << ")";
		}


		template <typename Type_t>
		typename enable_if<has_data_member_width<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &size = value.get();
			stream << "(" << make_pretty(size.width);
			return print_height(stream, value);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_Width<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &size = value.get();
			stream << "(" << make_pretty(size.Width);
			return print_height(stream, value);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_WIDTH<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &size = value.get();
			stream << "(" << make_pretty(size.WIDTH);
			return print_height(stream, value);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_size<Type_t>::value, out_stream &>::type print_size(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &rect = value.unsafe_get();
			return stream << " " << make_pretty(rect.size);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_Size<Type_t>::value, out_stream &>::type print_size(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &rect = value.unsafe_get();
			return stream << " " << make_pretty(rect.Size);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_SIZE<Type_t>::value, out_stream &>::type print_size(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &rect = value.unsafe_get();
			return stream << " " << make_pretty(rect.SIZE);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_origin<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &rect = value.get();
			stream << make_pretty(rect.origin);
			return print_size(stream, value);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_Origin<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
		{
			stream << FLUSH;
			const Type_t &rect = value.get();
			stream << make_pretty(rect.Origin);
			return print_size(stream, value);
		}


		template <typename Type_t>
		typename enable_if<has_data_member_ORIGIN<Type_t>::value, out_stream &>::type operator <<(out_stream &stream, const pretty<Type_t> &value)
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


#if defined(TRACE_OUT_CPP11)

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::unique_ptr<Type_t> > &value)
		{
			stream << FLUSH;
			const std::unique_ptr<Type_t> &pointer = value.get();
			return stream << make_pretty(pointer.get());
		}


		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::shared_ptr<Type_t> > &value)
		{
			stream << FLUSH;
			const std::shared_ptr<Type_t> &pointer = value.get();
			stream << make_pretty(static_cast<const void *>(pointer.get())) << " (use_count: " << to_string(pointer.use_count()) << ")";
			if (pointer.get() != NULL)
			{
				stream << " -> " << FLUSH;
				stream << make_pretty(*(value.get()));
			}

			return stream;
		}


		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::weak_ptr<Type_t> > &value)
		{
			stream << FLUSH;
			const std::weak_ptr<Type_t> &pointer = value.get();
			stream << "(use_count: " << to_string(pointer.use_count()) << ")";

			return stream;
		}

#endif // defined(TRACE_OUT_CPP11)


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


#if defined(TRACE_OUT_CPP11)

		template <std::size_t Index, typename ...Types_t>
		typename enable_if<Index == sizeof_pack<Types_t...>::value - 1, out_stream &>::type print_tuple(out_stream &stream, const std::tuple<Types_t...> &tuple)
		{
			return stream << make_pretty(std::get<Index>(tuple)) << "}";
		}


#if defined(TRACE_OUT_MVS)

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
			for (; next_itr(iterator) != std::end(container); ++iterator)
			{
				stream << make_pretty(*iterator) << ", ";
			}

			stream << make_pretty(*iterator) << "]";

			return stream;
		}

#endif // defined(TRACE_OUT_CPP11)


#if defined(TRACE_OUT_CPP11)

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

#endif // defined(TRACE_OUT_CPP11)


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



namespace trace_out {
	namespace detail
	{

		class function_printer
		{
		public:
			function_printer(const std::string &filename_line, const char *function_signature);
			~function_printer();

		private:
			std::string _filename_line;
			std::string _function_signature;
		};


		function_printer make_function_printer(const std::string &filename_line, const char *function_signature);



		class return_printer
		{
		public:
			return_printer(const std::string &filename_line);

			template <typename T>
			const T &operator ,(const T &value);

		private:
			std::string _filename_line;
		};


		return_printer make_return_printer(const std::string &filename_line);

	}
}


namespace trace_out {
	namespace detail
	{

		template <typename Type_t>
		const Type_t &return_printer::operator ,(const Type_t &value)
		{
			out_stream stream(_filename_line);
			stream << "return " << make_pretty(value) << ENDLINE;
			return value;
		}

	}
}

namespace trace_out {
	namespace detail
	{

		typedef uint32_t option_t;

		const size_t OPTIONS_START_BASE = 0;
		const size_t OPTIONS_START_BYTE_ORDER = 16;

	}
}


namespace trace_out
{

	const detail::option_t BIN = static_cast<detail::option_t>(0x1) << (detail::OPTIONS_START_BASE + 0);
	const detail::option_t SDEC = static_cast<detail::option_t>(0x1) << (detail::OPTIONS_START_BASE + 1);
	const detail::option_t UDEC = static_cast<detail::option_t>(0x1) << (detail::OPTIONS_START_BASE + 2);
	const detail::option_t HEX = static_cast<detail::option_t>(0x1) << (detail::OPTIONS_START_BASE + 3);
	const detail::option_t FLT = static_cast<detail::option_t>(0x1) << (detail::OPTIONS_START_BASE + 4);
	const detail::option_t DBL = static_cast<detail::option_t>(0x1) << (detail::OPTIONS_START_BASE + 5);
	const detail::option_t LDBL = static_cast<detail::option_t>(0x1) << (detail::OPTIONS_START_BASE + 6);

	const detail::option_t LITTLE = static_cast<detail::option_t>(0x1) << (detail::OPTIONS_START_BYTE_ORDER + 0);
	const detail::option_t BIG = static_cast<detail::option_t>(0x1) << (detail::OPTIONS_START_BYTE_ORDER + 1);

}


namespace trace_out {
	namespace detail
	{

		extern const char *const BASE_NAMES[];
		extern const size_t BASE_NAMES_LENGTH;

		extern const char *const BYTE_ORDER_NAMES[];
		extern const size_t BYTE_ORDER_NAMES_LENGTH;

		typedef std::streamsize outputwidth_t;


		enum typefamily_t
		{
			TYPE_FAMILY_INTEGER,
			TYPE_FAMILY_FLOATING_POINT,
			TYPE_FAMILY_OTHER
		};


		template <typename Type_t>
		struct type_family
		{
			enum
			{
				value = std::numeric_limits<Type_t>::is_integer ? TYPE_FAMILY_INTEGER : (std::numeric_limits<Type_t>::is_specialized ? TYPE_FAMILY_FLOATING_POINT : TYPE_FAMILY_OTHER)
			};
		};


		template <typefamily_t Family, size_t Size, bool IsSigned>
		struct print_traits_details
		{
			typedef uint8_t unit_t;
			static const outputwidth_t field_width = 2;
			static const option_t default_base = HEX;
			typedef void signed_t;
			typedef void unsigned_t;
		};

#define TRACE_OUT__DEFINE_PRINT_TRAITS(family, type_size, is_signed, unit_type, field_width_value, default_base_value, to_signed_type, to_unsigned_type) \
		template <> \
		struct print_traits_details<family, type_size, is_signed> \
				{ \
			typedef unit_type unit_t; \
			static const outputwidth_t field_width = field_width_value; \
			static const option_t default_base = default_base_value; \
			typedef to_signed_type signed_t; \
			typedef to_unsigned_type unsigned_t; \
				}

		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, true, int8_t, 4, HEX, int8_t, uint8_t);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, true, int16_t, 6, SDEC, int16_t, uint16_t);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, true, int32_t, 11, SDEC, int32_t, uint32_t);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, true, int64_t, 21, SDEC, int64_t, uint64_t);

		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, false, uint8_t, 3, HEX, int8_t, uint8_t);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, false, uint16_t, 5, UDEC, int16_t, uint16_t);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, false, uint32_t, 10, UDEC, int32_t, uint32_t);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, false, uint64_t, 20, UDEC, int64_t, uint64_t);

		// sign + first_digit + point + precision + 'e' + exponent_sign + exponent
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 4, true, float, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 3, FLT, float, float);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 8, true, double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 4, DBL, double, double);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 10, true, long double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 12, true, long double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);
		TRACE_OUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 16, true, long double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);

#undef TRACE_OUT__DEFINE_PRINT_TRAITS


		template <typename Type_t>
		struct print_traits :
			public print_traits_details<static_cast<typefamily_t>(type_family<Type_t>::value), sizeof(Type_t), std::numeric_limits<Type_t>::is_signed>
		{
		};


		option_t base_value_from_options(option_t options, option_t default_value);
		option_t byte_order_value_from_options(option_t options, option_t default_value);
		const char *option_name(option_t option, const char *const names[], size_t names_length, const char *default_name);
		const char *byte_to_binary(uint8_t byte);
		const char *byte_to_hexadecimal(uint8_t byte);

		template <typename Type_t>
		outputwidth_t field_width(option_t base);

		template <typename Type_t>
		const std::string bytes_to_binary_string(Type_t value);

		template <typename Type_t>
		const std::string bytes_to_signed_decimal_string(Type_t value);

		template <typename Type_t>
		const std::string bytes_to_unsigned_decimal_string(Type_t value);

		template <typename Type_t>
		const std::string bytes_to_floating_point_string(Type_t value);

		template <typename Type_t>
		const std::string bytes_to_hexadecimal_string(Type_t value);

		template <typename Type_t>
		const std::string(*select_conversion(option_t base))(Type_t);

		option_t current_byte_order();
		void reverse_bytes(void *destination, const void *source, size_t size);
		void order_bytes(void *ordered_bytes, const void *unordered_bytes, size_t size, option_t byte_order);

		template <typename Type_t>
		void print_memory(const std::string &filename_line, const char *name, const Type_t *pointer, size_t size = sizeof(Type_t), option_t options = 0);

	}
}


namespace trace_out {
	namespace detail
	{

		template <typename Type_t>
		outputwidth_t field_width(option_t base)
		{
			switch (base)
			{
			case BIN:
				return sizeof(typename print_traits<Type_t>::unit_t) * 8;

			case SDEC:
				return print_traits<Type_t>::field_width + (!std::numeric_limits<Type_t>::is_signed ? 1 : 0);

			case UDEC:
				return print_traits<Type_t>::field_width - (std::numeric_limits<Type_t>::is_signed ? 1 : 0);

			case FLT:
			case DBL:
			case LDBL:
				return print_traits<Type_t>::field_width;

			case HEX:
			default:
				return sizeof(typename print_traits<Type_t>::unit_t) * 2;
			}
		}


		template <typename Type_t>
		const std::string bytes_to_binary_string(Type_t value)
		{
			std::stringstream stream;
			uint8_t *data = reinterpret_cast<uint8_t *>(&value);
			for (size_t index = 0; index < sizeof(value); ++index)
			{
				stream << byte_to_binary(data[index]);
			}

			return stream.str();
		}


		template <typename Type_t>
		const std::string bytes_to_signed_decimal_string(Type_t value)
		{
			typedef typename print_traits<Type_t>::signed_t signed_promotion_t;

			signed_promotion_t signed_value = static_cast<signed_promotion_t>(value);
			int64_t signed_integer = static_cast<int64_t>(signed_value);

			return to_string(signed_integer);
		}


		template <typename Type_t>
		const std::string bytes_to_unsigned_decimal_string(Type_t value)
		{
			typedef typename print_traits<Type_t>::unsigned_t unsigned_promotion_t;

			unsigned_promotion_t unsigned_value = static_cast<unsigned_promotion_t>(value);
			uint64_t unsigned_integer = static_cast<uint64_t>(unsigned_value);

			return to_string(unsigned_integer);
		}


		template <typename Type_t>
		const std::string bytes_to_floating_point_string(Type_t value)
		{
			std::stringstream stream;
			stream.precision(std::numeric_limits<Type_t>::digits10);
			stream << std::scientific << value;

			return stream.str();
		}


		template <typename Type_t>
		const std::string bytes_to_hexadecimal_string(Type_t value)
		{
			std::stringstream stream;
			uint8_t *data = reinterpret_cast<uint8_t *>(&value);
			for (size_t index = 0; index < sizeof(value); ++index)
			{
				stream << byte_to_hexadecimal(data[index]);
			}

			return stream.str();
		}


		template <typename Type_t>
		const std::string(*select_conversion(option_t base))(Type_t)
		{
			switch (base)
			{
			case BIN:
				return bytes_to_binary_string<Type_t>;

			case SDEC:
				return bytes_to_signed_decimal_string<Type_t>;

			case UDEC:
				return bytes_to_unsigned_decimal_string<Type_t>;

			case FLT:
			case DBL:
			case LDBL:
				return bytes_to_floating_point_string<Type_t>;

			case HEX:
			default:
				return bytes_to_hexadecimal_string<Type_t>;
			}
		}


		template <typename Type_t>
		void print_memory_contents(out_stream &stream, const Type_t *pointer, size_t size, outputwidth_t column_width, const std::string(*bytes_to_string)(Type_t), option_t byte_order)
		{
			std::stringstream string_stream;

			const Type_t *iterator = pointer;
			size_t length = size / sizeof(Type_t);

			stream << make_pretty(static_cast<const void *>(iterator)) << ":";
			for (std::size_t index = 0; index < length; ++index)
			{
				const std::string string_representation = string_stream.str();
				if (string_representation.length() + static_cast<size_t>(column_width)+1 > stream.width_left())
				{
					stream << string_representation;
					string_stream.str("");

					stream << NEWLINE << make_pretty(static_cast<const void *>(&iterator[index])) << ":";
				}

				const Type_t &bytes = iterator[index];

				crash_on_bad_memory(bytes);

				Type_t ordered_bytes;
				order_bytes(&ordered_bytes, &bytes, sizeof(Type_t), byte_order);

				string_stream << " ";
				string_stream.fill(' ');
				string_stream.width(column_width);
				string_stream.flags(std::ios::right);

				string_stream << bytes_to_string(ordered_bytes);
			}

			const std::string string_representation = string_stream.str();
			if (!string_representation.empty())
			{
				stream << string_representation << NEWLINE;
			}
		}


		template <typename Type_t>
		void print_memory(const std::string &filename_line, const char *name, const Type_t *pointer, size_t size, option_t options)
		{
			typedef typename print_traits<Type_t>::unit_t unit_t;

			option_t base = base_value_from_options(options, print_traits<Type_t>::default_base);
			option_t byte_order = byte_order_value_from_options(options, current_byte_order());

			const char *base_name = option_name((base >> OPTIONS_START_BASE), BASE_NAMES, BASE_NAMES_LENGTH, "?");
			const char *byte_order_name = option_name((byte_order >> OPTIONS_START_BYTE_ORDER), BYTE_ORDER_NAMES, BYTE_ORDER_NAMES_LENGTH, "?");

			out_stream stream(filename_line);
			stream << name << " (" << base_name << ", " << byte_order_name << "):";
			indentation_add();
			stream << NEWLINE;

			const std::string(*bytes_to_string)(Type_t) = select_conversion<Type_t>(base);
			outputwidth_t column_width = field_width<Type_t>(base);
			print_memory_contents(stream, static_cast<const unit_t *>(pointer), size, column_width, bytes_to_string, byte_order);

			indentation_remove();

			stream << ENDLINE;
		}

	}
}



namespace trace_out {
	namespace detail
	{

		extern const char FILE_PATH_COMPONENT_DELIMITER;

	}
}




namespace trace_out {
	namespace detail
	{

		template <typename Type_t>
		class resource
		{
		public:
			typedef void(*deleter_t)(Type_t);

			resource(Type_t handle, deleter_t deleter);
			~resource();

			const Type_t &get() const;

		private:
			resource();
			resource(const resource &);
			resource &operator =(const resource &);

#if defined(TRACE_OUT_CPP11)

			resource(resource &&another);
			resource &operator =(resource &&another);

#endif // defined(TRACE_OUT_CPP11)

			Type_t _handle;
			deleter_t _deleter;
		};

	}
}


namespace trace_out {
	namespace detail
	{

		template <typename Type_t>
		resource<Type_t>::resource(Type_t handle, deleter_t deleter)
			:
			_handle(handle),
			_deleter(deleter)
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

namespace trace_out {
	namespace detail
	{

		class auto_indentation
		{
		public:
			auto_indentation();
			~auto_indentation();
		};


		class block
		{
		public:
			block(bool value);
			block(const block &another);
			~block();

			operator bool() const;

		private:
			block &operator =(const block &another); // = delete

#if defined(TRACE_OUT_CPP11)

			block &operator =(block &&another); // = delete

#endif // defined(TRACE_OUT_CPP11)

			auto_indentation _auto_indentation;
			bool _value;
		};


		template <typename Type_t>
		block if_block(const std::string &filename_line, const char *condition, const Type_t &value);

		template <typename Type_t>
		block while_block(const std::string &filename_line, const char *condition, const Type_t &value);

		block iteration_block(const std::string &filename_line, size_t iteration);



		class for_block
		{
		public:
			for_block(const std::string &filename_line, const char *expression);
			~for_block();

			operator bool() const;
			size_t iteration();

		private:
			size_t _iteration_number;
		};


		for_block make_for_block(const std::string &filename_line, const char *expression);



		void print_while_header(const std::string &filename_line, const char *condition);

	}
}


namespace trace_out {
	namespace detail
	{

		template <typename Type_t>
		block if_block(const std::string &filename_line, const char *condition, const Type_t &value)
		{
			out_stream stream(filename_line);
			stream << "if (" << condition << ") => " << FLUSH;
			stream << make_pretty_bool(value) << ENDLINE;

			return block(!!value);
		}


		template <typename Type_t>
		block while_block(const std::string &filename_line, const char *condition, const Type_t &value)
		{
			{
				auto_indentation auto_indentation;

				out_stream stream(filename_line);
				stream << "// while: " << condition << " => " << FLUSH;
				stream << make_pretty_bool(value) << ENDLINE;
			}

			return block(!!value);
		}

	}
}

namespace trace_out {
	namespace detail
	{

		size_t printf_string_length(const char *format, va_list arguments);
		size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments);
		int console_width();
	}
}

namespace trace_out {
	namespace detail
	{

		typedef struct _mutex *mutex_t;

		mutex_t mutex_new();
		void mutex_delete(mutex_t mutex);
		void mutex_lock(mutex_t mutex);
		void mutex_unlock(mutex_t mutex);


		class mutex
		{
		public:
			mutex();

			void lock();
			void unlock();

		private:
			mutex(const mutex &another);
			mutex &operator =(const mutex &another);

#if defined(TRACE_OUT_CPP11)

			mutex(mutex &&another);
			mutex &operator =(mutex &&another);

#endif // defined(TRACE_OUT_CPP11)

			resource<mutex_t> _handle;
		};

	}
}

namespace trace_out {
	namespace detail
	{

		uint64_t current_thread_id();

	}
}




namespace trace_out {
	namespace detail
	{

		typedef struct _tlskey *tlskey_t;

		tlskey_t tls_new_key();
		void tls_delete_key(tlskey_t key);
		void *tls_get(tlskey_t key);
		void tls_set(tlskey_t key, void *data);


		template <typename Type_t>
		class tls
		{
		public:
			tls();
			~tls();

			void set(const Type_t &value);
			const Type_t &get() const;

		private:
			tls(const tls &another);
			tls &operator =(const tls &another);

#if defined(TRACE_OUT_CPP11)

			tls(tls &&another);
			tls &operator =(tls &&another);

#endif // defined(TRACE_OUT_CPP11)

			resource<tlskey_t> _key;
		};

	}
}


namespace trace_out {
	namespace detail
	{

		template <typename Type_t>
		tls<Type_t>::tls()
			:
			_key(tls_new_key(), tls_delete_key)
		{
		}


		template <typename Type_t>
		tls<Type_t>::~tls()
		{
			Type_t *value = static_cast<Type_t *>(tls_get(_key.get()));
			delete value;
		}


		template <typename Type_t>
		void tls<Type_t>::set(const Type_t &value)
		{
			Type_t *new_value = new Type_t(value);

			Type_t *old_value = static_cast<Type_t *>(tls_get(_key.get()));
			delete old_value;

			tls_set(_key.get(), new_value);
		}


		template <typename Type_t>
		const Type_t &tls<Type_t>::get() const
		{
			Type_t *value = static_cast<Type_t *>(tls_get(_key.get()));
			if (value == NULL)
			{
				value = new Type_t;
				tls_set(_key.get(), value);
			}

			return *value;
		}

	}
}

namespace trace_out {
	namespace detail
	{

		uint64_t time_in_milliseconds();

	}
}




namespace trace_out {
	namespace detail
	{

		void print_execution_time_in_milliseconds(const std::string &filename_line, uint64_t milliseconds);
		void print_execution_time_in_ticks(const std::string &filename_line,const std::string& usermessage, std::clock_t ticks, double milliseconds);

	}
}

namespace trace_out {
	namespace detail
	{

#if !defined(TRACE_OUT_CPP11)

		template <typename Type_t>
		const Type_t &expression(const std::string &filename_line, const char *name, const Type_t &value);

		template <typename Type_t>
		Type_t &expression(const std::string &filename_line, const char *name, Type_t &value);

#else

		template <typename Type_t>
		Type_t &&expression(const std::string &filename_line, const char *name, Type_t &&value);

		template <typename ...Types_t>
		void watch(const std::string &filename_line, const char *names, const Types_t &...arguments);

#endif // !defined(TRACE_OUT_CPP11)

	}
}


namespace trace_out {
	namespace detail
	{

#if !defined(TRACE_OUT_CPP11)

		template <typename Type_t>
		const Type_t &expression(const std::string &filename_line, const char *name, const Type_t &value)
		{
			out_stream stream(filename_line);
			stream << name << " = " << make_pretty(value) << ENDLINE;
			return value;
		}


		template <typename Type_t>
		Type_t &expression(const std::string &filename_line, const char *name, Type_t &value)
		{
			out_stream stream(filename_line);
			stream << name << " = " << make_pretty(value) << ENDLINE;
			return value;
		}

#else

		template <typename Type_t>
		Type_t &&expression(const std::string &filename_line, const char *name, Type_t &&value)
		{
			out_stream stream(filename_line);
			stream << name << " = " << make_pretty(value) << ENDLINE;
			return std::forward<Type_t>(value);
		}


		template <typename ...Types_t>
		void watch(const std::string &filename_line, const char *names, const Types_t &...arguments)
		{
			out_stream stream(filename_line);
			stream << make_watches(names, arguments...) << ENDLINE;
		}

#endif // !defined(TRACE_OUT_CPP11)

	}
}

#if (!defined(NDEBUG) && !defined(TRACE_OUT_OFF)) || defined(TRACE_OUT_ON)

#define $e(...) \
				trace_out::detail::expression(TRACE_OUT_FILENAME_LINE, #__VA_ARGS__, ##__VA_ARGS__)

#define $w(...) \
				trace_out::detail::watch(TRACE_OUT_FILENAME_LINE, #__VA_ARGS__, ##__VA_ARGS__);

#define $m(pointer, ...) \
				trace_out::detail::print_memory(TRACE_OUT_FILENAME_LINE, #pointer, pointer, ##__VA_ARGS__);

#define $f \
				trace_out::detail::function_printer TRACE_OUT_PRIVATE__UNIFY(trace_out_f) = trace_out::detail::make_function_printer(TRACE_OUT_FILENAME_LINE, TRACE_OUT_FUNCTION_SIGNATURE);

#define $return \
				return trace_out::detail::make_return_printer(TRACE_OUT_FILENAME_LINE) ,

#define $trace_if(...) \
				if (trace_out::detail::block TRACE_OUT_PRIVATE__UNIFY(trace_out_if_block) = trace_out::detail::if_block(TRACE_OUT_FILENAME_LINE, #__VA_ARGS__, (__VA_ARGS__)))

#define trace_out_private__for(block_variable_name, ...) \
				if (trace_out::detail::for_block block_variable_name = trace_out::detail::make_for_block(TRACE_OUT_FILENAME_LINE, #__VA_ARGS__)) {} else \
					for (__VA_ARGS__) \
						if (trace_out::detail::block TRACE_OUT_PRIVATE__UNIFY(trace_out_iteration_block) = trace_out::detail::iteration_block(TRACE_OUT_FILENAME_LINE, block_variable_name.iteration())) {} else

#define $for(...) \
				trace_out_private__for(TRACE_OUT_PRIVATE__UNIFY(trace_out_for_block), ##__VA_ARGS__)

#define $while(...) \
				if (trace_out::detail::print_while_header(TRACE_OUT_FILENAME_LINE, #__VA_ARGS__), false) {} else \
									while (trace_out::detail::block TRACE_OUT_PRIVATE__UNIFY(trace_out_while_block) = trace_out::detail::while_block(TRACE_OUT_FILENAME_LINE, #__VA_ARGS__, (__VA_ARGS__)))

#define $trace_p(message) \
													{ \
					trace_out::detail::out_stream stream(TRACE_OUT_FILENAME_LINE); \
					stream<<message<<trace_out::detail::ENDLINE; \
													}

#define $thread(name) \
				trace_out::detail::set_current_thread_name(#name);

#define trace_out_private__time(start_time_variable, end_time_variable, ...) \
				{ \
					uint64_t start_time_variable = trace_out::detail::time_in_milliseconds(); \
					__VA_ARGS__ \
					uint64_t end_time_variable = trace_out::detail::time_in_milliseconds(); \
					trace_out::detail::print_execution_time_in_milliseconds(TRACE_OUT_FILENAME_LINE, end_time_variable - start_time_variable); \
				}

#define $time(...) \
				trace_out_private__time(TRACE_OUT_PRIVATE__UNIFY(trace_out_start_ticks), TRACE_OUT_PRIVATE__UNIFY(trace_out_end_ticks), ##__VA_ARGS__)

#define trace_out_private__ticks(start_ticks_variable, end_ticks_variable,usermessage, ...) \
				{ \
					std::clock_t start_ticks_variable = std::clock(); \
					__VA_ARGS__ \
					std::clock_t end_ticks_variable = std::clock(); \
					std::clock_t execution_time = end_ticks_variable - start_ticks_variable; \
					trace_out::detail::print_execution_time_in_ticks(TRACE_OUT_FILENAME_LINE,std::string(usermessage) ,execution_time, (static_cast<double>(execution_time)) / CLOCKS_PER_SEC * 1000); \
				}

#define $trace_ticks(usermessage,...) \
				trace_out_private__ticks(TRACE_OUT_PRIVATE__UNIFY(trace_out_start_time), TRACE_OUT_PRIVATE__UNIFY(trace_out_end_time),usermessage, ##__VA_ARGS__)

#elif defined(NDEBUG) || defined(TRACE_OUT_OFF)

#define $e(...) \
				__VA_ARGS__

#define $w(...)

#define $m(pointer, ...)

#define $f

#define $return \
				return

#define $trace_if(...) \
				if (__VA_ARGS__)

#define $for(...) \
				for (__VA_ARGS__)

#define $while(...) \
					while (__VA_ARGS__)

#define $trace_p(message)

#define $thread(name)

#define $time(...) \
				__VA_ARGS__

#define $trace_ticks(usermessage,...) \
				__VA_ARGS__

#endif

#endif
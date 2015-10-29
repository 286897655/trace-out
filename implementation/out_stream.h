#pragma once

#include <memory>
#include <string>
#include <cstring>

#if defined(PRETTY_OUTPUT_CPP11)
	#include <tuple>
#endif


#include "defines.h"
#include "pretty.h"


namespace pretty_output
{

	namespace impl
	{

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
		out_stream &operator <<(out_stream &stream, const pretty<std::tuple<Types_t...> > &value);

		template <template <typename ...> class Container, typename ...Parameters_t>
		out_stream &operator <<(out_stream &stream, const pretty<Container<Parameters_t...> > &value);

#endif // defined(PRETTY_OUTPUT_CPP11)


#if defined(PRETTY_OUTPUT_CPP11)

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretties<Type_t> &values);

		template <typename ...Types_t>
		out_stream &operator <<(out_stream &stream, const pretties<Types_t...> &values);

		out_stream &operator <<(out_stream &stream, const pretties<> &values);

#endif // defined(PRETTY_OUTPUT_CPP11)

		out_stream &operator <<(out_stream &stream, const pretty_bool<bool> &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty_bool<Type_t> &value);

	}

}


namespace pretty_output
{

	namespace impl
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
		typename std::enable_if<Index == sizeof...(Types_t) - 1, out_stream &>::type print_tuple(out_stream &stream, const std::tuple<Types_t...> &tuple)
		{
			return stream << make_pretty(std::get<Index>(tuple)) << ")";
		}


		template <std::size_t Index, typename ...Types_t>
		typename std::enable_if<Index < sizeof...(Types_t) - 1, out_stream &>::type print_tuple(out_stream &stream, const std::tuple<Types_t...> &tuple)
		{
			stream << make_pretty(std::get<Index>(tuple)) << ", ";
			return print_tuple<Index + 1>(stream, tuple);
		}


		template <typename ...Types_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::tuple<Types_t...> > &value)
		{
			stream << FLUSH;
			const std::tuple<Types_t...> &tuple = value.get();
			stream << "(";
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
		out_stream &operator <<(out_stream &stream, const pretties<Type_t> &values)
		{
			stream << FLUSH;
			return stream << make_pretty(values.first().get());
		}


		template <typename ...Types_t>
		out_stream &operator <<(out_stream &stream, const pretties<Types_t...> &values)
		{
			stream << FLUSH;
			return stream << make_pretty(values.first().get()) << values.delimiter() << values.rest();
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


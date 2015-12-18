#include <cstdint>
#include <cstdarg>
#include <cstdlib>
#include <sstream>

#include "constants.h"
#include "system_thread_local_storage.h"
#include "system_mutex.h"
#include "system_thread.h"
#include "stdlib_specific.h"

#include "out_stream.h"


#if defined(PRETTY_OUTPUT_REDIRECTION)
	#define PRETTY_OUTPUT_REDIRECTION_NAMESPACE PRETTY_OUTPUT_REDIRECTION
#else
	#define PRETTY_OUTPUT_REDIRECTION_NAMESPACE pretty_output_to_stdout
#endif


namespace PRETTY_OUTPUT_REDIRECTION_NAMESPACE
{

	void print(const char *string);
	void flush();
	size_t width();

}


namespace pretty_output { namespace detail
{

	const class newline_manipulator {} NEWLINE = newline_manipulator();
	const class endline_manipulator {} ENDLINE = endline_manipulator();
	const class flush_manipulator {} FLUSH = flush_manipulator();


	const std::string current_thread_name();
	bool is_running_same_thread();

	void lock_output();
	void unlock_output();

	const std::string thread_id_field(uint64_t thread_id);
	const std::string thread_header(const std::string &thread_id, const std::string &thread_name);


	uint64_t _current_thread_id;
	tls<std::string> _thread_name;
	mutex _output_mutex;
	tls<std::string> _indentation;

}
}


namespace pretty_output { namespace detail
{

	const std::string current_thread_name()
	{
		return _thread_name.get();
	}


	bool is_running_same_thread()
	{
		if (_current_thread_id != current_thread_id())
		{
			_current_thread_id = current_thread_id();
			return false;
		}

		return true;
	}


	void lock_output()
	{
#if !defined(PRETTY_OUTPUT_NO_OUTPUT_SYNC)

		_output_mutex.lock();

#endif // !defined(PRETTY_OUTPUT_NO_OUTPUT_SYNC)
	}


	void unlock_output()
	{
#if !defined(PRETTY_OUTPUT_NO_OUTPUT_SYNC)

		_output_mutex.unlock();

#endif // !defined(PRETTY_OUTPUT_NO_OUTPUT_SYNC)
	}


	const std::string thread_id_field(uint64_t thread_id)
	{
		std::stringstream stream;
		stream << reinterpret_cast<void *>(thread_id);

		return stream.str();
	}


	void set_current_thread_name(const std::string &name)
	{
		_thread_name.set(name);
	}


	const std::string thread_header(const std::string &thread_id, const std::string &thread_name)
	{
		std::stringstream stream;
		stream.fill(THREAD_HEADER_SEPARATOR);
		stream.flags(std::ios::left);
		stream.width(out_stream::width());
		stream << ("[Thread: " + thread_id + (!thread_name.empty() ? " " : "") + thread_name + "]");

		return stream.str();
	}


	const std::string &indentation()
	{
		return _indentation.get();
	}


	void indentation_add()
	{
		_indentation.set(_indentation.get() + INDENTATION);
	}


	void indentation_remove()
	{
		const std::string &old_indentation = _indentation.get();
		_indentation.set(old_indentation.substr(0, old_indentation.length() - INDENTATION_WIDTH));
	}


	out_stream::out_stream(const std::string &filename_line) :
		_current_line_length(0)
	{
		lock_output();

		if (!is_running_same_thread())
		{
			std::string thread_id = thread_id_field(current_thread_id());
			const std::string &thread_name = current_thread_name();
			const std::string &header = thread_header(thread_id, thread_name);
			*this << "\n" << header << "\n";
		}

		*this << filename_line << DELIMITER << indentation();
	}


	out_stream::out_stream() :
		_current_line_length(0)
	{
		lock_output();

		std::stringstream stream;
		stream.fill(' ');
		stream.width(FILENAME_FIELD_WIDTH + 1 + LINE_FIELD_WIDTH);
		stream << "";

		*this << stream.str() << DELIMITER << indentation();
	}


	out_stream::~out_stream()
	{
		flush();

		unlock_output();
	}


	out_stream &out_stream::operator <<(char character)
	{
		char string[2] = {character, '\0'};
		PRETTY_OUTPUT_REDIRECTION_NAMESPACE::print(string);
		++_current_line_length;

		return *this;
	}


	out_stream &out_stream::operator <<(const char *string)
	{
		PRETTY_OUTPUT_REDIRECTION_NAMESPACE::print(string);
		_current_line_length += std::strlen(string);

		return *this;
	}


	out_stream &out_stream::operator <<(const std::string &string)
	{
		return *this << string.c_str();
	}


	out_stream &out_stream::operator <<(const newline_manipulator &)
	{
		std::stringstream stream;
		stream.fill(' ');
		stream.width(FILENAME_FIELD_WIDTH + 1 + LINE_FIELD_WIDTH);
		stream << "";

		*this << "\n";
		_current_line_length = 0;
		*this << stream.str() << DELIMITER << indentation();

		return *this;
	}


	out_stream &out_stream::operator <<(const endline_manipulator &)
	{
		*this << "\n";
		_current_line_length = 0;

		return *this;
	}


	out_stream &out_stream::operator <<(const flush_manipulator &)
	{
		flush();
		return *this;
	}


	size_t out_stream::width_left() const
	{
		return out_stream::width() - _current_line_length;
	}


	void out_stream::printf(const char *format, ...)
	{
		va_list arguments;
		va_list arguments_copy;

		va_start(arguments, format);
		va_start(arguments_copy, format);

		size_t size = printf_string_length(format, arguments_copy) + 1;

		resource<void *> buffer(std::malloc(size), std::free);
		printf_to_string(static_cast<char *>(buffer.get()), size, format, arguments);
		*this << "// " << static_cast<char *>(buffer.get());

		va_end(arguments);
		va_end(arguments_copy);
	}


	void out_stream::flush()
	{
		PRETTY_OUTPUT_REDIRECTION_NAMESPACE::flush();
	}


	size_t out_stream::width()
	{
#if defined(PRETTY_OUTPUT_WIDTH)

		return PRETTY_OUTPUT_WIDTH;

#else

		return PRETTY_OUTPUT_REDIRECTION_NAMESPACE::width();

#endif // defined(PRETTY_OUTPUT_WIDTH)
	}


	out_stream &operator <<(out_stream &stream, const pretty<bool> &value)
	{
		stream << FLUSH;
		return stream << (value.get() ? "true" : "false");
	}


	out_stream &operator <<(out_stream &stream, const pretty<char> &value)
	{
		stream << FLUSH;
		return stream << "'" << value.get() << "'";
	}


	out_stream &operator <<(out_stream &stream, const pretty<const char *> &value)
	{
		stream << FLUSH;
		return stream << "\"" << value.get() << "\"";
	}


	out_stream &operator <<(out_stream &stream, const pretty<std::string> &value)
	{
		stream << FLUSH;
		return stream << "\"" << value.get() << "\"";
	}


	out_stream &operator <<(out_stream &stream, const pretty<short> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


	out_stream &operator <<(out_stream &stream, const pretty<unsigned short> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


	out_stream &operator <<(out_stream &stream, const pretty<int> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


	out_stream &operator <<(out_stream &stream, const pretty<unsigned int> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


	out_stream &operator <<(out_stream &stream, const pretty<long> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


	out_stream &operator <<(out_stream &stream, const pretty<unsigned long> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


#if defined(PRETTY_OUTPUT_CPP11)

	out_stream &operator <<(out_stream &stream, const pretty<long long> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


	out_stream &operator <<(out_stream &stream, const pretty<unsigned long long> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


	out_stream &operator <<(out_stream &stream, const pretty<float> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


	out_stream &operator <<(out_stream &stream, const pretty<double> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


	out_stream &operator <<(out_stream &stream, const pretty<long double> &value)
	{
		stream << FLUSH;
		return stream << to_string(value.get());
	}


	out_stream &operator <<(out_stream &stream, const pretty<const void *> &value)
	{
		stream << FLUSH;
		if (value.get() == NULL)
		{
			return stream << "(null)";
		}

		uintptr_t numeric_value = reinterpret_cast<uintptr_t>(value.get());
		return stream << to_string(numeric_value, std::hex, std::showbase, NULL);
	}


#if defined(PRETTY_OUTPUT_CPP11)

	out_stream &operator <<(out_stream &stream, const pretties<> &)
	{
		return stream;
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


	out_stream &operator <<(out_stream &stream, const pretty_bool<bool> &value)
	{
		stream << FLUSH;
		return stream << (value.get() ? "true" : "false");
	}

}
}


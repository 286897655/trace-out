#include "trace_out.h"

#if defined(TRACE_OUT_WINDOWS)
#include <Windows.h>
#endif

namespace trace_out {
	namespace detail
	{

		function_printer::function_printer(const std::string &filename_line, const char *function_signature)
			:
			_filename_line(filename_line),
			_function_signature(function_signature)
		{
			out_stream stream(_filename_line);
			stream << _function_signature << NEWLINE << "{" << ENDLINE;
			indentation_add();
		}


		function_printer::~function_printer()
		{
			indentation_remove();
			out_stream stream(_filename_line);
			stream << "}" << INDENTATION << "// " << _function_signature << NEWLINE << ENDLINE;
		}


		function_printer make_function_printer(const std::string &filename_line, const char *function_signature)
		{
			return function_printer(filename_line, function_signature);
		}



		return_printer::return_printer(const std::string &filename_line)
			:
			_filename_line(filename_line)
		{
		}


		return_printer make_return_printer(const std::string &filename_line)
		{
			return return_printer(filename_line);
		}

	}
}


namespace trace_out {
	namespace detail
	{

		const char *const BINARY_VALUES[] = {
			"00000000", "00000001", "00000010", "00000011", "00000100", "00000101", "00000110", "00000111",
			"00001000", "00001001", "00001010", "00001011", "00001100", "00001101", "00001110", "00001111",
			"00010000", "00010001", "00010010", "00010011", "00010100", "00010101", "00010110", "00010111",
			"00011000", "00011001", "00011010", "00011011", "00011100", "00011101", "00011110", "00011111",
			"00100000", "00100001", "00100010", "00100011", "00100100", "00100101", "00100110", "00100111",
			"00101000", "00101001", "00101010", "00101011", "00101100", "00101101", "00101110", "00101111",
			"00110000", "00110001", "00110010", "00110011", "00110100", "00110101", "00110110", "00110111",
			"00111000", "00111001", "00111010", "00111011", "00111100", "00111101", "00111110", "00111111",
			"01000000", "01000001", "01000010", "01000011", "01000100", "01000101", "01000110", "01000111",
			"01001000", "01001001", "01001010", "01001011", "01001100", "01001101", "01001110", "01001111",
			"01010000", "01010001", "01010010", "01010011", "01010100", "01010101", "01010110", "01010111",
			"01011000", "01011001", "01011010", "01011011", "01011100", "01011101", "01011110", "01011111",
			"01100000", "01100001", "01100010", "01100011", "01100100", "01100101", "01100110", "01100111",
			"01101000", "01101001", "01101010", "01101011", "01101100", "01101101", "01101110", "01101111",
			"01110000", "01110001", "01110010", "01110011", "01110100", "01110101", "01110110", "01110111",
			"01111000", "01111001", "01111010", "01111011", "01111100", "01111101", "01111110", "01111111",
			"10000000", "10000001", "10000010", "10000011", "10000100", "10000101", "10000110", "10000111",
			"10001000", "10001001", "10001010", "10001011", "10001100", "10001101", "10001110", "10001111",
			"10010000", "10010001", "10010010", "10010011", "10010100", "10010101", "10010110", "10010111",
			"10011000", "10011001", "10011010", "10011011", "10011100", "10011101", "10011110", "10011111",
			"10100000", "10100001", "10100010", "10100011", "10100100", "10100101", "10100110", "10100111",
			"10101000", "10101001", "10101010", "10101011", "10101100", "10101101", "10101110", "10101111",
			"10110000", "10110001", "10110010", "10110011", "10110100", "10110101", "10110110", "10110111",
			"10111000", "10111001", "10111010", "10111011", "10111100", "10111101", "10111110", "10111111",
			"11000000", "11000001", "11000010", "11000011", "11000100", "11000101", "11000110", "11000111",
			"11001000", "11001001", "11001010", "11001011", "11001100", "11001101", "11001110", "11001111",
			"11010000", "11010001", "11010010", "11010011", "11010100", "11010101", "11010110", "11010111",
			"11011000", "11011001", "11011010", "11011011", "11011100", "11011101", "11011110", "11011111",
			"11100000", "11100001", "11100010", "11100011", "11100100", "11100101", "11100110", "11100111",
			"11101000", "11101001", "11101010", "11101011", "11101100", "11101101", "11101110", "11101111",
			"11110000", "11110001", "11110010", "11110011", "11110100", "11110101", "11110110", "11110111",
			"11111000", "11111001", "11111010", "11111011", "11111100", "11111101", "11111110", "11111111"
		};

		const char *const HEXADECIMAL_VALUES[] = {
			"00", "01", "02", "03", "04", "05", "06", "07",
			"08", "09", "0a", "0b", "0c", "0d", "0e", "0f",
			"10", "11", "12", "13", "14", "15", "16", "17",
			"18", "19", "1a", "1b", "1c", "1d", "1e", "1f",
			"20", "21", "22", "23", "24", "25", "26", "27",
			"28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
			"30", "31", "32", "33", "34", "35", "36", "37",
			"38", "39", "3a", "3b", "3c", "3d", "3e", "3f",
			"40", "41", "42", "43", "44", "45", "46", "47",
			"48", "49", "4a", "4b", "4c", "4d", "4e", "4f",
			"50", "51", "52", "53", "54", "55", "56", "57",
			"58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
			"60", "61", "62", "63", "64", "65", "66", "67",
			"68", "69", "6a", "6b", "6c", "6d", "6e", "6f",
			"70", "71", "72", "73", "74", "75", "76", "77",
			"78", "79", "7a", "7b", "7c", "7d", "7e", "7f",
			"80", "81", "82", "83", "84", "85", "86", "87",
			"88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
			"90", "91", "92", "93", "94", "95", "96", "97",
			"98", "99", "9a", "9b", "9c", "9d", "9e", "9f",
			"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
			"a8", "a9", "aa", "ab", "ac", "ad", "ae", "af",
			"b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7",
			"b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
			"c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7",
			"c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf",
			"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
			"d8", "d9", "da", "db", "dc", "dd", "de", "df",
			"e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7",
			"e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
			"f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
			"f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff"
		};

		const char *const BASE_NAMES[] = { "binary", "signed decimal", "unsigned decimal", "hexadecimal", "float", "double", "long double" };
		const size_t BASE_NAMES_LENGTH = sizeof(BASE_NAMES) / sizeof(BASE_NAMES[0]);

		const char *const BYTE_ORDER_NAMES[] = { "little-endian", "big-endian" };
		const size_t BYTE_ORDER_NAMES_LENGTH = sizeof(BYTE_ORDER_NAMES) / sizeof(BYTE_ORDER_NAMES[0]);


		template <typename T>
		size_t first_set_bit(T number)
		{
			size_t index = 0;
			for (;;)
			{
				if ((number & static_cast<T>(0x1)) == static_cast<T>(0x1))
				{
					break;
				}

				number >>= 1;
				++index;
			}

			return index;
		}


		option_t base_value_from_options(option_t options, option_t default_value)
		{
			option_t base = options & (static_cast<option_t>(0x0000ffff) << OPTIONS_START_BASE);
			if (base == 0)
			{
				return default_value;
			}

			return base;
		}


		option_t byte_order_value_from_options(option_t options, option_t default_value)
		{
			option_t byte_order = options & (static_cast<option_t>(0x0000ffff) << OPTIONS_START_BYTE_ORDER);
			if (byte_order == 0)
			{
				return default_value;
			}

			return byte_order;
		}


		const char *option_name(option_t option, const char *const names[], size_t names_length, const char *default_name)
		{
			size_t index = first_set_bit(option);
			if (index >= names_length)
			{
				return default_name;
			}

			return names[index];
		}


		const char *byte_to_binary(uint8_t byte)
		{
			return BINARY_VALUES[byte];
		}


		const char *byte_to_hexadecimal(uint8_t byte)
		{
			return HEXADECIMAL_VALUES[byte];
		}


		option_t current_byte_order()
		{
			const uint16_t VALUE = static_cast<uint16_t>(0x0001);
			const uint8_t FIRST_BYTE = *reinterpret_cast<const uint8_t *>(&VALUE);

			if (FIRST_BYTE == static_cast<uint8_t>(0x01))
			{
				return BIG;
			}
			else
			{
				return LITTLE;
			}
		}


		void reverse_bytes(void *destination, const void *source, size_t size)
		{
			uint8_t *destination_iterator = static_cast<uint8_t *>(destination);
			const uint8_t *source_iterator = static_cast<const uint8_t *>(source)+size - 1;
			for (size_t bytes_processed = 0; bytes_processed < size; ++bytes_processed)
			{
				*destination_iterator = *source_iterator;
				++destination_iterator;
				--source_iterator;
			}
		}


		void order_bytes(void *ordered_bytes, const void *unordered_bytes, size_t size, option_t byte_order)
		{
			if (current_byte_order() != byte_order)
			{
				reverse_bytes(ordered_bytes, unordered_bytes, size);
			}
			else
			{
				std::memcpy(ordered_bytes, unordered_bytes, size);
			}
		}

	}
}



namespace trace_out {
	namespace detail
	{

		mutex::mutex()
			:
			_handle(mutex_new(), mutex_delete)
		{
		}


		void mutex::lock()
		{
			mutex_lock(_handle.get());
		}


		void mutex::unlock()
		{
			mutex_unlock(_handle.get());
		}

	}
}


#if defined(TRACE_OUT_REDIRECTION)
#define TRACE_OUT_REDIRECTION_NAMESPACE TRACE_OUT_REDIRECTION
#else
#define TRACE_OUT_REDIRECTION_NAMESPACE trace_out_to_stdout
#endif


namespace TRACE_OUT_REDIRECTION_NAMESPACE
{

	void print(const char *string);
	void flush();
	size_t width();

}


namespace trace_out {
	namespace detail
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


namespace trace_out {
	namespace detail
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
#if !defined(TRACE_OUT_NO_OUTPUT_SYNC)

			_output_mutex.lock();

#endif // !defined(TRACE_OUT_NO_OUTPUT_SYNC)
		}


		void unlock_output()
		{
#if !defined(TRACE_OUT_NO_OUTPUT_SYNC)

			_output_mutex.unlock();

#endif // !defined(TRACE_OUT_NO_OUTPUT_SYNC)
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
			stream.width(static_cast<std::streamsize>(out_stream::width()));
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


		out_stream::out_stream(const std::string &filename_line)
			:
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


		out_stream::out_stream()
			:
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
			char string[2] = { character, '\0' };
			TRACE_OUT_REDIRECTION_NAMESPACE::print(string);
			++_current_line_length;

			return *this;
		}


		out_stream &out_stream::operator <<(const char *string)
		{
			TRACE_OUT_REDIRECTION_NAMESPACE::print(string);
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

		void out_stream::flush()
		{
			TRACE_OUT_REDIRECTION_NAMESPACE::flush();
		}


		size_t out_stream::width()
		{
#if defined(TRACE_OUT_WIDTH)

			return TRACE_OUT_WIDTH;

#else

			return TRACE_OUT_REDIRECTION_NAMESPACE::width();

#endif // defined(TRACE_OUT_WIDTH)
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


		out_stream &operator <<(out_stream &stream, const pretty<unsigned char> &value)
		{
			stream << FLUSH;
			return stream << to_string(static_cast<unsigned int>(value.get()));
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


#if defined(TRACE_OUT_CPP11)

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

#endif // defined(TRACE_OUT_CPP11)


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


		out_stream &operator <<(out_stream &stream, const pretty_bool<bool> &value)
		{
			stream << FLUSH;
			return stream << (value.get() ? "true" : "false");
		}

	}
}

#if defined(TRACE_OUT_POSIX)



namespace trace_out {
	namespace detail
	{

		const char FILE_PATH_COMPONENT_DELIMITER = '/';

	}
}


#endif // defined(TRACE_OUT_POSIX)

#if defined(TRACE_OUT_POSIX)

#include <cstdio>



namespace trace_out {
	namespace detail
	{

		size_t printf_string_length(const char *format, va_list arguments)
		{
			int retval = vsnprintf(NULL, 0, format, arguments);
			return static_cast<size_t>(retval);
		}


		size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments)
		{
			int retval = vsnprintf(buffer, size, format, arguments);
			return static_cast<size_t>(retval);
		}

	}
}


#endif // defined(TRACE_OUT_POSIX)

#if defined(TRACE_OUT_POSIX)

#include <sys/ioctl.h>
#include <unistd.h>



namespace trace_out {
	namespace detail
	{

		int console_width()
		{
			winsize window_size;
			int retval = ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);
			if (retval == -1)
			{
				return -1;
			}

			return static_cast<int>(window_size.ws_col) - 1;
		}

	}
}


#endif // defined(TRACE_OUT_POSIX)

#if defined(TRACE_OUT_POSIX)

#include <cstddef>
#include <cassert>
#include <pthread.h>



namespace trace_out {
	namespace detail
	{

		struct _mutex
		{
			pthread_mutex_t value;
		};


		mutex_t mutex_new()
		{
			mutex_t mutex = new _mutex;
			int retval = pthread_mutex_init(&mutex->value, NULL);
			assert(retval == 0);

			return mutex;
		}


		void mutex_delete(mutex_t mutex)
		{
			int retval = pthread_mutex_destroy(&mutex->value);
			assert(retval == 0);

			delete mutex;
		}


		void mutex_lock(mutex_t mutex)
		{
			int retval = pthread_mutex_lock(&mutex->value);
			assert(retval == 0);
		}


		void mutex_unlock(mutex_t mutex)
		{
			int retval = pthread_mutex_unlock(&mutex->value);
			assert(retval == 0);
		}

	}
}


#endif // defined(TRACE_OUT_POSIX)

#if defined(TRACE_OUT_POSIX)

#include <cstdint>
#include <pthread.h>



namespace trace_out {
	namespace detail
	{

		uint64_t current_thread_id()
		{
			return reinterpret_cast<uint64_t>(pthread_self());
		}

	}
}


#endif // defined(TRACE_OUT_POSIX)

#if defined(TRACE_OUT_POSIX)

#include <cstddef>
#include <cassert>
#include <pthread.h>



namespace trace_out {
	namespace detail
	{

		struct _tlskey
		{
			pthread_key_t value;
		};


		tlskey_t tls_new_key()
		{
			tlskey_t key = new _tlskey;
			int retval = pthread_key_create(&key->value, NULL);
			assert(retval == 0);

			return key;
		}


		void tls_delete_key(tlskey_t key)
		{
			int retval = pthread_key_delete(key->value);
			assert(retval == 0);

			delete key;
		}


		void *tls_get(tlskey_t key)
		{
			return pthread_getspecific(key->value);
		}


		void tls_set(tlskey_t key, void *data)
		{
			int retval = pthread_setspecific(key->value, data);
			assert(retval == 0);
		}

	}
}


#endif // defined(TRACE_OUT_POSIX)

#if defined(TRACE_OUT_POSIX)

#include <cstddef>
#include <cassert>
#include <sys/time.h>



namespace trace_out {
	namespace detail
	{

		uint64_t time_in_milliseconds()
		{
			struct timeval time_value;
			int retval = gettimeofday(&time_value, NULL);
			assert(retval == 0);

			uint64_t microseconds = static_cast<uint64_t>(time_value.tv_usec);
			uint64_t seconds = static_cast<uint64_t>(time_value.tv_sec);
			uint64_t milliseconds = seconds * 1000 + microseconds / 1000;

			return milliseconds;
		}

	}
}


#endif // defined(TRACE_OUT_POSIX)



namespace trace_out {
	namespace detail
	{

		auto_indentation::auto_indentation()
		{
			indentation_add();
		}


		auto_indentation::~auto_indentation()
		{
			indentation_remove();
		}


		block::block(bool value)
			:
			_auto_indentation(),
			_value(value)
		{
		}


		block::block(const block &another)
			:
			_auto_indentation(),
			_value(another._value)
		{
		}


		block::~block()
		{
			out_stream stream;
			stream << ENDLINE;
		}


		block::operator bool() const
		{
			return _value;
		}


		block iteration_block(const std::string &filename_line, size_t iteration)
		{
			{
				auto_indentation auto_indentation;

				out_stream stream(filename_line);
				stream << "// for: iteration #" << make_pretty(iteration) << ENDLINE;
			}

			return block(false);
		}



		for_block::for_block(const std::string &filename_line, const char *expression)
			:
			_iteration_number(0)
		{
			out_stream stream(filename_line);
			stream << "for (" << expression << ")" << ENDLINE;
		}


		for_block::~for_block()
		{
		}


		for_block::operator bool() const
		{
			return false;
		}


		size_t for_block::iteration()
		{
			return ++_iteration_number;
		}


		for_block make_for_block(const std::string &filename_line, const char *expression)
		{
			return for_block(filename_line, expression);
		}



		void print_while_header(const std::string &filename_line, const char *condition)
		{
			out_stream stream(filename_line);
			stream << "while (" << condition << ")" << ENDLINE;
		}

	}
}


namespace trace_out {
	namespace detail
	{

		const std::string filename_from_path(const char *path)
		{
			std::string file_path(path);
			return file_path.substr(file_path.rfind(FILE_PATH_COMPONENT_DELIMITER) + 1);
		}


		const std::string filename_line_field(const std::string &file, unsigned int line)
		{
			std::stringstream stream;
			stream.fill(' ');

			stream.width(FILENAME_FIELD_WIDTH);
			stream.flags(std::ios::right);
			std::string filename = file;
			if (filename.length() > FILENAME_FIELD_WIDTH)
			{
				filename = filename.substr(0, FILENAME_FIELD_WIDTH - FILENAME_FIELD_EXCESS_PADDING_SIZE);
				filename += FILENAME_FIELD_EXCESS_PADDING;
			}

			stream << filename;

			stream.width(0);
			stream << ":";

			stream.width(LINE_FIELD_WIDTH);
			stream.flags(std::ios::left);
			stream << line;

			return stream.str();
		}


		void apply_io_manipulators(std::ostream &stream, va_list manipulators)
		{
			for (;;)
			{
				manipulator_t manipulator = va_arg(manipulators, manipulator_t);
				if (manipulator == NULL)
				{
					break;
				}

				stream << manipulator;
			}
		}


		std::string first_token(const std::string &tokens)
		{
			return tokens.substr(0, tokens.find(','));
		}


		std::string rest_tokens(const std::string &tokens)
		{
			size_t from = tokens.find(',') + 1;
			while (tokens[from] == ' ')
			{
				++from;
			}

			return tokens.substr(from, tokens.size());
		}

	}
}




namespace trace_out {
	namespace detail
	{

		void print_execution_time_in_milliseconds(const std::string &filename_line, uint64_t milliseconds)
		{
			out_stream stream(filename_line);
			stream << "// execution time: " << to_string(milliseconds) << " ms" << ENDLINE;
		}


		void print_execution_time_in_ticks(const std::string &filename_line,const std::string &usermessage ,std::clock_t ticks, double milliseconds)
		{
			out_stream stream(filename_line);
			stream<< "//" << usermessage << "execution time: " << to_string(ticks) << " ticks (" << to_string(milliseconds) << " ms)" << ENDLINE;
		}

	}
}

#include <fstream>
#include <cassert>


#define TRACE_OUT_TO_FILE__QUOTIZE_IMPL(something) \
			#something

#define TRACE_OUT_TO_FILE__QUOTIZE(something) \
			TRACE_OUT_TO_FILE__QUOTIZE_IMPL(something)


#if !defined(TRACE_OUT_TO_FILE)
#define TRACE_OUT_TO_FILE trace-out.txt
#endif


namespace trace_out_to_file
{

	std::ofstream stream;


	void print(const char *string)
	{
		if (!stream.is_open())
		{
			stream.open(TRACE_OUT_TO_FILE__QUOTIZE(TRACE_OUT_TO_FILE));
		}

		stream << string;
	}


	void flush()
	{
		assert(stream.is_open());

		stream.flush();
	}


	size_t width()
	{
		return 120;
	}

}

#if !defined(TRACE_OUT_REDIRECTION)

#include <cstddef>
#include <iostream>



namespace trace_out_to_stdout
{

	void print(const char *string);
	void flush();
	size_t width();

}


namespace trace_out_to_stdout
{

	const size_t DEFAULT_WIDTH = 79;


	void print(const char *string)
	{
		std::cout << string;
	}


	void flush()
	{
		std::cout.flush();
	}


	size_t width()
	{
		int width = trace_out::detail::console_width();
		if (width == -1)
		{
			return DEFAULT_WIDTH;
		}

		return static_cast<size_t>(width);
	}

}


#endif // !defined(TRACE_OUT_REDIRECTION)

#if defined(TRACE_OUT_WINDOWS)
namespace trace_out_to_wdo
{

	void print(const char *string)
	{
		OutputDebugStringA(string);
	}


	void flush()
	{
	}


	size_t width()
	{
		return 120;
	}

}

namespace trace_out {
	namespace detail
	{

		const char FILE_PATH_COMPONENT_DELIMITER = '\\';

	}
}

#endif // defined(TRACE_OUT_WINDOWS)

#if defined(TRACE_OUT_WINDOWS)
namespace trace_out {
	namespace detail
	{

#if defined(TRACE_OUT_MVS)

		int vsnprintf(char *buffer, size_t size, const char *format, va_list arguments);

#endif // defined(TRACE_OUT_MVS)

	}
}


namespace trace_out {
	namespace detail
	{

		size_t printf_string_length(const char *format, va_list arguments)
		{
			int retval = _vscprintf(format, arguments);
			return static_cast<size_t>(retval);
		}


		size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments)
		{
			int retval = vsnprintf(buffer, size, format, arguments);
			return static_cast<size_t>(retval);
		}


#if defined(TRACE_OUT_MVS)

		int vsnprintf(char *buffer, size_t size, const char *format, va_list arguments)
		{
			return _vsnprintf_s(buffer, size, _TRUNCATE, format, arguments);
		}

#endif // defined(TRACE_OUT_MVS)

	}
}


#endif // defined(TRACE_OUT_WINDOWS)

#if defined(TRACE_OUT_WINDOWS)
namespace trace_out {
	namespace detail
	{

		struct _mutex
		{
			CRITICAL_SECTION value;
		};


		mutex_t mutex_new()
		{
			mutex_t mutex = new _mutex;
			InitializeCriticalSection(&mutex->value);

			return mutex;
		}


		void mutex_delete(mutex_t mutex)
		{
			DeleteCriticalSection(&mutex->value);
		}

		void mutex_lock(mutex_t mutex)
		{
			EnterCriticalSection(&mutex->value);
		}

		void mutex_unlock(mutex_t mutex)
		{
			LeaveCriticalSection(&mutex->value);
		}
	}
}

#endif // defined(TRACE_OUT_WINDOWS)

#if defined(TRACE_OUT_WINDOWS)
#include <cassert>
namespace trace_out {
	namespace detail
	{
		struct _tlskey
		{
			DWORD value;
		};

		tlskey_t tls_new_key()
		{
			tlskey_t key = new _tlskey;
			DWORD retval = TlsAlloc();
			assert(retval != TLS_OUT_OF_INDEXES);

			key->value = retval;
			return key;
		}

		void tls_delete_key(tlskey_t key)
		{
			int retval = TlsFree(key->value);
			assert(retval != 0);

			delete key;
		}

		void *tls_get(tlskey_t key)
		{
			return TlsGetValue(key->value);
		}

		void tls_set(tlskey_t key, void *data)
		{
			BOOL retval = TlsSetValue(key->value, data);
			assert(retval != 0);
		}

		uint64_t time_in_milliseconds()
		{
			FILETIME file_time;
			ULARGE_INTEGER large_integer;

			GetSystemTimeAsFileTime(&file_time);
			large_integer.LowPart = file_time.dwLowDateTime;
			large_integer.HighPart = file_time.dwHighDateTime;

			uint64_t milliseconds = (large_integer.QuadPart - 116444736000000000LL) / 10000;

			return milliseconds;
		}

		uint64_t current_thread_id()
		{
			return static_cast<uint64_t>(GetCurrentThreadId());
		}

		int console_width()
		{
			CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

			BOOL retval = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen_buffer_info);
			if (retval == 0)
			{
				return -1;
			}

			int width = static_cast<int>(screen_buffer_info.srWindow.Right - screen_buffer_info.srWindow.Left);
			return width;
		}
	}
}
#endif // defined(TRACE_OUT_WINDOWS)

#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <sstream>

#include "stuff.h"
#include "out_stream.h"


namespace pretty_output { namespace detail
{

	typedef uint32_t option_t;

	const size_t OPTIONS_START_BASE = 0;
	const size_t OPTIONS_START_BYTE_ORDER = 16;

}
}


namespace pretty_output
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


namespace pretty_output { namespace detail
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

#define PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(family, type_size, is_signed, unit_type, field_width_value, default_base_value, to_signed_type, to_unsigned_type) \
		template <> \
		struct print_traits_details<family, type_size, is_signed> \
		{ \
			typedef unit_type unit_t; \
			static const outputwidth_t field_width = field_width_value; \
			static const option_t default_base = default_base_value; \
			typedef to_signed_type signed_t; \
			typedef to_unsigned_type unsigned_t; \
		}

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, true, int8_t, 4, HEX, int8_t, uint8_t);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, true, int16_t, 6, SDEC, int16_t, uint16_t);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, true, int32_t, 11, SDEC, int32_t, uint32_t);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, true, int64_t, 21, SDEC, int64_t, uint64_t);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, false, uint8_t, 3, HEX, int8_t, uint8_t);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, false, uint16_t, 5, UDEC, int16_t, uint16_t);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, false, uint32_t, 10, UDEC, int32_t, uint32_t);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, false, uint64_t, 20, UDEC, int64_t, uint64_t);

	// sign + first_digit + point + precision + 'e' + exponent_sign + exponent
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 4, true, float, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 3, FLT, float, float);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 8, true, double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 4, DBL, double, double);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 10, true, long double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 12, true, long double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 16, true, long double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);

#undef PRETTY_OUTPUT__DEFINE_PRINT_TRAITS


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
	const std::string (*select_conversion(option_t base))(Type_t);

	option_t current_byte_order();
	void reverse_bytes(void *destination, const void *source, size_t size);
	void order_bytes(void *ordered_bytes, const void *unordered_bytes, size_t size, option_t byte_order);

	template <typename Type_t>
	void print_memory(const std::string &filename_line, const char *name, const Type_t *pointer, size_t size = sizeof(Type_t), option_t options = 0);

}
}


namespace pretty_output { namespace detail
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
	const std::string (*select_conversion(option_t base))(Type_t)
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
	void print_memory_contents(out_stream &stream, const Type_t *pointer, size_t size, outputwidth_t column_width, const std::string (*bytes_to_string)(Type_t), option_t byte_order)
	{
		std::stringstream string_stream;

		const Type_t *iterator = pointer;
		size_t length = size / sizeof(Type_t);

		stream << make_pretty(static_cast<const void *>(iterator)) << ":";
		for (std::size_t index = 0; index < length; ++index)
		{
			const std::string string_representation = string_stream.str();
			if (string_representation.length() + static_cast<size_t>(column_width) + 1 > stream.width_left())
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

		const std::string (*bytes_to_string)(Type_t) = select_conversion<Type_t>(base);
		outputwidth_t column_width = field_width<Type_t>(base);
		print_memory_contents(stream, static_cast<const unit_t *>(pointer), size, column_width, bytes_to_string, byte_order);

		indentation_remove();

		stream << ENDLINE;
	}

}
}


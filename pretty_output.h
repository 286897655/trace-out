#pragma once

#include <cstdint>
#include <ctime>

#include "detail/platform_defines.h"
#include "detail/out_stream.h"
#include "detail/watch_printer.h"
#include "detail/function_printer.h"
#include "detail/memory_printer.h"
#include "detail/statements_printer.h"
#include "detail/system_time.h"
#include "detail/time_printer.h"


#if defined(PRETTY_OUTPUT_CLANG)

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
	#pragma clang diagnostic ignored "-Wvariadic-macros"
	#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#elif defined(PRETTY_OUTPUT_GCC)

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvariadic-macros" // doesn't work with MinGW and probably also with GCC

#elif defined(PRETTY_OUTPUT_MVS)

	#pragma warning(push)
	#pragma warning(disable:4296) // expression is always false

#endif


#if (!defined(NDEBUG) && !defined(PRETTY_OUTPUT_OFF)) || defined(PRETTY_OUTPUT_ON)

	#define $e(...) \
				pretty_output::detail::expression(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, ##__VA_ARGS__)

	#define $w(...) \
				pretty_output::detail::watch(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, ##__VA_ARGS__);

	#define $m(pointer, ...) \
				pretty_output::detail::print_memory(PRETTY_OUTPUT_FILENAME_LINE, #pointer, pointer, ##__VA_ARGS__);

	#define $f \
				pretty_output::detail::function_printer PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_f) = pretty_output::detail::make_function_printer(PRETTY_OUTPUT_FILENAME_LINE, PRETTY_OUTPUT_FUNCTION_SIGNATURE);

	#define $return \
				return pretty_output::detail::make_return_printer(PRETTY_OUTPUT_FILENAME_LINE) ,

	#define $if(...) \
				if (pretty_output::detail::block PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_if_block) = pretty_output::detail::if_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, (__VA_ARGS__)))

	#define pretty_output_private__for(block_variable_name, ...) \
				if (pretty_output::detail::for_block block_variable_name = pretty_output::detail::make_for_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__)) {} else \
					for (__VA_ARGS__) \
						if (pretty_output::detail::block PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_iteration_block) = pretty_output::detail::iteration_block(PRETTY_OUTPUT_FILENAME_LINE, block_variable_name.iteration())) {} else

	#define $for(...) \
				pretty_output_private__for(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_for_block), ##__VA_ARGS__)

	#define $while(...) \
				if (pretty_output::detail::print_while_header(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__), false) {} else \
					while (pretty_output::detail::block PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_while_block) = pretty_output::detail::while_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, (__VA_ARGS__)))

	#define $p(format, ...) \
				{ \
					pretty_output::detail::out_stream stream(PRETTY_OUTPUT_FILENAME_LINE); \
					stream.printf(format, ##__VA_ARGS__); \
					stream << pretty_output::detail::ENDLINE; \
				}

	#define $thread(name) \
				pretty_output::detail::set_current_thread_name(#name);

	#define pretty_output_private__time(start_time_variable, end_time_variable, ...) \
				{ \
					uint64_t start_time_variable = pretty_output::detail::time_in_milliseconds(); \
					__VA_ARGS__ \
					uint64_t end_time_variable = pretty_output::detail::time_in_milliseconds(); \
					pretty_output::detail::print_execution_time_in_milliseconds(PRETTY_OUTPUT_FILENAME_LINE, end_time_variable - start_time_variable); \
				}

	#define $time(...) \
				pretty_output_private__time(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_start_ticks), PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_end_ticks), ##__VA_ARGS__)

	#define pretty_output_private__ticks(start_ticks_variable, end_ticks_variable, ...) \
				{ \
					std::clock_t start_ticks_variable = std::clock(); \
					__VA_ARGS__ \
					std::clock_t end_ticks_variable = std::clock(); \
					std::clock_t execution_time = end_ticks_variable - start_ticks_variable; \
					pretty_output::detail::print_execution_time_in_ticks(PRETTY_OUTPUT_FILENAME_LINE, execution_time, (static_cast<double>(execution_time)) / CLOCKS_PER_SEC * 1000); \
				}

	#define $ticks(...) \
				pretty_output_private__ticks(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_start_time), PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_end_time), ##__VA_ARGS__)

#elif defined(NDEBUG) || defined(PRETTY_OUTPUT_OFF)

	#define $e(...)
				(__VA_ARGS__)

	#define $w(...)

	#define $m(pointer, ...)

	#define $f

	#define $return \
				return

	#define $if(...) \
				if (__VA_ARGS__)

	#define $for(...) \
				for (__VA_ARGS__)

	#define $while(...) \
				while (__VA_ARGS__)

	#define $_

	#define $p(format, ...)

	#define $thread(name)

	#define $time(...) \
				__VA_ARGS__

	#define $ticks(...) \
				__VA_ARGS__

#endif



#if defined(__clang__)

	#pragma clang diagnostic pop

#elif defined(__GNUC__) || defined(__GNUG__)

	#pragma GCC diagnostic pop

#elif defined(_MSC_VER)

	#pragma warning(pop)

#endif


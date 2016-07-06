#pragma once

#include <cstdint>
#include <ctime>

#include "detail/platform_defines.hpp"
#include "detail/out_stream.hpp"
#include "detail/watch_printer.hpp"
#include "detail/function_printer.hpp"
#include "detail/memory_printer.hpp"
#include "detail/statements_printer.hpp"
#include "detail/system_time.hpp"
#include "detail/time_printer.hpp"


#if defined(TRACE_OUT_CLANG)

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
	#pragma clang diagnostic ignored "-Wvariadic-macros"
	#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#elif defined(TRACE_OUT_GCC)

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvariadic-macros" // doesn't work with MinGW and probably also with GCC

#endif


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

	#define $if(...) \
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

	#define $p(format, ...) \
				{ \
					trace_out::detail::out_stream stream(TRACE_OUT_FILENAME_LINE); \
					stream.printf(format, ##__VA_ARGS__); \
					stream << trace_out::detail::ENDLINE; \
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

	#define trace_out_private__ticks(start_ticks_variable, end_ticks_variable, ...) \
				{ \
					std::clock_t start_ticks_variable = std::clock(); \
					__VA_ARGS__ \
					std::clock_t end_ticks_variable = std::clock(); \
					std::clock_t execution_time = end_ticks_variable - start_ticks_variable; \
					trace_out::detail::print_execution_time_in_ticks(TRACE_OUT_FILENAME_LINE, execution_time, (static_cast<double>(execution_time)) / CLOCKS_PER_SEC * 1000); \
				}

	#define $ticks(...) \
				trace_out_private__ticks(TRACE_OUT_PRIVATE__UNIFY(trace_out_start_time), TRACE_OUT_PRIVATE__UNIFY(trace_out_end_time), ##__VA_ARGS__)

#elif defined(NDEBUG) || defined(TRACE_OUT_OFF)

	#define $e(...) \
				__VA_ARGS__

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

#endif


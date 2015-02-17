#include "pretty_output_to_file.h"


#define PRETTY_OUTPUT_TO_FILE__QUOTIZE_IMPL(something) \
			#something

#define PRETTY_OUTPUT_TO_FILE__QUOTIZE(something) \
			PRETTY_OUTPUT_TO_FILE__QUOTIZE_IMPL(something)


std::ofstream pretty_output_stream(PRETTY_OUTPUT_TO_FILE__QUOTIZE(PRETTY_OUTPUT_TO_FILE));


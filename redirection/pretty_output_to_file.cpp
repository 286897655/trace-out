#include <fstream>


#define PRETTY_OUTPUT_TO_FILE__QUOTIZE_IMPL(something) \
			#something

#define PRETTY_OUTPUT_TO_FILE__QUOTIZE(something) \
			PRETTY_OUTPUT_TO_FILE__QUOTIZE_IMPL(something)


#if !defined(PRETTY_OUTPUT_TO_FILE)
	#define PRETTY_OUTPUT_TO_FILE pretty_output_log.txt
#endif


namespace pretty_output_to_file
{

	std::ofstream stream(PRETTY_OUTPUT_TO_FILE__QUOTIZE(PRETTY_OUTPUT_TO_FILE));


	void print(const char *string)
	{
		stream << string;
	}


	void flush()
	{
		stream.flush();
	}

}


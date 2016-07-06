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


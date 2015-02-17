#include <fstream>


extern std::ofstream pretty_output_stream;


inline void pretty_output_print(const char *string)
{
	pretty_output_stream << string;
}


inline void pretty_output_flush()
{
	pretty_output_stream.flush();
}


#if defined(TRACE_OUT_WINDOWS)

#include <windows.h>


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

#endif // defined(TRACE_OUT_WINDOWS)

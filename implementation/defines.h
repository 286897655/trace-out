//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#pragma once


#if __cplusplus >= 201103L || _MSC_VER >= 1800
	#define PRETTY_OUTPUT_CPP11
#endif


#if defined(PRETTY_OUTPUT_REDIRECTION)
	#define PRETTY_OUTPUT_REDIRECTION_NAMESPACE PRETTY_OUTPUT_REDIRECTION
#else
	#define PRETTY_OUTPUT_REDIRECTION_NAMESPACE pretty_output_to_stdout
#endif


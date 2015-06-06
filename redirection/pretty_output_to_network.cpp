//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>


#define PRETTY_OUTPUT_TO_NETWORK__QUOTIZE_IMPL(something) \
			#something

#define PRETTY_OUTPUT_TO_NETWORK__QUOTIZE(something) \
			PRETTY_OUTPUT_TO_NETWORK__QUOTIZE_IMPL(something)


#if !defined(PRETTY_OUTPUT_TO_HOST)
	#define PRETTY_OUTPUT_TO_HOST 127.0.0.1
#endif

#if !defined(PRETTY_OUTPUT_TO_PORT)
	#define PRETTY_OUTPUT_TO_PORT 45678
#endif


namespace pretty_output_to_network
{

	class tcp_socket
	{
	public:
		tcp_socket(const char *host_string, int port);
		~tcp_socket();

		void send(const void *buffer, size_t size);

	private:
		int _handle;
	};

	tcp_socket server_socket(PRETTY_OUTPUT_TO_NETWORK__QUOTIZE(PRETTY_OUTPUT_TO_HOST), PRETTY_OUTPUT_TO_PORT);


	void print(const char *string)
	{
		server_socket.send(string, strlen(string));
	}


	void flush()
	{
	}


	tcp_socket::tcp_socket(const char *host_string, int port)
	{
		_handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		assert(_handle != -1);

		struct sockaddr_in socket_address;
		socket_address.sin_family = AF_INET;

		struct hostent *host_entry = gethostbyname(host_string);
		memcpy(&(socket_address.sin_addr.s_addr), host_entry->h_addr, host_entry->h_length);
		socket_address.sin_port = htons(port);
		int retval = connect(_handle, (struct sockaddr *)&socket_address, sizeof(socket_address));
		assert(retval != -1);

		retval = shutdown(_handle, SHUT_RD);
		assert(retval != -1);
	}


	tcp_socket::~tcp_socket()
	{
		int retval = shutdown(_handle, SHUT_WR);
		assert(retval != -1);

		retval = close(_handle);
		assert(retval != -1);
	}


	void tcp_socket::send(const void *buffer, size_t size)
	{
		ssize_t retval = ::send(_handle, buffer, size, 0);
		assert(retval != -1);
	}

}


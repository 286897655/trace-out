#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char *HELP_STRING =
	"Usage:\n"
	"	catch <host> <port>";

const size_t BUFFER_SIZE = 1024;


struct sockaddr_in sockaddr_new(const char *host_string, int port)
{
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;

	struct hostent *host_entry = gethostbyname(host_string);
	memcpy(&(socket_address.sin_addr.s_addr), host_entry->h_addr, host_entry->h_length);

	socket_address.sin_port = htons(port);

	return socket_address;
}


int port_from_string(const char *string)
{
	return atoi(string);
}


int main(int argc, const char *argv[])
{
	if (argc != 3)
	{
		printf("%s\n", HELP_STRING);
		return -1;
	}

	const char *host = argv[1];
	int port = port_from_string(argv[2]);

	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == -1)
	{
		printf("%s\n", strerror(errno));
		return -1;
	}

	printf("Opening sockets...\n");
	struct sockaddr_in socket_address = sockaddr_new(host, port);
	int retval = bind(server_socket, (struct sockaddr *)&socket_address, sizeof(socket_address));
	if (retval == -1)
	{
		printf("%s\n", strerror(errno));
		return -1;
	}

	retval = listen(server_socket, 1);
	if (retval == -1)
	{
		printf("%s\n", strerror(errno));
		return -1;
	}

	for (;;)
	{

		printf("Waiting for a client...\n");
		int client_socket = accept(server_socket, NULL, NULL);
		if (client_socket == -1)
		{
			printf("%s\n", strerror(errno));
			return -1;
		}

		shutdown(client_socket, SHUT_WR);

		printf("Client connected\n");

		for (;;)
		{
			char buffer[BUFFER_SIZE];
			ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
			if (bytes_read == -1)
			{
				printf("%s\n", strerror(errno));
				return -1;
			}

			if (bytes_read == 0)
			{
				break;
			}

			buffer[bytes_read] = 0;
			printf("%s", buffer);
		}

		shutdown(client_socket, SHUT_RDWR);
		close(client_socket);

	}

	shutdown(server_socket, SHUT_RDWR);
	close(server_socket);

	return 0;
}


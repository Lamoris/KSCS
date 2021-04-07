#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <assert.h>
#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#define BUFFER_SIZE 256
#define PORT 65429

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[])
{
	int temp;
	char buffer[BUFFER_SIZE];
	ssize_t bufferSize;
	
	WSADATA wsaData;
	SOCKET sock;
	struct timeval timeout;
	socklen_t socklen1;
	struct sockaddr_in sockaddr1;
	socklen_t socklen2;
	struct sockaddr_in sockaddr2;
	
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	printf("trying: socket...\n");
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		temp = WSAGetLastError();
		printf("Socket Error: %x(%d)\n", temp, temp);
		assert(sock != INVALID_SOCKET);
	}
	
	memset(&timeout, 0, sizeof(struct timeval));
	timeout.tv_sec = 1000;
	timeout.tv_usec = 0;
	
	printf("trying: setsockopt...\n");
	if ((temp = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(struct timeval))) == SOCKET_ERROR)
	{
		temp = WSAGetLastError();
		printf("Socket Error: %x(%d)\n", temp, temp);
		
		if ((temp = closesocket(sock)) == SOCKET_ERROR)
		{
			temp = WSAGetLastError();
			printf("Socket Error: %x(%d)\n", temp, temp);
			assert(temp != SOCKET_ERROR);
		}
		
		assert(temp != SOCKET_ERROR);
	}
	
	memset(&sockaddr1, 0, sizeof(struct sockaddr_in));
	socklen1 = sizeof(struct sockaddr_in);
	sockaddr1.sin_family = AF_INET;
	sockaddr1.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sockaddr1.sin_port = htons(PORT);
	
	printf("trying: bind [timeout = %d]...\n", timeout.tv_sec);
	if ((temp = bind(sock, (struct sockaddr *)&sockaddr1, sizeof(struct sockaddr_in))) == SOCKET_ERROR)
	{
		temp = WSAGetLastError();
		printf("Socket Error: %x(%d)\n", temp, temp);
		
		if ((temp = closesocket(sock)) == SOCKET_ERROR)
		{
			temp = WSAGetLastError();
			printf("Socket Error: %x(%d)\n", temp, temp);
			assert(temp != SOCKET_ERROR);
		}
		
		assert(temp != SOCKET_ERROR);
	}
	
	memset(&buffer, 0, BUFFER_SIZE);
	memset(&sockaddr2, 0, sizeof(struct sockaddr_in));
	socklen2 = sizeof(struct sockaddr_in);
	if ((bufferSize = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sockaddr2, (int *)&socklen2)) < 0)
	{
		temp = WSAGetLastError();
		printf("Socket Error: %x(%d)\n", temp, temp);
		
		if ((temp = closesocket(sock)) == SOCKET_ERROR)
		{
			temp = WSAGetLastError();
			printf("Socket Error: %x(%d)\n", temp, temp);
			assert(temp != SOCKET_ERROR);
		}
		
		assert(temp != SOCKET_ERROR);
	}
	
	if ((temp = closesocket(sock)) == SOCKET_ERROR)
	{
		temp = WSAGetLastError();
		printf("Socket Error: %x(%d)\n", temp, temp);
		assert(temp != SOCKET_ERROR);
	}
		
	assert(temp != SOCKET_ERROR);
		
	printf("message is : %s\n", buffer);
	printf("size is: %d byte(s)\n", bufferSize);
	printf("from: %d.%d.%d.%d\n", sockaddr2.sin_addr.S_un.S_un_b.s_b1, sockaddr2.sin_addr.S_un.S_un_b.s_b2, sockaddr2.sin_addr.S_un.S_un_b.s_b3, sockaddr2.sin_addr.S_un.S_un_b.s_b4);
	
	WSACleanup();
	
	printf("END");
	
	return 0;
}

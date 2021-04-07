#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <assert.h>
#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 65429

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[])
{
	int temp;
	char message[] = "Hello, World!";
	
	WSADATA wsaData;
	SOCKET sock;
	int fBroadcast = 1;
	struct sockaddr_in sockaddr1;
	struct sockaddr_in sockaddr2;
	
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	printf("trying: socket...\n");
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		temp = WSAGetLastError();
		printf("Socket Error: %x(%d)\n", temp, temp);
		assert(sock != INVALID_SOCKET);
	}
	
	printf("trying: setsockopt...\n");
	if ((temp = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char *)&fBroadcast, sizeof(fBroadcast))) == SOCKET_ERROR)
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
	sockaddr1.sin_family = AF_INET;
	sockaddr1.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
	sockaddr1.sin_port = htons(PORT);
	
	printf("trying: sendto...\n");
	if ((temp = sendto(sock, message, strlen(message), 0, (const struct sockaddr *)&sockaddr1, sizeof(struct sockaddr_in))) == SOCKET_ERROR)
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
	
	
	printf("trying: closesocket...\n");
	if ((temp = closesocket(sock)) == SOCKET_ERROR)
	{
		temp = WSAGetLastError();
		printf("Socket Error: %x(%d)\n", temp, temp);
		assert(temp != SOCKET_ERROR);
	}
		
	WSACleanup();
	
	printf("END");
	
	return 0;
}

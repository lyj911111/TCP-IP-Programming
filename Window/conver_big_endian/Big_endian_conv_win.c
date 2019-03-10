/*
	컴퓨터가 빅엔디안인지, 리틀엔디안인지 확인하는 코드.
*/

#include <stdio.h>
#include <winsock2.h>

void ErrorHandling(char * message);

int main(int argc, char * argv[])
{
	WSADATA wsaData;
	unsigned short host_port = 0x1234;
	unsigned short net_port;
	unsigned long host_addr = 0x12345678;
	unsigned long net_addr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//	소켓 라이브러리 시작.
		ErrorHandling("WSAStartup error!");

	// htons(): host PC to network(big endian), short(port)
	// htonl(): long(network address)
	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("Host ordered port: %#x \n", host_port);
	printf("Network oredered port: %#x \n", net_port);
	printf("Host ordered address: %#lx \n", host_addr);
	printf("Network ordered addressL %#lx \n", net_addr);

	WSACleanup();												// 소켓라이브러리 끝

	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
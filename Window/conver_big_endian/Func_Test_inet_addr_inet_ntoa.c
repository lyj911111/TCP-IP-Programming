/*
	컴퓨터가 빅엔디안인지, 리틀엔디안인지 확인하는 코드.
*/

#include <stdio.h>
#include <string.h>
#include <winsock2.h>

void ErrorHandling(char * message);

int main(int argc, char * argv[])
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//	소켓 라이브러리 시작.
		ErrorHandling("WSAStartup error!");

	{
		char * addr = "127.212.124.78";
		unsigned long conv_addr = inet_addr(addr);
		if (conv_addr == INADDR_NONE)
			printf("error occurred");
		else
			printf("Network ordered integer addr : %#ls \n", conv_addr);
	}
	printf("hello");
	{
		struct sockaddr_in addr;
		char* strPtr;
		char strArr[20];

		addr.sin_addr.s_addr = htonl(0x1020304);
		strPtr = inet_ntoa(addr.sin_addr);
		strcpy(strArr, strPtr);
		printf("Dotted-Decimal notation3 : %s \n", strArr);

	}

	WSACleanup();												// 소켓라이브러리 끝

	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
/*
	Connected UDP 소켓 기반으로 입출력.

*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#define BUF_SIZE 30

void ErrorHandling(char * message);

int main(int argc, char * argv[])
{
	WSADATA wsaData;
	SOCKET Sock;
	char message[BUF_SIZE];
	int strLen;


	SOCKADDR_IN servAddr;
	// Parsing 인자가 3개 이상되면 다시 실행
	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT>\n", argv[0]);	// argv[0] : exe 파일명		
		exit(1);
	}

	// 소켓 라이브러리를 사용하기 시작.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() Error");

	// 소켓을 받음.
	Sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (Sock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// Sever와 연결을 시도 ( connect() )
	connect(Sock, (SOCKADDR *)&servAddr, sizeof(servAddr));

	while (1)
	{
		fputs("Insert message (q or Q : quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
			break;

		send(Sock, message, strlen(message), 0);
		strLen = recv(Sock, message, sizeof(message)-1, 0);
		message[strLen] = 0;
		printf("Message from server : %s", message);
	}

	// 소켓을 닫아줌. WSA를 clean up함.
	closesocket(Sock);
	WSACleanup();

	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

/*
	Connected UDP ���� ������� �����.

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
	// Parsing ���ڰ� 3�� �̻�Ǹ� �ٽ� ����
	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT>\n", argv[0]);	// argv[0] : exe ���ϸ�		
		exit(1);
	}

	// ���� ���̺귯���� ����ϱ� ����.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() Error");

	// ������ ����.
	Sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (Sock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// Sever�� ������ �õ� ( connect() )
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

	// ������ �ݾ���. WSA�� clean up��.
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

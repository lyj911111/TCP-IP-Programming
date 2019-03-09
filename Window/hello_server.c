/*
	윈도우 기반 소켓프로그래밍.
	Visual studio 2018에서 세팅.

	프로젝트(P) - <프로젝트이름>속성 - 구성속성 - 링커 - 입력 - 추가종속성 - ws2_32.lib 를 추가

*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>		// <= 윈도우에서만 include (window socket)

void ErrorHandling(char * message);

int main(int argc, int * argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	printf("server code start!\n");

	int szClntAddr;
	char message[] = "Hello World!";
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// WSAStartup() 윈도우에서 소켓프로그램 시작을 알림.
		ErrorHandling("WSAStartup() Error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);		//	socket() 소켓생성, SOCKET 자료형에 handler를 리턴 (정수형태로 descripter랑 비슷)
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("WSAStartup() Error!");

	printf("socket was done!\n");

	// 인자로 받을 값 초기화
	memset(&servAddr, 0, sizeof(servAddr));				// 주소값을 저장할 공간을 미리 깨끗하게 치움
	servAddr.sin_family = AF_INET;						// family지정
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);		// ip조소
	servAddr.sin_port = htons(atoi(argv[1]));			// Parsing으로 2번째 인자를 전달받아서 port번호를 저장.

	// bind() IP주소를 바인딩함.
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() Error!");

	printf("bind was done\n");

	// listen() 응답을 받을 수 있는 상태가 됨.
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() Error!");

	printf("listen was done!\n");
	printf("waiting for accept from client ...\n");

	// accept() 함수로 데이터가 올때까지 계속 기다리기, 오면 client socket으로 리턴
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() Error!");
	
	printf("accept was done!\n");

	// 받은 client정보에 message를 전달함.
	send(hClntSock, message, sizeof(message), 0);
	closesocket(hClntSock);								//	client 소켓을 닫고
	closesocket(hServSock);								//	server 소켓을 닫음.
	WSACleanup();										// 윈도우에서는 반드시 WSA startup 했으면, cleanup 시켜줘야 함.

	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
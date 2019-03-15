/*
	윈도우 기반 소켓프로그래밍.
	Visual studio 에서 세팅.
	프로젝트(P) - <프로젝트이름>속성 - 구성속성 - 링커 - 입력 - 추가종속성 - ws2_32.lib 를 추가


	TCP기반 Half-Close (소켓의 우아한 종료)

	Server                    client
	  |                         |
	  |<------연결 요청----------|
	  |                         |
	  |------파일 데이터 전송---->|  서버가 전송할 byte의 크기를 client가 모르므로 전송을 자 보내고, EOF를 보냄.
	  |------파일 데이터 전송---->|
	  |------파일 데이터 전송---->|
	  |------파일 데이터 전송---->|
	  |                          |
	  |------EOF(전송완료)------>|  다 보냈다고 알림
	  |                         |
	  |<------Thank you---------|

	  종료의 과정에서,
	  * close() 함수는 buffer로 보내는 입출력의 stream을 아예 닫아버리는 것으로 이후 read, write 불가.
	  * shutdown() 함수는 입력 stream, 출력 stream을 지정해서 닫을 수 있음.

	  int shutdown(SOCKET socket,  int howto);

	  1 param : 소켓의 descripter 전달
	  2 param : 종료 방법에 대한 정보 전달.
				 - SD_RECEIVE: 입력 스트림 종료
				 - SD_SEND   : 출력 스트림 종료
				 - SD_BOTH   : 입출력 스트림 종료
	  3 return: 성공 0, 실패 -1

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>		// <= 윈도우에서만 include (window socket)

#define BUF_SIZE 30

void ErrorHandling(char * message);

int main(int argc, int * argv[])
{
	WSADATA wsaData;
	SOCKET hservSock, hClntSock;
	FILE * fp;
	char buf[BUF_SIZE];
	int readCnt;

	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSz;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// WSAStartup() 윈도우에서 소켓프로그램 시작을 알림.
		ErrorHandling("WSAStartup() Error!");

	fp = fopen("file_server_win.c", "rb");
	hservSock = socket(PF_INET, SOCK_STREAM, 0);		//	socket() 소켓생성, SOCKET 자료형에 handler를 리턴 (정수형태로 descripter랑 비슷)
	
	// 인자로 받을 값 초기화
	memset(&servAddr, 0, sizeof(servAddr));				// 주소값을 저장할 공간을 미리 깨끗하게 치움
	servAddr.sin_family = AF_INET;						// family지정
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);		// ip조소
	servAddr.sin_port = htons(atoi(argv[1]));			// Parsing으로 2번째 인자를 전달받아서 port번호를 저장.

	// bind() IP주소를 바인딩함.
	bind(hservSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hservSock, 5);

	clntAdrSz = sizeof(clntAddr);
	hClntSock = accept(hservSock, (SOCKADDR*)&clntAddr, &clntAdrSz);

	while (1)
	{
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE)
		{
			send(hClntSock, (char*)&buf, readCnt, 0);
			break;
		}
		send(hClntSock, (char*)&buf, BUF_SIZE, 0);
	}

	shutdown(hClntSock, SD_SEND);
	recv(hClntSock, (char*)buf, BUF_SIZE, 0);
	printf("Message from the Client: %s \n", buf);

	fclose(fp);
	closesocket(hservSock);								//	client 소켓을 닫고
	WSACleanup();										// 윈도우에서는 반드시 WSA startup 했으면, cleanup 시켜줘야 함.
	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
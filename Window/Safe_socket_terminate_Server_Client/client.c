/*
	Connected UDP 소켓 기반으로 입출력.


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
#include <winsock2.h>
#include <WS2tcpip.h>

#define BUF_SIZE 30

void ErrorHandling(char * message);

int main(int argc, char * argv[])
{
	WSADATA wsaData;
	SOCKET Sock;
	FILE * fp;

	char buf[BUF_SIZE];
	int readCnt;
	SOCKADDR_IN servAddr;
	
	// Parsing 인자가 3개 아니면 다시 실행
	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT>\n", argv[0]);	// argv[0] : exe 파일명		
		exit(1);
	}

	// 소켓 라이브러리를 사용하기 시작.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() Error");
	
	fp = fopen("receive.dat", "wb");
	// 소켓을 받음.
	Sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// Sever와 연결을 시도 ( connect() )
	connect(Sock, (SOCKADDR *)&servAddr, sizeof(servAddr));

	while ((readCnt = recv(Sock, buf, BUF_SIZE, 0)) != 0)
		fwrite((void*)buf, 1, readCnt, fp);

	puts("Receive file data");
	send(Sock, "Thank you! it was done.", 10, 0);

	fclose(fp);				// 파일을 닫아줌
	closesocket(Sock);		// 소켓을 닫아줌. WSA를 clean up함.
	WSACleanup();

	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

/*
	반복 서버코딩 방법.
	socket() -> bind() -> listen() -> accept() -> write() or read() -> close(client) -> close(server)
									↑                                                ↓
									-<-------------------------<-------------------<--(while)
	5번만 서비스를 제공하는 서버코드 작성.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024	// 값을 쓸 버퍼사이즈 할당

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int serv_sock;		// 서버 소켓 handler
	int clnt_sock;		// 클라이언트 소켓 hander

	char message[BUF_SIZE];		//	메세지 버퍼할당.
	int str_len, i;

	struct sockaddr_in serv_addr;		//	서버 주소값 저장할 구조체.
	struct sockaddr_in clnt_addr;		//	클라이언트 주소값 저장할 구조체.
	socklen_t clnt_addr_size;			//	클라이언트 주소 사이즈

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// 서버 소켓 descripter(handler)할당, IPv4 , TCP 사용
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));			//	서버의 주소를 저장하는 곳에 깨끗하게 0으로 clear하는 작업
	serv_addr.sin_family = AF_INET;						//	type결정
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);		//	PC host 주소를 가져와서 Big Endian으로 변형(네트워크 표준 순서)
	serv_addr.sin_port = htons(atoi(argv[1]));			//	파씽 인자 포트번호 문자열을 int로 바꾸고 Big Endian으로 변형

	// bind(서버소켓 hander, 위에 선언 family,IP,Port정보,  구조체 크기) 
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	// listen(서버소켓 hander, 대기열 QUEUE 사이즈 (클라이언트 소켓 5개 까지 대기 가능))
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	clnt_addr_size = sizeof(clnt_addr);
	
	for(i = 0; i < 5; i++)
	{
		// 클라이언트에서 connect를 하면, accept(), 클라이언트 소켓 hander 할당
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected with Client %d \n", i+1);
		
		// read(클라리언트소켓 hander, 메세지, 메세지크기) 문자열로 크기로 리턴
		while( (str_len = read(clnt_sock, message, BUF_SIZE)) != 0 )
			write(clnt_sock, message, str_len);			//	읽은 메세지를 다시 클라이언트로 씀. (에코)
	
	}
	close(clnt_sock);		//	운영체제에게 사용종료를 알림.
	close(serv_sock);		//	서버와 클라이언트 소켓을 종료시킴.

	return 0;

}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
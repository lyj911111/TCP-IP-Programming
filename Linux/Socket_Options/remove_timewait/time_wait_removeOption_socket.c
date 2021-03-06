/*
	time wait란, 만약 host A 와 host B가 있는데,
	A가 B에게 종료를 요청하면, A는 time wait를 갖음.
	FIN과 ACK를 주고 받으면서 A의 마지막에 FIN을 받고
	B에게 ACK를 날리는데, 그 사이에 네트워크문제로 데이터가 소멸되면
	다시 보내야 함으로 일정시간 time wait를 갖음.
	
	OS가 알아서 할당하며, ACK를 다시 보내면, time wait 시간은 연장됨.
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int serv_sock;		// 서버 소켓 handler
	int clnt_sock;		// 클라이언트 소켓 hander
	
	int option, str_len; // 소켓 옵션을 정함.

	char message[30];		//	메세지 버퍼할당.

	struct sockaddr_in serv_addr;		//	서버 주소값 저장할 구조체.
	struct sockaddr_in clnt_addr;		//	클라이언트 주소값 저장할 구조체.
	socklen_t clnt_addr_size, opt_len;	//	클라이언트 주소 사이즈, 옵션을 담을 길이 사이즈

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// 서버 소켓 descripter(handler)할당, IPv4 , TCP 사용
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

//////////////////////////////// 옵션 정보 ////////////////////////////////////////

	// TRUE 일때 옵션 활성화.
	// SO_REUSEADDR 는 time-wait를 사용하지 않겠다는 옵션.

	opt_len = sizeof(option);
	option = TRUE;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, opt_len);

//////////////////////////////// 옵션 정보 ////////////////////////////////////////

	memset(&serv_addr, 0, sizeof(serv_addr));           //	서버의 주소를 저장하는 곳에 깨끗하게 0으로 clear하는 작업
	serv_addr.sin_family = AF_INET;                     //	type결정
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);      //	PC host 주소를 가져와서 Big Endian으로 변형(네트워크 표준 순서)
	serv_addr.sin_port = htons(atoi(argv[1]));          //	파씽 인자 포트번호 문자열을 int로 바꾸고 Big Endian으로 변형

	// bind(서버소켓 hander, 위에 선언 family,IP,Port정보,  구조체 크기) 
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	// listen(서버소켓 hander, 대기열 QUEUE 사이즈 (클라이언트 소켓 5개 까지 대기 가능))
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	clnt_addr_size = sizeof(clnt_addr);
	
	for(int i = 0; i < 5; i++)
	{
		// 클라이언트에서 connect를 하면, accept(), 클라이언트 소켓 hander 할당
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected with Client %d \n", i+1);
		
		// read(클라리언트소켓 hander, 메세지, 메세지크기) 문자열로 크기로 리턴
		while( (str_len = read(clnt_sock, message, sizeof(message))) != 0 )
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
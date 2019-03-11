/*
	서버코딩 방법.
	socket() -> bind() -> listen() -> accept() -> write() or read()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;		//	소켓을 생성한 일련번호를 저장.
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[] = "Hello World";

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	/*
		socket() 함수 설명.
		return 값  : Descripter 번호 반환
		1번째 param : - PF_INET  : IPv4 사용
					 - PF_INET6 : IPv6 사용
					 
		2번? param : - SOCK_STREAM : TCP 방식사용, (데이터 소멸이 없음(소멸시 재요청), 전송 순서데로 감, 1:1으로만 통신, Boundary가 존재하지 않음)
					 - SOCK_DGRAM  : UDP 방식사용, (속도가 빠름, 파손의 우려, 크기제한, Boundary가 존재(ex)write가 3개면, read도 3개 필요))
		
		3번째 param : protocol 방식, 0으로 해도 됨. (한개방식 밖에 없기 때문)
	*/
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	/*
		임터넷 주소의 초기화과정.
		1. memset()				: &serv_addr 구조체 내부를 모두 0으로 초기화과정
		2. serv_addr.sin_family	: 사용할 타입, IPv4 으로 지정.
		3. serv_addr.sin_addr.s_addr	: INADDR_ANY <= 컴퓨터 서버의 주소값을 찾아서 스스로 할당함. (서버만 해당)
		4. serv_addr.sin_port			: 문자열 포트를 atoi 함수로 int로 만들어서 htons 으로 빅엔디안 형태로 보냄.
	*/
	memset(&serv_addr, 0, sizeof(serv_addr));			//	서버의 주소를 저장하는 곳에 깨끗하게 0으로 clear하는 작업
	serv_addr.sin_family = AF_INET;						//	주소, IP 등을 초기화
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	
	/*
		bind(Descripter, IP와 Port의 주소값정보, 2번째 인자의 크기 sizeof)
		=> 성공시 0반환, 실패시 -1반환
		
		맨 위에서 선언한 serv_addr 의 구조체:						sockaddr 의 구조체:
		
		struct sockaddr_in serv_addr						struct sockaddr 
		{													{
			sa_family_t		sin_family;		- n byte	=>		sa_family_t		sin_family;	 - n byte
			uint16_t		sin_port;		- 2 byte +	=>		char			sa_data[14]; - 14 byte
			struct in_addr	sin_addr;		- 4 byte +
			char			sin_zero[8];	- 8 byte +
		}													}
			이렇게, 크기를 구조체의 크기를 맞춰서 casting 하기 위해 아래와 같이 zero[8] 처럼 0으로 채운 인자가 필요.
			(2+4+8 = 14 byte)      ↓ bind() 2번? 인자가 casting함.
	*/
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	// serv_sock 으로 할당된 주소로 인터넷에 연결하라 (listen 들을 수 있는 상태로 둠)
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	// accept()함수, 나한테 걸려온 전화가 있는지 운영체제에게 확인하는 과정. 연결이 올때까지 계속 기다리다가 전화가 오면 그때 반환함.
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");

	/*
		ssize_t  write(파일 Descripter, 전송할 데이터를 저장할 버퍼의 주소값, 전송할 바이트 크기);
		
		1번째 param : - 소켓의 Descripter(Handler) 번호를 인자로 넣음
		2번째 param : - 보낼 buffer의 주소값을 전달.
		3번째 param : - 한번 당 보낼 데이터 크기
		return값   : - 정상적으로 수행 되었을 때 보낸 byte 수를 리턴.
		
	*/
	write(clnt_sock, message, sizeof(message));
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
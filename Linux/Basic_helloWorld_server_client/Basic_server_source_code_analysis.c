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

	// 소켓을 생성함. (create socket) - 소켓을 지칭하는 숫자가 반환됨.
	// 생성한 소켓의 일련번호를 int형으로 serv_sock 변수에 저장함.
	// 이를 descripter 라고 부르며, 소켓을 핸들링하는 일련번호.
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));			//	서버의 주소를 저장하는 곳에 깨끗하게 0으로 clear하는 작업
	serv_addr.sin_family = AF_INET;						//	주소, IP 등을 초기화
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	// bind함수를 호출, serv_sock에 할당된 숫자에, &serv_addr 주소정보를 할당하라
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

	// write()함수를 이용해서 데이터를 쓸 수 있음.
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
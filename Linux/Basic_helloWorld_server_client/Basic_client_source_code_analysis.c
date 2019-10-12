/*

	연결을 요청하는 소켓.
	과정 : socket() -> connect() 
	_
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
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// 소켓을 생성해서 descripter 의 int를 반환해서 sock 에 넣음.
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	// 연결시킬 상대방의 주소를 초기화 함.
	memset(&serv_addr, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	// connect(), &serv_addr에 저장된 주소에 연결하라
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	// read() 서버로부터 데이터를 읽음. (서버가 write를 했기 때문에 read할 수 있음. 서버가 read하면 서로 아무것도 못함)
	str_len = read(sock, message, sizeof(message)-1);
	if (str_len == -1)
		error_handling("read() error");

	printf("Message from server : %s \n", message);
	close(sock);
	return 0;

}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
/*
	에코 클라이언트 입출력 프로세스 분할
	기존 에코에서는 하나의 프로세스에서 입출력을 함으로 write()를 하고, read()가 올때까지 무한정으로 기다리기만함.

	그러나 프로세스를 2개로 나뉘고 (연결된 소켓을 하나)
	1개 프로세스는 write()만 실행하여 계속 입력된 데이터를 전송.
	1개 프로세스는 read()만 실행하여 계속 데이터를 받아서 출력.

	서로 기다릴 필요없고 여러개를 보내도 여러개를 빠르게 받을 수 있음.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char * message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char * argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_addr;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// 				IPv4, TCP 사용
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	// 연결시킬 서버의 주소를 초기화 함. 주소 1번 파씽, 포트 2번 파씽
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	// connect(소켓 handler, 위에서 정한 IP PORT 정보, 구조체 크기) 
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	else
		puts("Connected ................. ");

	// 자식 프로세스 복제. (자식은 pid 0값 리턴, 부모는 본래 pid값 리턴(0이 아닌 다른수))
	pid = fork();

	if (pid == 0)					// 자식 프로세스는 계속 write()함수만 실행.
		write_routine(sock, buf);
	else							// 부모 프로세스는 계속 read()함수만 실행.
		read_routine(sock, buf);

	close(sock);
	return 0;

}

// 소켓정보와 버퍼의 주소값을 받음.
void read_routine(int sock, char *buf)
{
	while (1)
	{
		int str_len = read(sock, buf, BUF_SIZE);
		if (str_len == 0)
			return 0;

		buf[str_len] = 0;
		printf("Message from the Server: %s", buf);
	}
}

void write_routine(int sock, char *buf)
{
	while (1)
	{
		fgets(buf, BUF_SIZE, stdin);
		if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
		{
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	}
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
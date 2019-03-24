/*
	에코 클라이언트

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char * message);

int main(int argc, char * argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	int str_len;

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

	while (1)
	{
		fputs("Input message(Q or q = quit): ", stdout);			//	모니터로 출력
		fgets(message, BUF_SIZE, stdin);							//	키보드로 입력 받음
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))		// 대소문자 q가 눌릴 경우 종료
			break;

		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE-1);
		message[str_len] = 0;							//	버퍼의 끝에 NULL을 넣어 String끝을 알림
		printf("Message from server : %s \n", message);

	}
	close(sock);
	return 0;

}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
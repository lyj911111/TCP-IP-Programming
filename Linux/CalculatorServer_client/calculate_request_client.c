/*
	** 계산기능을 장착한 서버, 계산을 요청하는 클라이언트 코드 작성.
	
	프로토콜
	1. client는 server에 접속하자마자 피연산자 개수정보를 1byte형태로 전달.
	2. client가 server에 전달하는 정수는 4byte로 표현
	3. 정수를 전달한 다음, 연산의 종류를 전달한다. (정수 하나는 4byte으로 표현)
	4. 문자 +,-,* 중 하나를 전달.
	5. 서버는 연산결과를 4byte 정수형태로 client에게 전달한다.
	6. 연산결과를 얻은 client는 server와 연결을 종료한다.

	Client 코드

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int sock;
	char opmsg[BUF_SIZE];
	int result, opnd_cnt, i;
	struct sockaddr_in serv_adr;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("Socket() Error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error");
	else
		printf("connected.............");

	// 1. client는 server에 접속하자마자 피연산자 개수정보를 1byte형태로 전달. 
	fputs("Operand count : ", stdout);
	scanf("%d", &opnd_cnt);
	opmsg[0] = (char)opnd_cnt;

	for (i = 0; i < opnd_cnt; i++)
	{
		printf("Operand %d", i + 1);
		scanf("%d", (int*)&opmsg[i * OPSZ + 1]);
	}

	fgetc(stdin);
	fputs("Operator: ", stdout);
	scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);
	write(sock, opmsg, opnd_cnt * OPSZ + 2);
	read(sock, &result, RLT_SIZE);

	printf("Operation result : %d \n", result);
	close(sock);
	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
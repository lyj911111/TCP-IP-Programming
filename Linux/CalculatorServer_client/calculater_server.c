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
#define OPSZ 4

void error_handling(char * message);
int calculate(int opnum, int opnds[], char operator);

int main(int argc, char * argv[])
{
	int serv_sock, clnt_sock;
	char opinfo[BUF_SIZE];
	int result, opnd_cnt, i;
	int recv_cnt, recv_len;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);

	for (i = 0; i < 5; i++)
	{
		opnd_cnt = 0;
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		read(clnt_sock, &opnd_cnt, 1);

		recv_len = 0;
		while ((opnd_cnt * OPSZ + 1) > recv_len)
		{
			recv_cnt = read(clnt_sock, &opnd_cnt[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}
		result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);
		write(clnt_sock, (char*)&result, sizeof(result));
		close(serv_sock);
	}

	close(sock);
	return 0;
}

int calculate(int opnum, int opnds[], char operator)
{
	int result = opnds[0], i;
	switch (op)
	{
	case '+':
		for (i = 0; i < opnum; i++)
			result += opnds[i];
	case '-':
		for (i = 0; i < opnum; i++)
			result -= opnds[i];
	case '*':
		for (i = 0; i < opnum; i++)
			result *= opnds[i];

		break;
	}

	return result;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
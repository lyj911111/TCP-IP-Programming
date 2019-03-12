/*
	** 계산기능을 장착한 서버, 계산을 요청하는 클라이언트 코드 작성.
	
	프로토콜
	1. client는 server에 접속하자마자 피연산자 개수정보를 1byte형태로 전달.
	2. client가 server에 전달하는 정수는 4byte로 표현
	3. 정수를 전달한 다음, 연산의 종류를 전달한다. (정수 하나는 4byte으로 표현)
	4. 문자 +,-,* 중 하나를 전달.
	5. 서버는 연산결과를 4byte 정수형태로 client에게 전달한다.
	6. 연산결과를 얻은 client는 server와 연결을 종료한다.

		프로토콜 예시
	ex)  -> 3개의 피연산자 덧셈송신
		[3][ , , , ][ , , , ][ , , , ][+] 
		1byte /    4byte 3개    /  연산자 1byte 
		
		 -> 2개의 피연산자 뺄셈송신
		[2][ , , , ][ , , , ][-]

	tip.
		서버에는 위와 같은 버퍼가 다 찬 배열이 client으로 부터 날라옴.
		이를 배열을 벗겨내며 연산을 해서 결과값을 다시 client에게 write함.

	Server 코드

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024		//	송수신할 버퍼의 사이즈
#define OPSZ 4				//	피연산자 사이즈를 4byte로 정함.

void error_handling(char * message);
int calculate(int opnum, int opnds[], char operator);		// 계산을 수행할 함수.

int main(int argc, char * argv[])
{
	int serv_sock, clnt_sock;
	char opinfo[BUF_SIZE];						// 데이터 송수신을 위한 버퍼사이즈 배열
	int result, opnd_cnt, i;
	int recv_cnt, recv_len;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error!");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);		// client 소켓 사이즈

	// 5명의 client로 제한
	for (i = 0; i < 5; i++)
	{
		opnd_cnt = 0;
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);	// 서버소켓과 클라이언트 소켓 연결
		read(clnt_sock, &opnd_cnt, 1);												// 클라이언트로 맨처음 1byte(피연산자 갯수)를 읽음 (몇개의 피연산자가 들어왔는지 파씽된 데이터) for문을 위해?

		recv_len = 0;																// ex) 파씽 데이터가 3일때 예시             (13)
		while ((opnd_cnt * OPSZ + 1) > recv_len)									// 파씽된 피연산자 갯수만큼 횟수를 돌게함. ex) (3x4+1) > recv_len
		{																			//                                                                 
			recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);			// opnd_cnt가 3일때 예시, 배열값: &opinfo[1] &opinfo[5] &opinfo[9]  => [3][ , ,3, ][ , ,3, ][ , ,3, ][+]
			recv_len += recv_cnt;													// recv_cnt 0 4 4 4 
		}																			// recv_len 1 4 8 12
		result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);
		write(clnt_sock, (char*)&result, sizeof(result));
		close(serv_sock);
	}

	close(serv_sock);
	return 0;
}

/*
	계산 수행 함수.
	1st param: 몇개의 피연산자가 있는지 대입. (몇개의 인자를 넣을지 결정)
	2nd param: 4 byte씩 읽는 피연산자(위 예시에서 3개)를 인자로 넣음.
	3rd param: char 형 1byte 연산자(+,-,*)
*/
int calculate(int opnum, int opnds[], char op)
{
	int result = opnds[0], i;
	switch (op)
	{
	case '+':
		for (i = 1; i < opnum; i++)
			result += opnds[i];			//	
		break;
	case '-':
		for (i = 1; i < opnum; i++)
			result -= opnds[i];
		break;
	case '*':
		for (i = 1; i < opnum; i++)
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
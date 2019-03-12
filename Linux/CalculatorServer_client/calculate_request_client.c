/*
	** 계산기능을 장착한 서버, 계산을 요청하는 클라이언트 코드 작성.
	
	프로토콜 조건
	1. client는 server에 접속하자마자 피연산자 개수정보를 1byte형태로 전달.
	2. client가 server에 전달하는 정수는 4byte로 표현
	3. 정수를 전달한 다음, 연산의 종류를 전달한다. (정수 하나는 4byte으로 표현)
	4. 문자 +,-,* 중 하나를 전달.
	5. 서버는 연산결과를 4byte 정수형태로 client에게 전달한다.
	6. 연산결과를 얻은 client는 server와 연결을 종료한다.
	
	프로토콜 예시
	ex)  -> 3개의 피연산자 덧셈송신
		[3][ , , , ][ , , , ][ , , , ][+] 
		1byte /     4byte 3개    /  연산자 1byte 
		
		 -> 2개의 피연산자 뺄셈송신
		[2][ , , , ][ , , , ][-]
		1byte  /4byte 2개/  연산자 1byte
	
	tip.
		하나의 배열에 다양한 자료형을 저장하려면, 배열을 반드시 char으로 선언해야 한다. (위의 4byte는 (int*)로 캐스팅하여 전달함)
		
	Client 코드

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024		//	데이터를 송수신할 버퍼
#define RLT_SIZE 4			//	Result size, 프로토콜에 의한 연산결과물 4byte 정수를 위한 사이즈.
#define OPSZ 4				//	Operand Size, 프로토콜에 의한 피연산자 4byte 정수를 위한 사이즈.

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int sock;
	char opmsg[BUF_SIZE];			// 실질적으로 데이터 송수신이 되는 메모리 배열. opmsg[1024 크기]
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
	fputs("Operand count : ", stdout);	// 모니터에 출력
	scanf("%d", &opnd_cnt);				// 사용자의 정수 입력을 받아, opnd_cnt(int형)에 저장.
	opmsg[0] = (char)opnd_cnt;			// 입력 받은 정수 int를 char으로 casting해서 송신 배열 첫번째 1byte로 저장.

	// 2. client가 server에 전달하는 정수는 4byte로 표현
	// 3. 정수를 전달한 다음, 연산의 종류를 전달한다. (정수 하나는 4byte으로 표현)
	for (i = 0; i < opnd_cnt; i++)					// 위에서 전달받은 정수 만큼 for문 반복동작.
	{
		printf("Operand %d : ", i + 1);				// opmsg[0]바이트는 이미 채워졌고, 그 뒤로 4 byte씩 파씽받은 데이터 할당.
		scanf("%d", (int*)&opmsg[i * OPSZ + 1]);	// opmsg[0x4+1], opmsg[1x4+1], opmsg[2x4+1]...  , char형 배열에 저장해야 함으로 (int*)으로 casting하여 전달
	}												// opmsg[1]      opmsg[5]    , opmsg[9]...

	// 4. 문자 +,-,* 중 하나를 전달.
	// 5. 서버는 연산결과를 4byte 정수형태로 client에게 전달한다.
	// 6. 연산결과를 얻은 client는 server와 연결을 종료한다.
	fgetc(stdin);									// 문자를 입력받아야 하는데, 앞서 버퍼에 남아있는 \n문자의 삭제를 위해 fgetc 함수 호출.
	fputs("Operator: ", stdout);					// 화면출력
	scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);		// (+,-,*)중 하나의 character 인자를 입력 받겠다. ex) opnd_cnt = 3일때, &opmsg[3 x 4 + 1] = opmsg[13], 여기에 연산자를 넣음.
	write(sock, opmsg, opnd_cnt * OPSZ + 2);		// 꽉채운 버퍼에 마지막 NULL 까지 포함하여 전송. (데이터의 경계를 만듦)
	
	read(sock, &result, RLT_SIZE);					// 서버로 부터 받은 데이터를 4byte크기로 읽어서 &result 주소에 저장.
	

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
/*

	연결을 요청하는 소켓. (문제를 해결한 코드)
	* 데이터 전송이 다될때까지 읽음.
	과정 : socket() -> connect()

	서버와 1:1 이나, 자원이 넘치는 경우는 괜찮지만,
	TCP는 데이터경계가 존재하지 않아, 전송데이터가 커지면 OS가 자동으로 데이터를 잘라서 전송.
	그러면 client는 데이터가 다 전송되지 않았음에도 불구하고 write를 실행 할 수도 있다.
	그래서 client는 데이터가 다 왔는지 확인하고, 그렇지 않으면 다시 read를 수행해야 한다.

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
	int recv_len;
	int recv_cnt;

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

		/*
			1 param : sock의 descripter(handler)
			2 param : Buffer의 주소값
			3 param : 보낸 총 buffer 크기
			4 return : 전송한 buffer 길이 
		*/
		str_len = write(sock, message, strlen(message));			//	message를 작성해서 총 메세지 buffer의 길이 정보 반환

		recv_len = 0;
		while (recv_len < str_len)  // (몇개 보내졌는지 셈 길이 < 내가 보낸 메세지길이), 내가 보낸 길이가 될 때까지 byte 읽기를 반복
		{
			recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);	// sock descripter 전달 // &message[0] -> &message[1] -> &message[2] ... 이렇게 1개의 byte씩 읽어서 1을 리턴 // 한번에 최대 1023개까지 읽음.
			if (recv_cnt == -1)
				error_handling("read() Error!");
			recv_len += recv_cnt;							//	1byte를 리턴한 것을 recv_len 에 계속 더하여 몇개 보내졌는지 갯수를 셈.
			
		}
		message[str_len] = NULL;							//	버퍼의 끝에 NULL을 넣어 String끝을 알림
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
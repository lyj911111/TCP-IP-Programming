/*
		Server에서 write 함수를 쓰고, client가 read하여 buffer를 읽고,
		그 byte index의 길이를 하나씩 읽어보는 보기. 다 읽고 NULL을 만나면 while 종료 테스트 코드
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
	int str_len = 0;
	int idx = 0, read_len = 0;		//	read한 데이터의 byte를 읽기 위한 테스트 !!

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
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

	/*
		Server에서 write 함수를, client가 read하여 buffer를 읽고,
		그 byte index의 길이를 하나씩 읽어보는 보기. 다 읽고 데이터가 없어서 NULL을 만나면 while 종료.
	*/
	while( read_len = read(sock, &message[idx++], 1 ))
	{
		if ( read_len == -1)
			error_handling("read() error");
		
		str_len = read_len++;
	}

	printf("Message from server : %s \n", message);
	printf("Function read() call count : %d\n", str_len);
	close(sock);
	return 0;

}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
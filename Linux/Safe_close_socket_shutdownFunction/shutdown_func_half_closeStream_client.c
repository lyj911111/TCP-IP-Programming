/*
	TCP기반 Half-Close (소켓의 우아한 종료)

	Server                 client
	  |                       |
	  |<------연결 요청---------|
	  |                       |
	  |------파일 데이터 전송---->|  서버가 전송할 byte의 크기를 client가 모르므로 전송을 자 보내고, EOF를 보냄.
	  |------파일 데이터 전송---->|
	  |------파일 데이터 전송---->|
	  |------파일 데이터 전송---->|
	  |                      |
	  |------EOF(전송완료)----->|  다 보냈다고 알림
	  |                       |
	  |<------Thank you-------|

	  종료의 과정에서,
	  * close() 함수는 buffer로 보내는 입출력의 stream을 아예 닫아버리는 것으로 이후 read, write 불가.
	  * shutdown() 함수는 입력 stream, 출력 stream을 지정해서 닫을 수 있음.

	  int shutdown(int socket,  int howto);

	  1 param : 소켓의 descripter 전달
	  2 param : 종료 방법에 대한 정보 전달.
				 - SHUT_RD   : 입력 스트림 종료
				 - SHUT_WR   : 출력 스트림 종료
				 - SHUT_RDWR : 입출력 스트림 종료
	  3 return: 성공 0, 실패 -1

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char * message);

int main(int argc, char* argv[])
{
	int sd;
	FILE * fp;
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	fp = fopen("receive.dat", "wb");
	sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)		// EOF가 전달되기 전까지 계속해서 read를 함.
		fwrite((void*)buf, 1, read_cnt, fp);				// EOF가 전달되면 그 buf의 data를 파일에 씀.

	puts("Receive file data");
	write(sd, "Thank you", 10);		//	EOF를 받고 나서 마지막으로 서버에게 Thank you를 보내고
	close(fp);						//	파일도 닫고, 소켓도 닫고 종료
	close(sd);
	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
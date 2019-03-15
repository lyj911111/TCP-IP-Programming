/*
	TCP기반 Half-Close (소켓의 우아한 종료)
	
	Server                    client
	  |                         |
	  |<------연결 요청-----------|
	  |                         |
	  |------파일 데이터 전송------>|  서버가 전송할 byte의 크기를 client가 모르므로 전송을 자 보내고, EOF를 보냄.
	  |------파일 데이터 전송------>|
	  |------파일 데이터 전송------>|
	  |------파일 데이터 전송------>|
	  |                         |
	  |------EOF(전송완료)------->|  다 보냈다고 알림
	  |                         |
	  |<------Thank you---------|
	  
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
	int serv_sd, clnt_sd;
	FILE * fp;
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if (argc != 2)
	{
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	fp = fopen("file_server.c", "rb");
	serv_sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

	while (1)
	{
		/*
			fread()함수.
			-> stream에서 count의 원소를 가지는 배열을 읽어온다. 이때 각 원소의 크기는 size 바이트이고,
			   ptr이 가리키는 배열에 넣게 된다. (이때, count 와 size, ptr 은 모두 fread의 각 인자를 의미)
			
			size_t  fread(void * ptr, size_t  size, size_t  count,  FILE * stream)
			
			1 param : ptr이 가리키고 있는 곳에 배열을 넣음.
			2 param : 사이즈를 읽어들일 byte크기
			3 param : sizeof 버퍼(size)
			4 param : stream데이터를 입력 받을 FILE 포인터.
			5 return: 성공적으로 지정한 개수만큼 읽었다면 그 갯수를 리턴. (정수타입임), 읽어들인 개수가 count에서 지정한 것과 다르거나 EOF이면 오류발생.
			
		*/
		
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);		// fp파일의 buf에 값을 읽어 갯수를 샘. 
		if (read_cnt < BUF_SIZE)							// BUF_SIZE 만큼의 데이터가 될때까지 읽음
		{
			write(clnt_sd, buf, read_cnt);					// client소켓에 값을 write
			break;
		}
		write(clnt_sd, buf, BUF_SIZE);
	}

	shutdown(clnt_sd, SHUT_WR);								// ** write하는 Stream을 마침 EOF를 보냄.
	read(clnt_sd, buf, BUF_SIZE);							// 그리고 client의 마지막 신호를 기다리다가 마침.
	printf("Message from client : %s \n", buf);

	fclose(fp);
	close(clnt_sd);
	close(serv_sd);

	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
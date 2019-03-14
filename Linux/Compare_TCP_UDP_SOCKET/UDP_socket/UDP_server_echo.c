/*
	UDP 으로 에코출력하는 서버.
	data의 경계가 존재함. 한번에 데이터를 보내고, 한번이 읽음.
	UDP에서의 서버와 클라이언트는 연결(connect)되어 있지 않다. socket생성, bind 후 주소값과 데이터만으로 통신.
	서버나 클라이언트나 하나의 소켓을 필요로 함.
	UDP는 1:1 연결 통신이 아닌, 여러곳으로 한번에 전송 가능.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30		//	송수신할 버퍼의 사이즈

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int serv_sock;
	char message[BUF_SIZE];						// 데이터 송수신을 위한 버퍼사이즈 배열
	int str_len;
	socklen_t clnt_adr_sz;

	struct sockaddr_in serv_adr, clnt_adr;
	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	// IPv4방식의 UDP(SOCK_DGRAM)를 사용
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("UDP socket creation error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// IP주소와 Port를 할당받기 위해 외부와 통신을 위해
	if (bind(serv_adr, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	while (1)
	{
		// client의 주소사이즈(주소정보)
		clnt_adr_sz = sizeof(clnt_adr);
		// 소켓으로부터 메세지를 읽어들임.
		/*
			ssize_t  recvfrom(int  sock, void *buff, size_t  nbytes, int  flags,  struct sockaddr *from,  socklent_t  *addrlen);
			
			1 param : UDP로 사용할 소켓의 descripter전달
			2 param : 데이터 수신에 사용할 최대 버퍼 주소값 전달
			3 param : 수신할 최대 바이트 수, (2번 매개변수 buff가 가리키는 버퍼크기를 넘길 수 없음.)
			4 param : 옵션선택, 없으면 0
			5 param : 발신자의 정보를 넣을 sockaddr 구조체변수 주소값 전달
			6 param : 5번인자 매개변수의, 주소의 구조체변수 크기 정보를 담고있는 변수의 주소값전달 (sizeof)
		*/
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
		// server소켓으로 메세지를 보냄.
		/*
			ssize_t  sendto(int  sock, void  *buff,  size_t  nbytes,  int flags, struct sockaddr  *to,  socklen_t  addrlen);
			
			1 param : UDP로 사용할 소켓의 descripter전달
			2 param : 데이터 수신에 사용할 최대 버퍼 주소값 전달
			3 param : 전송할 데이터크기를 바이트 수로 보냄
			4 param : 옵션선택, 없으면 0
			5 param : 목적지의 주소를 담고있는 sockaddr 구조체변수 주소값 전달
			6 param : 5번인자 매개변수의, 전달될 구조체변수 크기 정보 (sizeof)
		*/
		sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
	}
	close(serv_sock);

	return 0;
}


void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

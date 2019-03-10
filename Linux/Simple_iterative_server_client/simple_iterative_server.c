/*
	�ݺ� �����ڵ� ���.
	socket() -> bind() -> listen() -> accept() -> write() or read() -> close(client) -> close(server)
									��                                                ��
									-<-------------------------<-------------------<--(while)
	5���� ���񽺸� �����ϴ� �����ڵ� �ۼ�.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024	// ���� �� ���ۻ����� �Ҵ�

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int serv_sock;		// ���� ���� handler
	int clnt_sock;		// Ŭ���̾�Ʈ ���� hander

	char message[BUF_SIZE];		//	�޼��� �����Ҵ�.
	int str_len, i;

	struct sockaddr_in serv_addr;		//	���� �ּҰ� ������ ����ü.
	struct sockaddr_in clnt_addr;		//	Ŭ���̾�Ʈ �ּҰ� ������ ����ü.
	socklen_t clnt_addr_size;			//	Ŭ���̾�Ʈ �ּ� ������

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// ���� ���� descripter(handler)�Ҵ�, IPv4 , TCP ���
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));			//	������ �ּҸ� �����ϴ� ���� �����ϰ� 0���� clear�ϴ� �۾�
	serv_addr.sin_family = AF_INET;						//	type����
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);		//	PC host �ּҸ� �����ͼ� Big Endian���� ����(��Ʈ��ũ ǥ�� ����)
	serv_addr.sin_port = htons(atoi(argv[1]));			//	�ľ� ���� ��Ʈ��ȣ ���ڿ��� int�� �ٲٰ� Big Endian���� ����

	// bind(�������� hander, ���� ���� family,IP,Port����,  ����ü ũ��) 
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	// listen(�������� hander, ��⿭ QUEUE ������ (Ŭ���̾�Ʈ ���� 5�� ���� ��� ����))
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	clnt_addr_size = sizeof(clnt_addr);
	
	for(i = 0; i < 5; i++)
	{
		// Ŭ���̾�Ʈ���� connect�� �ϸ�, accept(), Ŭ���̾�Ʈ ���� hander �Ҵ�
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected with Client %d \n", i+1);
		
		// read(Ŭ�󸮾�Ʈ���� hander, �޼���, �޼���ũ��) ���ڿ��� ũ��� ����
		while( (str_len = read(clnt_sock, message, BUF_SIZE)) != 0 )
			write(clnt_sock, message, str_len);			//	���� �޼����� �ٽ� Ŭ���̾�Ʈ�� ��. (����)
	
	}
	close(clnt_sock);		//	�ü������ ������Ḧ �˸�.
	close(serv_sock);		//	������ Ŭ���̾�Ʈ ������ �����Ŵ.

	return 0;

}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
/*
	TCP��� Half-Close (������ ����� ����)

	Server                 client
	  |                       |
	  |<------���� ��û---------|
	  |                       |
	  |------���� ������ ����---->|  ������ ������ byte�� ũ�⸦ client�� �𸣹Ƿ� ������ �� ������, EOF�� ����.
	  |------���� ������ ����---->|
	  |------���� ������ ����---->|
	  |------���� ������ ����---->|
	  |                      |
	  |------EOF(���ۿϷ�)----->|  �� ���´ٰ� �˸�
	  |                       |
	  |<------Thank you-------|

	  ������ ��������,
	  * close() �Լ��� buffer�� ������ ������� stream�� �ƿ� �ݾƹ����� ������ ���� read, write �Ұ�.
	  * shutdown() �Լ��� �Է� stream, ��� stream�� �����ؼ� ���� �� ����.

	  int shutdown(int socket,  int howto);

	  1 param : ������ descripter ����
	  2 param : ���� ����� ���� ���� ����.
				 - SHUT_RD   : �Է� ��Ʈ�� ����
				 - SHUT_WR   : ��� ��Ʈ�� ����
				 - SHUT_RDWR : ����� ��Ʈ�� ����
	  3 return: ���� 0, ���� -1

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

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)		// EOF�� ���޵Ǳ� ������ ����ؼ� read�� ��.
		fwrite((void*)buf, 1, read_cnt, fp);				// EOF�� ���޵Ǹ� �� buf�� data�� ���Ͽ� ��.

	puts("Receive file data");
	write(sd, "Thank you", 10);		//	EOF�� �ް� ���� ���������� �������� Thank you�� ������
	close(fp);						//	���ϵ� �ݰ�, ���ϵ� �ݰ� ����
	close(sd);
	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
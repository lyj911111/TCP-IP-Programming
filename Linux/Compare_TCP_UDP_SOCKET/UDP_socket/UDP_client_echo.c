/*
	UDP ���� ��������ϴ� ����.
	data�� ��谡 ������. �ѹ��� �����͸� ������, �ѹ��� ����.
	UDP������ ������ Ŭ���̾�Ʈ�� ����(connect)�Ǿ� ���� �ʴ�. socket����, bind �� �ּҰ��� �����͸����� ���.
	������ Ŭ���̾�Ʈ�� �ϳ��� ������ �ʿ�� ��.
	UDP�� 1:1 ���� ����� �ƴ�, ���������� �ѹ��� ���� ����.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30		//	�ۼ����� ������ ������

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int sock;
	char message[BUF_SIZE];						// ������ �ۼ����� ���� ���ۻ����� �迭
	int str_len;
	socklen_t adr_sz;

	struct sockaddr_in serv_adr, from_adr;
	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// IPv4����� UDP(SOCK_DGRAM)�� ���
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("UDP socket creation error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	while (1)
	{
		fputs("Insert message(q or Q : quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		// server�������� �޼����� ����.
		/*
			ssize_t  sendto(int  sock, void  *buff,  size_t  nbytes,  int flags, struct sockaddr  *to,  socklen_t  addrlen);

			1 param : UDP�� ����� ������ descripter����
			2 param : ������ ���ſ� ����� �ִ� ���� �ּҰ� ����
			3 param : ������ ������ũ�⸦ ����Ʈ ���� ����
			4 param : �ɼǼ���, ������ 0
			5 param : �������� �ּҸ� ����ִ� sockaddr ����ü���� �ּҰ� ����
			6 param : 5������ �Ű�������, ���޵� ����ü���� ũ�� ���� (sizeof)
		*/
		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		adr_sz = sizeof(from_adr);

		// �������κ��� �޼����� �о����.
		/*
			ssize_t  recvfrom(int  sock, void *buff, size_t  nbytes, int  flags,  struct sockaddr *from,  socklent_t  *addrlen);

			1 param : UDP�� ����� ������ descripter����
			2 param : ������ ���ſ� ����� �ִ� ���� �ּҰ� ����
			3 param : ������ �ִ� ����Ʈ ��, (2�� �Ű����� buff�� ����Ű�� ����ũ�⸦ �ѱ� �� ����.)
			4 param : �ɼǼ���, ������ 0
			5 param : �߽����� ������ ���� sockaddr ����ü���� �ּҰ� ����
			6 param : 5������ �Ű�������, �ּ��� ����ü���� ũ�� ������ ����ִ� ������ �ּҰ����� (sizeof)
		*/
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, adr_sz);
		message[str_len] = 0;
		printf("Message from the server : %s", message);
		
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

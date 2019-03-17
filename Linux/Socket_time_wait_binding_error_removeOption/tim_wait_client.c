/*

	������ ��û�ϴ� ����. (������ �ذ��� �ڵ�)
	* ������ ������ �ٵɶ����� ����.
	���� : socket() -> connect()

	������ 1:1 �̳�, �ڿ��� ��ġ�� ���� ��������,
	TCP�� �����Ͱ�谡 �������� �ʾ�, ���۵����Ͱ� Ŀ���� OS�� �ڵ����� �����͸� �߶� ����.
	�׷��� client�� �����Ͱ� �� ���۵��� �ʾ������� �ұ��ϰ� write�� ���� �� ���� �ִ�.
	�׷��� client�� �����Ͱ� �� �Դ��� Ȯ���ϰ�, �׷��� ������ �ٽ� read�� �����ؾ� �Ѵ�.

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

	// 				IPv4, TCP ���
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	// �����ų ������ �ּҸ� �ʱ�ȭ ��. �ּ� 1�� �ľ�, ��Ʈ 2�� �ľ�
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	// connect(���� handler, ������ ���� IP PORT ����, ����ü ũ��) 
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	else
		puts("Connected ................. ");

	while (1)
	{
		fputs("Input message(Q or q = quit): ", stdout);			//	����ͷ� ���
		fgets(message, BUF_SIZE, stdin);							//	Ű����� �Է� ����
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))		// ��ҹ��� q�� ���� ��� ����
			break;

		/*
			1 param : sock�� descripter(handler)
			2 param : Buffer�� �ּҰ�
			3 param : ���� �� buffer ũ��
			4 return : ������ buffer ���� 
		*/
		str_len = write(sock, message, strlen(message));			//	message�� �ۼ��ؼ� �� �޼��� buffer�� ���� ���� ��ȯ

		recv_len = 0;
		while (recv_len < str_len)  // (� ���������� �� ���� < ���� ���� �޼�������), ���� ���� ���̰� �� ������ byte �б⸦ �ݺ�
		{
			recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);	// sock descripter ���� // &message[0] -> &message[1] -> &message[2] ... �̷��� 1���� byte�� �о 1�� ���� // �ѹ��� �ִ� 1023������ ����.
			if (recv_cnt == -1)
				error_handling("read() Error!");
			recv_len += recv_cnt;							//	1byte�� ������ ���� recv_len �� ��� ���Ͽ� � ���������� ������ ��.
			
		}
		message[str_len] = NULL;							//	������ ���� NULL�� �־� String���� �˸�
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
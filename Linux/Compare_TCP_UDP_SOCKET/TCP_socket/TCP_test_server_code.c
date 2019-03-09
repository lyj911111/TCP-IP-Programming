/*
	�����ڵ� ���.
	socket() -> bind() -> listen() -> accept() -> write() or read()
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
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;		//	������ ������ �Ϸù�ȣ�� ����.
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[] = "Hello World";

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	/*
		socket() �Լ� ����.
		return ��  : Descripter ��ȣ ��ȯ
		1��° param : - PF_INET  : IPv4 ���
					 - PF_INET6 : IPv6 ���
					 
		2��? param : - SOCK_STREAM : TCP ��Ļ��, (������ �Ҹ��� ����(�Ҹ�� ���û), ���� �������� ��, 1:1���θ� ���, Boundary�� �������� ����)
					 - SOCK_DGRAM  : UDP ��Ļ��, (�ӵ��� ����, �ļ��� ���, ũ������, Boundary�� ����(ex)write�� 3����, read�� 3�� �ʿ�))
		
		3��° param : protocol ���, 0���� �ص� ��. (�Ѱ���� �ۿ� ���� ����)
	*/
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));			//	������ �ּҸ� �����ϴ� ���� �����ϰ� 0���� clear�ϴ� �۾�
	serv_addr.sin_family = AF_INET;						//	�ּ�, IP ���� �ʱ�ȭ
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	// bind�Լ��� ȣ��, serv_sock�� �Ҵ�� ���ڿ�, &serv_addr �ּ������� �Ҵ��϶�
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	// serv_sock ���� �Ҵ�� �ּҷ� ���ͳݿ� �����϶� (listen ���� �� �ִ� ���·� ��)
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	// accept()�Լ�, ������ �ɷ��� ��ȭ�� �ִ��� �ü������ Ȯ���ϴ� ����. ������ �ö����� ��� ��ٸ��ٰ� ��ȭ�� ���� �׶� ��ȯ��.
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");

	// write()�Լ��� �̿��ؼ� �����͸� �� �� ����.
	write(clnt_sock, message, sizeof(message));
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
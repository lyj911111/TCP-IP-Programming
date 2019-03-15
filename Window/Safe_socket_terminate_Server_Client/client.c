/*
	Connected UDP ���� ������� �����.


	������ ��� �������α׷���.
	Visual studio ���� ����.
	������Ʈ(P) - <������Ʈ�̸�>�Ӽ� - �����Ӽ� - ��Ŀ - �Է� - �߰����Ӽ� - ws2_32.lib �� �߰�


	TCP��� Half-Close (������ ����� ����)

	Server                    client
	  |                         |
	  |<------���� ��û----------|
	  |                         |
	  |------���� ������ ����---->|  ������ ������ byte�� ũ�⸦ client�� �𸣹Ƿ� ������ �� ������, EOF�� ����.
	  |------���� ������ ����---->|
	  |------���� ������ ����---->|
	  |------���� ������ ����---->|
	  |                          |
	  |------EOF(���ۿϷ�)------>|  �� ���´ٰ� �˸�
	  |                         |
	  |<------Thank you---------|

	  ������ ��������,
	  * close() �Լ��� buffer�� ������ ������� stream�� �ƿ� �ݾƹ����� ������ ���� read, write �Ұ�.
	  * shutdown() �Լ��� �Է� stream, ��� stream�� �����ؼ� ���� �� ����.

	  int shutdown(SOCKET socket,  int howto);

	  1 param : ������ descripter ����
	  2 param : ���� ����� ���� ���� ����.
				 - SD_RECEIVE: �Է� ��Ʈ�� ����
				 - SD_SEND   : ��� ��Ʈ�� ����
				 - SD_BOTH   : ����� ��Ʈ�� ����
	  3 return: ���� 0, ���� -1

*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#define BUF_SIZE 30

void ErrorHandling(char * message);

int main(int argc, char * argv[])
{
	WSADATA wsaData;
	SOCKET Sock;
	FILE * fp;

	char buf[BUF_SIZE];
	int readCnt;
	SOCKADDR_IN servAddr;
	
	// Parsing ���ڰ� 3�� �ƴϸ� �ٽ� ����
	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT>\n", argv[0]);	// argv[0] : exe ���ϸ�		
		exit(1);
	}

	// ���� ���̺귯���� ����ϱ� ����.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() Error");
	
	fp = fopen("receive.dat", "wb");
	// ������ ����.
	Sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// Sever�� ������ �õ� ( connect() )
	connect(Sock, (SOCKADDR *)&servAddr, sizeof(servAddr));

	while ((readCnt = recv(Sock, buf, BUF_SIZE, 0)) != 0)
		fwrite((void*)buf, 1, readCnt, fp);

	puts("Receive file data");
	send(Sock, "Thank you! it was done.", 10, 0);

	fclose(fp);				// ������ �ݾ���
	closesocket(Sock);		// ������ �ݾ���. WSA�� clean up��.
	WSACleanup();

	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

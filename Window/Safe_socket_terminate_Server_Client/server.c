/*
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
#include <string.h>
#include <winsock2.h>		// <= �����쿡���� include (window socket)

#define BUF_SIZE 30

void ErrorHandling(char * message);

int main(int argc, int * argv[])
{
	WSADATA wsaData;
	SOCKET hservSock, hClntSock;
	FILE * fp;
	char buf[BUF_SIZE];
	int readCnt;

	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSz;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// WSAStartup() �����쿡�� �������α׷� ������ �˸�.
		ErrorHandling("WSAStartup() Error!");

	fp = fopen("file_server_win.c", "rb");
	hservSock = socket(PF_INET, SOCK_STREAM, 0);		//	socket() ���ϻ���, SOCKET �ڷ����� handler�� ���� (�������·� descripter�� ���)
	
	// ���ڷ� ���� �� �ʱ�ȭ
	memset(&servAddr, 0, sizeof(servAddr));				// �ּҰ��� ������ ������ �̸� �����ϰ� ġ��
	servAddr.sin_family = AF_INET;						// family����
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);		// ip����
	servAddr.sin_port = htons(atoi(argv[1]));			// Parsing���� 2��° ���ڸ� ���޹޾Ƽ� port��ȣ�� ����.

	// bind() IP�ּҸ� ���ε���.
	bind(hservSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hservSock, 5);

	clntAdrSz = sizeof(clntAddr);
	hClntSock = accept(hservSock, (SOCKADDR*)&clntAddr, &clntAdrSz);

	while (1)
	{
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE)
		{
			send(hClntSock, (char*)&buf, readCnt, 0);
			break;
		}
		send(hClntSock, (char*)&buf, BUF_SIZE, 0);
	}

	shutdown(hClntSock, SD_SEND);
	recv(hClntSock, (char*)buf, BUF_SIZE, 0);
	printf("Message from the Client: %s \n", buf);

	fclose(fp);
	closesocket(hservSock);								//	client ������ �ݰ�
	WSACleanup();										// �����쿡���� �ݵ�� WSA startup ������, cleanup ������� ��.
	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
/*
	������ ��� �������α׷���.
	Visual studio ���� ����.
	������Ʈ(P) - <������Ʈ�̸�>�Ӽ� - �����Ӽ� - ��Ŀ - �Է� - �߰����Ӽ� - ws2_32.lib �� �߰�

	connected UDP ����

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
	SOCKET servSock;
	char message[BUF_SIZE];
	int strLen;
	int clntadrSz;

	SOCKADDR_IN servAddr, clntAddr;
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// WSAStartup() �����쿡�� �������α׷� ������ �˸�.
		ErrorHandling("WSAStartup() Error!");

	servSock = socket(PF_INET, SOCK_DGRAM, 0);		//	socket() ���ϻ���, SOCKET �ڷ����� handler�� ���� (�������·� descripter�� ���)
	if (servSock == INVALID_SOCKET)
		ErrorHandling("UDP socket creation Error!");

	// ���ڷ� ���� �� �ʱ�ȭ
	memset(&servAddr, 0, sizeof(servAddr));				// �ּҰ��� ������ ������ �̸� �����ϰ� ġ��
	servAddr.sin_family = AF_INET;						// family����
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);		// ip����
	servAddr.sin_port = htons(atoi(argv[1]));			// Parsing���� 2��° ���ڸ� ���޹޾Ƽ� port��ȣ�� ����.

	// bind() IP�ּҸ� ���ε���.
	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() Error!");

	while (1)
	{
		clntadrSz = sizeof(clntAddr);
		strLen = recvfrom(servSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntadrSz);
		sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));

	}

	closesocket(servSock);								//	client ������ �ݰ�
	WSACleanup();										// �����쿡���� �ݵ�� WSA startup ������, cleanup ������� ��.
	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
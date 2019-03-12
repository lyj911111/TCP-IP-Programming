/*
	** ������� ������ ����, ����� ��û�ϴ� Ŭ���̾�Ʈ �ڵ� �ۼ�.
	
	�������� ����
	1. client�� server�� �������ڸ��� �ǿ����� ���������� 1byte���·� ����.
	2. client�� server�� �����ϴ� ������ 4byte�� ǥ��
	3. ������ ������ ����, ������ ������ �����Ѵ�. (���� �ϳ��� 4byte���� ǥ��)
	4. ���� +,-,* �� �ϳ��� ����.
	5. ������ �������� 4byte �������·� client���� �����Ѵ�.
	6. �������� ���� client�� server�� ������ �����Ѵ�.
	
	�������� ����
	ex)  -> 3���� �ǿ����� �����۽�
		[3][ , , , ][ , , , ][ , , , ][+] 
		1byte /     4byte 3��    /  ������ 1byte 
		
		 -> 2���� �ǿ����� �����۽�
		[2][ , , , ][ , , , ][-]
		1byte  /4byte 2��/  ������ 1byte
	
	tip.
		�ϳ��� �迭�� �پ��� �ڷ����� �����Ϸ���, �迭�� �ݵ�� char���� �����ؾ� �Ѵ�. (���� 4byte�� (int*)�� ĳ�����Ͽ� ������)
		
	Client �ڵ�

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024		//	�����͸� �ۼ����� ����
#define RLT_SIZE 4			//	Result size, �������ݿ� ���� �������� 4byte ������ ���� ������.
#define OPSZ 4				//	Operand Size, �������ݿ� ���� �ǿ����� 4byte ������ ���� ������.

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int sock;
	char opmsg[BUF_SIZE];			// ���������� ������ �ۼ����� �Ǵ� �޸� �迭. opmsg[1024 ũ��]
	int result, opnd_cnt, i;
	struct sockaddr_in serv_adr;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("Socket() Error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error");
	else
		printf("connected.............");

	// 1. client�� server�� �������ڸ��� �ǿ����� ���������� 1byte���·� ����. 
	fputs("Operand count : ", stdout);	// ����Ϳ� ���
	scanf("%d", &opnd_cnt);				// ������� ���� �Է��� �޾�, opnd_cnt(int��)�� ����.
	opmsg[0] = (char)opnd_cnt;			// �Է� ���� ���� int�� char���� casting�ؼ� �۽� �迭 ù��° 1byte�� ����.

	// 2. client�� server�� �����ϴ� ������ 4byte�� ǥ��
	// 3. ������ ������ ����, ������ ������ �����Ѵ�. (���� �ϳ��� 4byte���� ǥ��)
	for (i = 0; i < opnd_cnt; i++)					// ������ ���޹��� ���� ��ŭ for�� �ݺ�����.
	{
		printf("Operand %d : ", i + 1);				// opmsg[0]����Ʈ�� �̹� ä������, �� �ڷ� 4 byte�� �ľŹ��� ������ �Ҵ�.
		scanf("%d", (int*)&opmsg[i * OPSZ + 1]);	// opmsg[0x4+1], opmsg[1x4+1], opmsg[2x4+1]...  , char�� �迭�� �����ؾ� ������ (int*)���� casting�Ͽ� ����
	}												// opmsg[1]      opmsg[5]    , opmsg[9]...

	// 4. ���� +,-,* �� �ϳ��� ����.
	// 5. ������ �������� 4byte �������·� client���� �����Ѵ�.
	// 6. �������� ���� client�� server�� ������ �����Ѵ�.
	fgetc(stdin);									// ���ڸ� �Է¹޾ƾ� �ϴµ�, �ռ� ���ۿ� �����ִ� \n������ ������ ���� fgetc �Լ� ȣ��.
	fputs("Operator: ", stdout);					// ȭ�����
	scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);		// (+,-,*)�� �ϳ��� character ���ڸ� �Է� �ްڴ�. ex) opnd_cnt = 3�϶�, &opmsg[3 x 4 + 1] = opmsg[13], ���⿡ �����ڸ� ����.
	write(sock, opmsg, opnd_cnt * OPSZ + 2);		// ��ä�� ���ۿ� ������ NULL ���� �����Ͽ� ����. (�������� ��踦 ����)
	
	read(sock, &result, RLT_SIZE);					// ������ ���� ���� �����͸� 4byteũ��� �о &result �ּҿ� ����.
	

	printf("Operation result : %d \n", result);
	close(sock);
	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
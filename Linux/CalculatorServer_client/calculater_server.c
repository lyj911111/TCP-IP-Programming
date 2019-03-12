/*
	** ������� ������ ����, ����� ��û�ϴ� Ŭ���̾�Ʈ �ڵ� �ۼ�.
	
	��������
	1. client�� server�� �������ڸ��� �ǿ����� ���������� 1byte���·� ����.
	2. client�� server�� �����ϴ� ������ 4byte�� ǥ��
	3. ������ ������ ����, ������ ������ �����Ѵ�. (���� �ϳ��� 4byte���� ǥ��)
	4. ���� +,-,* �� �ϳ��� ����.
	5. ������ �������� 4byte �������·� client���� �����Ѵ�.
	6. �������� ���� client�� server�� ������ �����Ѵ�.

		�������� ����
	ex)  -> 3���� �ǿ����� �����۽�
		[3][ , , , ][ , , , ][ , , , ][+] 
		1byte /    4byte 3��    /  ������ 1byte 
		
		 -> 2���� �ǿ����� �����۽�
		[2][ , , , ][ , , , ][-]

	tip.
		�������� ���� ���� ���۰� �� �� �迭�� client���� ���� �����.
		�̸� �迭�� ���ܳ��� ������ �ؼ� ������� �ٽ� client���� write��.

	Server �ڵ�

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024		//	�ۼ����� ������ ������
#define OPSZ 4				//	�ǿ����� ����� 4byte�� ����.

void error_handling(char * message);
int calculate(int opnum, int opnds[], char operator);		// ����� ������ �Լ�.

int main(int argc, char * argv[])
{
	int serv_sock, clnt_sock;
	char opinfo[BUF_SIZE];						// ������ �ۼ����� ���� ���ۻ����� �迭
	int result, opnd_cnt, i;
	int recv_cnt, recv_len;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error!");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);		// client ���� ������

	// 5���� client�� ����
	for (i = 0; i < 5; i++)
	{
		opnd_cnt = 0;
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);	// �������ϰ� Ŭ���̾�Ʈ ���� ����
		read(clnt_sock, &opnd_cnt, 1);												// Ŭ���̾�Ʈ�� ��ó�� 1byte(�ǿ����� ����)�� ���� (��� �ǿ����ڰ� ���Դ��� �ľŵ� ������) for���� ����?

		recv_len = 0;																// ex) �ľ� �����Ͱ� 3�϶� ����             (13)
		while ((opnd_cnt * OPSZ + 1) > recv_len)									// �ľŵ� �ǿ����� ������ŭ Ƚ���� ������. ex) (3x4+1) > recv_len
		{																			//                                                                 
			recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);			// opnd_cnt�� 3�϶� ����, �迭��: &opinfo[1] &opinfo[5] &opinfo[9]  => [3][ , ,3, ][ , ,3, ][ , ,3, ][+]
			recv_len += recv_cnt;													// recv_cnt 0 4 4 4 
		}																			// recv_len 1 4 8 12
		result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);
		write(clnt_sock, (char*)&result, sizeof(result));
		close(serv_sock);
	}

	close(serv_sock);
	return 0;
}

/*
	��� ���� �Լ�.
	1st param: ��� �ǿ����ڰ� �ִ��� ����. (��� ���ڸ� ������ ����)
	2nd param: 4 byte�� �д� �ǿ�����(�� ���ÿ��� 3��)�� ���ڷ� ����.
	3rd param: char �� 1byte ������(+,-,*)
*/
int calculate(int opnum, int opnds[], char op)
{
	int result = opnds[0], i;
	switch (op)
	{
	case '+':
		for (i = 1; i < opnum; i++)
			result += opnds[i];			//	
		break;
	case '-':
		for (i = 1; i < opnum; i++)
			result -= opnds[i];
		break;
	case '*':
		for (i = 1; i < opnum; i++)
			result *= opnds[i];
		break;
	}

	return result;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
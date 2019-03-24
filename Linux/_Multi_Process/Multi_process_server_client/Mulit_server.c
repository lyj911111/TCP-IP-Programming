/*
	��Ƽ�½�ŷ ��� �������� ����.
	
	1. Ŭ���̾�Ʈ�� �������� ��û.
	2. �θ����μ���(����)�� accept() �Լ��� ���� ������ ����.
	3. �̶� ��ԵǴ� PID (�������� ��ũ����)�� �ڽ����μ������� �ѱ�. (���� fork()�Լ��� ���� �θ� ���� �Ӽ��� �׵��� ������. - ����)
	4. fock() �Լ��� ������ �ڽ����μ����� PID�� �������� ���񽺸� ����.

	* ������ ������ �ü�� ������, ������ü�� ������� �ʴ´�. (���� port���.)
	  �ٸ� ����Ǵ� ����, ���Ͽ��� ������ PID��, �� descipter�� ����� ���̴�.

/////////////////////////////////////////////////////////////////////
	�׸�1.

	[parent process]                   [ O S ]

	 socket descripter 1 ----->   server socket

	 socket descripter 2 ----->   client connected socket

	 (fock()�Լ� ȣ��� ���� ����)
	[child process]

	 socket descripter 1 ----->   server socket

	 socket descripter 2 ----->   client connected socket

//////////////////////////////////////////////////////////////////////
	 �׸�2.

[parent process]                            [ O S ]

	 socket descripter 1  ------------->   server socket

	 socket descripter 2    (close())      client connected socket

	 (fock()�Լ� ȣ��� ���� ����)
	[child process]

	 socket descripter 1    (close())      server socket

	 socket descripter 2 -------------->   client connected socket

 �� �׸�1 �� �׸�2 ���� ����.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message);
void read_childproc(int sig);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];


	if (argc != 2)
	{
		printf("Usage : %s <addr> \n", argv[0]);
		exit(1);
	}


	/* ���� ���μ����� ���� ���� ����. */
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flag = 0;
	state = sigaction(SIGCHLD, &act, 0);


	serv_sock = socket(PF_INET, SOCK_STREAM, 0);		// ���� ���� ����.
	memset(&serv_adr, 0, sizeof(serv_adr));				// �ʱ�ȭ ����
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.port = htons(atoi(argv[1]));

	// �θ� ���μ����� bind(), listen() �Լ� ȣ��.
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("bind() error");

	while (1)
	{
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);		// accept()�Լ��� ����, client���� �����û ������.

		if (clnt_sock == -1)
			continue;
		else
			puts("new client connected ...");	// client�� ������ �ȴٸ� �Ʒ��� ����.

		pid = fork();		//	fock() �Լ��� �ڽ� ���μ��� ����. (�ڽ����μ����� descripter = 0)
		if (pid == -1)		//	fock() �Լ� ���н� ��õ� ����ó��
		{
			close(clnt_sock);
			continue;
		}

		// pid = 0 �϶�, �ڽ� ���μ��� ����. (�ڽ� ���μ��� ����)
		if (pid == 0)
		{
			close(serv_sock);		// listen������ ���� (���ʿ��� ������ ����.)
			while ((ste_len = read(clnt_sock, buf, BUF_SIZE)) != 0)		// read & write ���� �Լ�
				write(clnt_sock, buf, str_len);

			close(clnt_sock);
			puts("client disconnected .....");
			return 0;
		}
		// �׿� �θ����μ��� ����.
		else
			close(clnt_sock);		// ���ʿ��� client�� ������ ����. �ڽ����μ����� �̹� ���񽺸� �����ϹǷ�.

	}
	close(serv_sock);

	return 0;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed process id : %d \n", pid);
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}



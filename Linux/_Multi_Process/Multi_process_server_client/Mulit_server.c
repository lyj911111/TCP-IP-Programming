/*
	멀티태스킹 기반 다중접속 서버.
	
	1. 클라이언트가 서버연결 요청.
	2. 부모프로세스(서버)의 accept() 함수를 통해 연결을 수락.
	3. 이때 얻게되는 PID (소켓파일 디스크립터)를 자식프로세스에게 넘김. (실제 fork()함수를 통해 부모가 가진 속성을 그데로 가져감. - 복제)
	4. fock() 함수로 복제된 자식프로세스는 PID를 바탕으로 서비스를 제공.

	* 생성된 소켓은 운영체제 소유로, 소켓자체가 복사되지 않는다. (동일 port사용.)
	  다만 복사되는 것은, 소켓에서 제공된 PID값, 즉 descipter만 복사될 뿐이다.

/////////////////////////////////////////////////////////////////////
	그림1.

	[parent process]                   [ O S ]

	 socket descripter 1 ----->   server socket

	 socket descripter 2 ----->   client connected socket

	 (fock()함수 호출로 인한 복제)
	[child process]

	 socket descripter 1 ----->   server socket

	 socket descripter 2 ----->   client connected socket

//////////////////////////////////////////////////////////////////////
	 그림2.

[parent process]                            [ O S ]

	 socket descripter 1  ------------->   server socket

	 socket descripter 2    (close())      client connected socket

	 (fock()함수 호출로 인한 복제)
	[child process]

	 socket descripter 1    (close())      server socket

	 socket descripter 2 -------------->   client connected socket

 위 그림1 이 그림2 으로 변함.
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


	/* 좀비 프로세스를 막기 위한 과정. */
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flag = 0;
	state = sigaction(SIGCHLD, &act, 0);


	serv_sock = socket(PF_INET, SOCK_STREAM, 0);		// 서버 소켓 생성.
	memset(&serv_adr, 0, sizeof(serv_adr));				// 초기화 과정
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.port = htons(atoi(argv[1]));

	// 부모 프로세스의 bind(), listen() 함수 호출.
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("bind() error");

	while (1)
	{
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);		// accept()함수를 통해, client와의 연결요청 수락함.

		if (clnt_sock == -1)
			continue;
		else
			puts("new client connected ...");	// client와 연결이 된다면 아래로 진행.

		pid = fork();		//	fock() 함수로 자식 프로세스 생성. (자식프로세스의 descripter = 0)
		if (pid == -1)		//	fock() 함수 실패시 재시도 예외처리
		{
			close(clnt_sock);
			continue;
		}

		// pid = 0 일때, 자식 프로세스 실행. (자식 프로세스 영역)
		if (pid == 0)
		{
			close(serv_sock);		// listen소켓을 닫음 (불필요한 소켓을 닫음.)
			while ((ste_len = read(clnt_sock, buf, BUF_SIZE)) != 0)		// read & write 에코 함수
				write(clnt_sock, buf, str_len);

			close(clnt_sock);
			puts("client disconnected .....");
			return 0;
		}
		// 그외 부모프로세스 영역.
		else
			close(clnt_sock);		// 불필요한 client와 소켓을 닫음. 자식프로세스가 이미 서비스를 제공하므로.

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



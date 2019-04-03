/*
	두개의 pipe를 생성하여 프로세스간 양방향 통신. (부모와 자식프로세스간의 통신)

	pipe가 하나일 때 양방향 통신을 한다면, 예를들어 2개의 프로세스가 접근한다고 치면,
	하나의 프로세스가 write으로 pipe메모리 공간에 데이터를 저장하고, 다른 프로세스에게 전달 하려고 할때
	본인이 아주 빠르다면 본인이 read로 데이터를 가져갈 수 있다.
	그러면 나머지 프로세스는 데이터를 받지 못하고 blocking 상태에 빠질 우려가 있음.

	이를 방지하기 위해, 파이프를 2개를 생성.
	하나의 파이프에서 A 에서 B으로 보내고, 다른 하나의 파이프에서는 B 에서 A 으로 보냄.


	pipe() 함수는 int형 2개의 배열공간에 입출력이 가능한 파일 디스크립터(File Descripter)를 생성.
	- fds[0]: 출력을 위한 디스크립터
	- fds[1]: 입력을 위한 디스크립터

*/


#include<stdio.h>
#include<unistd.h>

#define BUF_SIZE 30

int main(int argc, char * argv[])
{
	int fds1[2], fds2[2];			// 2개의 pipe에 할당할 int형 2개 배열 메모리생성.
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;				// 프로세스id 정보를 갖을 구조체

	pipe(fds1);				// 첫번째 파이프 생성. OS에게 메모리공간 생성을 요청하고 fds의 주소값을 알려줌
	pipe(fds2);				// 두번째 파이프 생성.
	pid = fork();			//	자식 프로세스 생성. (위 모든것을 복사 별도메모리를 갖음)

	// 자식 프로세스 영역, fds[1]: 입력디스크립터 전달, str주소값의 데이터전달, 버퍼사이즈
	if (pid == 0)
	{
		write(fds1[1], str1, sizeof(str1));	// 첫번째 파이프에 값을 입력만 함.
		read(fds2[0], buf, BUF_SIZE);		// 두번째 파이프에서는 값을 읽기만 함.
		printf("Child Process output: %s \n", buf);
	}
	// 부모 프로세스 영역, fds[0]: 출력디스크립터 전달, 출력받은것을 buf에 저장, 내가 할당한 사이즈크기 
	else
	{
		read(fds1[0], buf, BUF_SIZE);	// 첫번째 파이프에서 값을 읽기만 함.
		printf("Parents process output: %s \n", buf);
		write(fds2[1], str2, sizeof(str2));	// 두번째 파이프에서 값을 입력만 함.

		sleep(3);	// 의미 없고, 부모 프로세스 종료를 지연시키려고
	}

	return 0;
}



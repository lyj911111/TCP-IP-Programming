/*
	프로세스간 통신. (부모와 자식프로세스간의 통신 예제)

	프로세스는 기본적으로 메모리를 공유하지 않고있다.
	프로세스가 메모리를 공유하기 위해서는 OS가 pipe라는 메모리를 생성시켜주고,
	두 프로세스가 pipe라는 메모리에 함께 접근해야 한다. 

	pipe() 함수는 int형 2개의 배열공간에 입출력이 가능한 파일 디스크립터(File Descripter)를 생성.
	- fds[0]: 출력을 위한 디스크립터
	- fds[1]: 입력을 위한 디스크립터



*/


#include<stdio.h>
#include<unistd.h>

#define BUF_SIZE 30

int main(int argc, char * argv[])
{
	int fds[2];							// pipe() 함수에 할당할 int형 2개 배열 메모리생성.
	char str[] = "Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;				// 프로세스id 정보를 갖을 구조체

	pipe(fds);				//	OS에게 메모리공간 생성을 요청하고 fds의 주소값을 알려줌
	pid = fork();			//	자식 프로세스 생성. (위 모든것을 복사 별도메모리를 갖음)

	// 자식 프로세스 영역, fds[1]: 입력디스크립터 전달, str주소값의 데이터전달, 버퍼사이즈
	if (pid == 0)
	{
		write(fds[1], str, sizeof(str));	// 입력
	}
	// 부모 프로세스 영역, fds[0]: 출력디스크립터 전달, 출력받은것을 buf에 저장, 내가 할당한 사이즈크기 
	else
	{
		read(fds[0], buf, BUF_SIZE);	// 값을 읽음
		puts(buf);						// 값을 모니터에 씀
	}

	return 0;
}



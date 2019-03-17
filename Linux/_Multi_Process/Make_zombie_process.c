/*
	의도적인 좀비 프로세스 생성과 확인.
	
	자식 프로세스가 부모 프로세스보다 먼저 종료되어 값을 return 하면
	자식 처리가 안되어 자식프로세스는 메모리만 잡아먹는 좀비로 변함.
	
	그러나 부모프로세스가 종료되면 함께 종료
	
	리눅스에서 확인하는 방법.
	명령어 - ps au
	(에서 +z 모양의 좀비를 찾으면됨).
	
*/

#include<stdio.h>
#include<unistd.h>

int main(int argc, char * argv[])
{
	pid_t pid = fork();								// 1. 자식 프로세스 생성
	
	if (pid == 0)
		puts("Hi, I am a child process");			// 2. 자식 프로세스의 영역 출력
	else
	{
		printf("child process ID : %d \n", pid);	// 3. 부모 프로세스 영역 출력, 그리고 30초간 멈추고
		sleep(30);
	}
	
	if (pid == 0)
		printf("End Child process");				// 3. 자식 프로세스 종료, 값 리턴 => 부모가 종료되지 않았으므로, 좀비가 되어버림.
	else
		printf("End parent process");				// 4. 30초 멈춤이 끝나고 종료하여 리턴, 이때 좀비가 된 자식도 데려감.
	
	return 0;
}

/*
	의도적인 좀비 프로세스 생성과 확인.
	
	자식 프로세스가 부모 프로세스보다 먼저 종료되어 값을 return 하면
	자식 처리가 안되어 자식프로세스는 메모리만 잡아먹는 좀비로 변함.
	
	그러나 부모프로세스가 종료되면 함께 종료
	
	리눅스에서 확인하는 방법.
	명령어 - ps au
	(에서 +z 모양의 좀비를 찾으면됨).
	
	* wait() 함수는 좀비프로세스를 소멸시키는 방법이지만, 호출시점에서 종료된 자식프로세스가 없으면
	  임의 자식 프로세스가 종료될때까지 blocking 상태에 놓여 영원히 종료 불가할 수 도 있다.
	
	
	pid_t  wait(int *  statloc);
	
	성공 -> 종료된 자식 PID반환, 실패시 -1
	
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(int argc, char * argv[])
{
	int status;
	pid_t pid = fork();								// 1. 자식 프로세스 생성.
	
	if (pid == 0)
		return 3;									// 2. 자식 프로세스가 아무 값을 리턴. 그리고 종료. <= 좀비가 되어버림
	else
	{
		printf("child process ID : %d \n", pid);	// 3. 부모 프로세스 영역 출력
		pid = fork();								//    새로운 자식 프로세스 생성.
		
		if ( pid == 0)
			exit(7);								// 4. 새로운 자식 프로세스도 OS에 7을 리턴하고 종료. <= 좀비가 되어버림.
		else
		{
			printf("child process ID : %d \n", pid);						// 5. 부모 프로세스 실행

			wait(&status);													// wait() 함수! 종료된 자식프로세스 ID (PID)를 반환. 해당 PID는 완전히 소멸
			if(WIFEXITED(status))											// WIFEXITED 매크로, 자식프로세스의 정상종료 여부를 확인.
				printf("Child send one : %d \n", WEXITSTATUS(status));		// 자식프로세스가 return, exit한 값들을 출력함.
		
			wait(&status);
			if(WIFEXITED(status))
				printf("Child send one : %d \n", WEXITSTATUS(status));
			sleep(30);														// $ ps au 명령어로 자식프로세스 상태를 확인용.
		}
	}
	
	return 0;
}

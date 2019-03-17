/*
	의도적인 좀비 프로세스 생성과 확인.
	
	자식 프로세스가 부모 프로세스보다 먼저 종료되어 값을 return 하면
	자식 처리가 안되어 자식프로세스는 메모리만 잡아먹는 좀비로 변함.
	
	그러나 부모프로세스가 종료되면 함께 종료
	
	리눅스에서 확인하는 방법.
	명령어 - ps au
	(에서 +z 모양의 좀비를 찾으면됨).
	
	pid_t  pidwait( pid_t  pid , int *  statloc,  int options);
	
	1 param: 종료를 확인하고자 하는 자식 프로세스의 ID전달. -1을 전달 시 wait함수와 마찬가지로 임의 자식 프로세스가 종료되길 기다림.
	2 param: &status 아래와 같이.
	3 param: sys/wait.h 에 선언되어있는 매크로 상수, WNOHANG 인자전달 시, 종료된 자식 프로세스가 존재하지 않아도 Blocking상태가 아니고 0을 반환하여 빠져나옴.
	
	성공 -> 종료된 자식 PID반환, 실패시 -1 , 0 일반종료(blocking 으로 기다리지 않음)
	
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char * argv[])
{
	int status;
	pid_t pid = fork();								// 1. 자식 프로세스 생성.
	
	if (pid == 0)
	{
		sleep(15);									// 2. 자식 프로세스가 15초동안 있다가 아무 값을 리턴. 그리고 종료. <= 좀비가 되어버림
		return 24;									//    자식 프로세스의 종료를 지연시키기.
	}
	else
	{
		while( !(waitpid(-1, &status, WNOHANG)) )	// 3. 0이 아닐때까지 계속 실행, -1: PID를 지정하지 않고 임의로, 반복적으로 종료된 자식프로세스 확인하고, 없으면 0으로 빠져나감.
		{											//    3초마다 반복적으로 수행.
			sleep(3);
			puts("sleep 3 sec");
		}
		
		if (WIFEXITED(status))
			printf("Child send %d \n", WEXITSTATUS(status));
	}

	return 0;
}

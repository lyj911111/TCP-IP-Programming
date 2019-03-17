#include<stdio.h>
#include<unistd.h>

int gval = 10;

int main(int argc, char * argv[])
{
	pid_t pid;
	int lval = 20;
	gval++, lval +=5;
	
	pid = fork();
	
	if (pid == 0)
		gval += 2, lval += 2;	// 반환값 0, 자식프로세스 일떄 수행
	else
		gval -= 2, lval -= 2;	// 반환값이 0이 아닐때 (부모프로세스) 수행.
	
	if (pid == 0)
		printf("Child process: [%d, %d] \n", gval, lval);
	else
		printf("Parent Process: [%d %d] \n", gval, lval);
	
	return 0;
}

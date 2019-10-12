/*
	�ΰ��� pipe�� �����Ͽ� ���μ����� ����� ���. (�θ�� �ڽ����μ������� ���)

	pipe�� �ϳ��� �� ����� ����� �Ѵٸ�, ������� 2���� ���μ����� �����Ѵٰ� ġ��,
	�ϳ��� ���μ����� write���� pipe�޸� ������ �����͸� �����ϰ�, �ٸ� ���μ������� ���� �Ϸ��� �Ҷ�
	������ ���� �����ٸ� ������ read�� �����͸� ������ �� �ִ�.
	�׷��� ������ ���μ����� �����͸� ���� ���ϰ� blocking ���¿� ���� ����� ����.

	�̸� �����ϱ� ����, �������� 2���� ����.
	�ϳ��� ���������� A ���� B���� ������, �ٸ� �ϳ��� ������������ B ���� A ���� ����.


	pipe() �Լ��� int�� 2���� �迭������ ������� ������ ���� ��ũ����(File Descripter)�� ����.
	- fds[0]: ����� ���� ��ũ����
	- fds[1]: �Է��� ���� ��ũ����

*/


#include<stdio.h>
#include<unistd.h>

#define BUF_SIZE 30

int main(int argc, char * argv[])
{
	int fds1[2], fds2[2];			// 2���� pipe�� �Ҵ��� int�� 2�� �迭 �޸𸮻���.
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;				// ���μ���id ������ ���� ����ü

	pipe(fds1);				// ù��° ������ ����. OS���� �޸𸮰��� ������ ��û�ϰ� fds�� �ּҰ��� �˷���
	pipe(fds2);				// �ι�° ������ ����.
	pid = fork();			//	�ڽ� ���μ��� ����. (�� ������ ���� �����޸𸮸� ����)

	// �ڽ� ���μ��� ����, fds[1]: �Էµ�ũ���� ����, str�ּҰ��� ����������, ���ۻ�����
	if (pid == 0)
	{
		write(fds1[1], str1, sizeof(str1));	// ù��° �������� ���� �Է¸� ��.
		read(fds2[0], buf, BUF_SIZE);		// �ι�° ������������ ���� �б⸸ ��.
		printf("Child Process output: %s \n", buf);
	}
	// �θ� ���μ��� ����, fds[0]: ��µ�ũ���� ����, ��¹������� buf�� ����, ���� �Ҵ��� ������ũ�� 
	else
	{
		read(fds1[0], buf, BUF_SIZE);	// ù��° ���������� ���� �б⸸ ��.
		printf("Parents process output: %s \n", buf);
		write(fds2[1], str2, sizeof(str2));	// �ι�° ���������� ���� �Է¸� ��.

		sleep(3);	// �ǹ� ����, �θ� ���μ��� ���Ḧ ������Ű����
	}

	return 0;
}


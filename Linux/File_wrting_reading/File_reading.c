/*
	�������� ������ ���Ϸ� ���� (�ܼ�, ����, ���� ���)
	���Ͽ� ������ �б�

	ssize_t read(int fd, const void * buf, size_t nbytes);

	fd     : Descripter ���ڸ� ����, 0,1,2... �� �ֿܼ� ����Ǿ� ������ �ֿܼ� ���, ���Ͽ� ����Ǿ������� ���Ͽ� ����, ���Ͽ� ����Ǿ������� ��� �������� ���� ����
	buf    : �迭�� �ּҰ�
	nbytes : ����Ʈ ������


	���� ���� -> ���� �б� -> ���� �ݱ�
	open()       read()     close()
*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUF_SIZE 100

void error_handling(char * message);

int main(void)
{
	int fd;
	char buf[BUF_SIZE];

	// ������ ����, O_CREAT: ���� ����, O_WRONLY: ���� ����, O_TRUNC: ���� �����ʹ� ���� ����
	fd = open("data.txt", O_RDONLY);
	if (fd == -1)
		error_handling("open() error!");
	printf("file descripter: %d \n", fd);

	// fd ��ũ������ buf�� �ִ� ������ sizeof(buf)ũ�⸸ŭ ����(����)�϶�
	if (read(fd, buf, sizeof(buf)) == -1)
		error_handling("write() error!");
	printf("file data: %s", buf);

	// ������ �ݾƶ�.
	close(fd);

	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
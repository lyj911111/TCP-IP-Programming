/*
	�������� ������ ���Ϸ� ���� (�ܼ�, ����, ���� ���)
	���Ͽ� ������ ����

	ssize_t write(int fd, const void * buf, size_t nbytes);

	fd     : Descripter ���ڸ� ����, 0,1,2... �� �ֿܼ� ����Ǿ� ������ �ֿܼ� ���, ���Ͽ� ����Ǿ������� ���Ͽ� ����, ���Ͽ� ����Ǿ������� ��� �������� ���� ����
	buf    : �迭�� �ּҰ�
	nbytes : ����Ʈ ������


	���� ���� -> ���� ���� -> ���� �ݱ�
	open()       write()     close()
*/

#include <unistd.h>

int main(void)
{
	int fd;
	char buf[] = "Let's go!\n";

	// ������ ����, O_CREAT: ���� ����, O_WRONLY: ���� ����, O_TRUNC: ���� �����ʹ� ���� ����
	fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
	if (fd == -1)
		error_handling("open() error!");
	printf("file descripter: %d \n", fd);

	// fd ��ũ������ buf�� �ִ� ������ sizeof(buf)ũ�⸸ŭ ����(����)�϶�
	if (write(fd, buf, sizeof(buf)) == -1)
		error_handling("write() error!");

	// ������ �ݾƶ�.
	close(fd);

	return 0;
}
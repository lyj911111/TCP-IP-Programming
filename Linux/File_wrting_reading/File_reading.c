/*
	리눅스는 모든것을 파일로 간주 (콘솔, 파일, 소켓 등등)
	파일에 데이터 읽기

	ssize_t read(int fd, const void * buf, size_t nbytes);

	fd     : Descripter 숫자를 전달, 0,1,2... 등 콘솔에 연결되어 있으면 콘솔에 출력, 파일에 연결되어있으면 파일에 쓰기, 소켓에 연결되어있으면 상대 소켓으로 값을 전달
	buf    : 배열의 주소값
	nbytes : 바이트 사이즈


	파일 열기 -> 파일 읽기 -> 파일 닫기
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

	// 파일을 열기, O_CREAT: 파일 생성, O_WRONLY: 쓰기 전용, O_TRUNC: 기존 데이터는 전부 삭제
	fd = open("data.txt", O_RDONLY);
	if (fd == -1)
		error_handling("open() error!");
	printf("file descripter: %d \n", fd);

	// fd 디스크립터의 buf에 있는 내용을 sizeof(buf)크기만큼 전송(쓰기)하라
	if (read(fd, buf, sizeof(buf)) == -1)
		error_handling("write() error!");
	printf("file data: %s", buf);

	// 파일을 닫아라.
	close(fd);

	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
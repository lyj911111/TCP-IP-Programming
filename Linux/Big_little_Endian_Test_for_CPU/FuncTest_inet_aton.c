/*
	함수 형태
	in_addr_t  inet_addr  (const char * string,  struct in_addr * addr );
	-> 성공시 1, 실패시 0반환.
	   
	우리 볼수있는 형태의 문자열 주소값 xxx.xxx.xxx.xxx 을 네트워크에서 사용할 수 있는 32비트 주소값으로 출력.   
	
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

void error_handling(char * message);

int main(int argc, char * argv[])
{
	
	char * addr = "127.232.124.79";	//	임의 IP주소 넣음.
	struct sockaddr_in addr_inet;
	
	// 첫번째 인자: string값
	// 두번? 인자: sockaddr_in 구조체 내부에 있는 sin_addr의 맴버 s_addr에 접근 주소값 대입
	if (! inet_aton(addr, &addr_inet.sin_addr))
		error_handling("conversion error occurred");
	// sockaddr_in 구조체 내부에 있는 sin_addr의 맴버 s_addr에 접근해서 출력.
	else
		printf( "Network ordered integer addr is %#x \n", addr_inet.sin_addr.s_addr );

	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}	
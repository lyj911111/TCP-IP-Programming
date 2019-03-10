/*
	함수 형태
	in_addr_t  inet_addr  (const char * string);
	-> 성공시 빅엔디안 변환 32비트 반환,
	   실패시 INADDR_NONE 반환.
	   
	예제, IP주소값을 String으로 전달했을때, 빅엔디안으로 변환된 32비트 값을 받아보기 위함.
	
*/

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char * argv[])
{
	
	char * addr1 = "12.34.56.255";	// 1 byte의 최대 크기는 0~255
	char * addr2 = "12.34.56.256";	// 범위를 벗어났으므로 오류 발생해야 함.
	
	printf(" Print out addr1 \n");
	unsigned long conv_addr = inet_addr(addr1);
	if (conv_addr == INADDR_NONE)
		printf("Error ocrrured \n");
	else
		printf("Network ordered integer addr : %#lx \n", conv_addr);
	
	printf("print out addr2 \n");
	conv_addr = inet_addr(addr2);
	if (conv_addr == INADDR_NONE)
		printf("Error ocrrured \n");
	else
		printf("Network ordered integer addr : %#lx \n", conv_addr);
	

	return 0;
}

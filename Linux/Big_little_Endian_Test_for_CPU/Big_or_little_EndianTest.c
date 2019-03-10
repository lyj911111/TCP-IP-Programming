/*
	네트워크 시스템의 데이터순서는 Big endian 으로 통일.
	
	데이터 순서 변환 함수
	htons() : host to network as a short (2 byte - for port)	
	ntohs() : network to host as a short

	htonl() : host to network as a long (4 byte - for network address)
	ntohl() : network to host as a long

	본인 CPU가 big endian일 경우, 그데로 출력.
	little endian일 경우 순서가 바껴서 출력.
	대부분의 Intel, AMD 계열 CPU는 little Endian으로 뒤바껴서 출력될 확률이 높음.
*/

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char * argv[])
{
	unsigned short host_port = 0x1234;	// port는 2byte [0x12][0x34]
	unsigned short net_port;
	unsigned long host_addr = 0x12345678;  // IP는 4byte [0x12][0x34][0x56][0x78]
	unsigned long net_addr;

	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("Host PC ordered port:  %#x \n", host_port);
	printf("Network ordered port:  %#x \n", net_port);
	printf("Host PC ordered address:  %#x \n", host_addr);
	printf("Network ordered address:  %#x \n", net_addr);

	return 0;
}

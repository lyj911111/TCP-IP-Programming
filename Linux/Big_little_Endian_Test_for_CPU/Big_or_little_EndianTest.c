/*
	��Ʈ��ũ �ý����� �����ͼ����� Big endian ���� ����.
	
	������ ���� ��ȯ �Լ�
	htons() : host to network as a short (2 byte - for port)	
	ntohs() : network to host as a short

	htonl() : host to network as a long (4 byte - for network address)
	ntohl() : network to host as a long

	���� CPU�� big endian�� ���, �׵��� ���.
	little endian�� ��� ������ �ٲ��� ���.
	��κ��� Intel, AMD �迭 CPU�� little Endian���� �ڹٲ��� ��µ� Ȯ���� ����.
*/

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char * argv[])
{
	unsigned short host_port = 0x1234;	// port�� 2byte [0x12][0x34]
	unsigned short net_port;
	unsigned long host_addr = 0x12345678;  // IP�� 4byte [0x12][0x34][0x56][0x78]
	unsigned long net_addr;

	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("Host PC ordered port:  %#x \n", host_port);
	printf("Network ordered port:  %#x \n", net_port);
	printf("Host PC ordered address:  %#x \n", host_addr);
	printf("Network ordered address:  %#x \n", net_addr);

	return 0;
}

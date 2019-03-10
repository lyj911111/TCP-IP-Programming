/*
	�Լ� ����
	in_addr_t  inet_addr  (const char * string);
	-> ������ �򿣵�� ��ȯ 32��Ʈ ��ȯ,
	   ���н� INADDR_NONE ��ȯ.
	   
	����, IP�ּҰ��� String���� ����������, �򿣵������ ��ȯ�� 32��Ʈ ���� �޾ƺ��� ����.
	
*/

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char * argv[])
{
	
	char * addr1 = "12.34.56.255";	// 1 byte�� �ִ� ũ��� 0~255
	char * addr2 = "12.34.56.256";	// ������ ������Ƿ� ���� �߻��ؾ� ��.
	
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

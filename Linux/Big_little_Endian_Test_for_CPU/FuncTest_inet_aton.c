/*
	�Լ� ����
	in_addr_t  inet_addr  (const char * string,  struct in_addr * addr );
	-> ������ 1, ���н� 0��ȯ.
	   
	�츮 �����ִ� ������ ���ڿ� �ּҰ� xxx.xxx.xxx.xxx �� ��Ʈ��ũ���� ����� �� �ִ� 32��Ʈ �ּҰ����� ���.   
	
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

void error_handling(char * message);

int main(int argc, char * argv[])
{
	
	char * addr = "127.232.124.79";	//	���� IP�ּ� ����.
	struct sockaddr_in addr_inet;
	
	// ù��° ����: string��
	// �ι�? ����: sockaddr_in ����ü ���ο� �ִ� sin_addr�� �ɹ� s_addr�� ���� �ּҰ� ����
	if (! inet_aton(addr, &addr_inet.sin_addr))
		error_handling("conversion error occurred");
	// sockaddr_in ����ü ���ο� �ִ� sin_addr�� �ɹ� s_addr�� �����ؼ� ���.
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
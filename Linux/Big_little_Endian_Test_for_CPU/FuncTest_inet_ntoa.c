/*
	�Լ� ����
	char * inet_ntoa  (struct in_addr addr );
	-> ������ ��ȯ�� ���ڿ� �ּ� ��, ���н� -1 ��ȯ.
	   
	16���� ������ �Ǿ��ִ� Network ordered number�� �츮�� �� �� �ִ� ������ xx.xx.xx.xx IP�ּҷ� ��ȯ�����ִ� ����
	
	�ο� ����:
		�Լ��� ��ȯ���� char * �̴�. �ּҰ� ���ڿ��� �޸� ��� �ֱ⶧���� �װ��� ������ �� �ִٴ� ���,
		�Լ� ���������� �޸𸮸� �Ҵ��Ͽ� ��ȯ�� ���ڿ��� ���ϰ� ����.
		���� �Լ�ȣ�� �� ������ ��ȯ�� ���ڿ��� �����ϴ� ���� ����.
		�ٽ� inet_ntoa �� ȣ��Ǹ� ���� ���ڿ� ������ ����������.
		��, inet_ntoa �Լ��� ��ȣ��Ǳ��� ���ڿ� ������ ����ؾ� �Ѵٸ� ���� char * ptr ���� �����ؾ� ��.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	
	struct sockaddr_in addr1, addr2;
	char * str_ptr;						// ���ڿ��� ������ ������
	char str_arr[20];					// ����� ������ �迭����
	
	// ���Ƿ� IP�ּ� ��ȯ �� �Ҵ�.
	addr1.sin_addr.s_addr = htonl(0x01020304);
	addr2.sin_addr.s_addr = htonl(0x01010101);
	
	str_ptr = inet_ntoa(addr1, sin_addr);					// ������ �����ͺ����� ���ڿ� ���� IP�ּҸ� ����
	strcpy(str_arr, str_ptr);								// ��µ� IP�ּ� string�� str_arr�� ����
	printf("Dotted-Decimal notation 1: %s \n", str_ptr);
	
	inet_ntoa(addr2, sin_addr);
	printf("Dotted-Decimal notation 2: %s \n", str_ptr);
	printf("Dotted-Decimal notation 3: %s \n", str_arr);
	
	return 0;
}

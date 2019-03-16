/*
	IP�ּҸ� �̿��ؼ� ������ ���� ������.
	
	�Լ� ����
		struct hostent * gethostbyaddr(const char * addr,  socklen_t  len,  int  family);
		
		1 param : IP�ּҸ� ���ϴ� in_addr ����ü ������ ������ ����, IPv4�� �پ��� ������ ���� �� �ֵ��� char�� �Ϲ�ȭ
		2 param : ù��° ������ �ּ����� ����, IPv4�� 4������, IPv6�� 16�� ����
		3 param : �ּ�ü��, AF_INET or AF_INET6
	
	
	�ڷ��� ����ü ����
		struct hostent
		{
			char * h_name;            // Official name ���� ��Ī.
			char ** h_aliases;        // alias list, ����� �����θ�.
			int h_addrtype;           // IPv4����, IPv6 ���� type�� ����
			int h_length;             // �ּҰ� ���� ����
			char ** h_addr_list;      // ���� IP�ּҰ����� ������ ��, alias�� ������ �̸�, IP�ּҵ� ��������. (char�� �ڷ����� �� ������, byte������ �������� ����, �������)
		}			
	
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int i;
	struct hostent *host;
	struct sockaddr_in addr;
	if (argc != 2)
	{
		printf("Usage : %s <addr> \n", argv[0]);
		exit(1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);

	if (!host)
		error_handling("gethost.... error");

	printf("Official name : %s \n", host->h_name);	// ����ü ù��°����, ���ĸ��� ������.
	
	for (i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s \n", i+1, host->h_aliases[i]);	// �ι�° ����, ����� Domain name�� ������. (������ �ִٸ� ������ ������)
	
	printf("Aliases type : %s \n", (host->h_addrtype == AF_INET)? "AF_INET" : "AF_INET6");		// IPv4���� 6 ���� Ȯ���� ������
	
	for (i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d: %s \n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));	// ������ IP�ּҸ� ������.

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}



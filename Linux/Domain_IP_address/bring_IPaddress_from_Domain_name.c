/*
	IP�ּҰ��� ���÷� �ٲ�� ������, domain name�� ���� ������ ����.
	���� �ڵ��, domain name�� �ľ��ؼ�, �� domain�� �����ִ� IP�� �Լ�����ü�� ���� �ִ� ��� ������ �����ؼ� ���.
	���� domain�� official name�� alias domain���� ����.
	
	�Լ� ����
		struct hostent * gethostbyname(const char * hostname);
	
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
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int i;
	struct hostent *host;
	if (argc != 2)
	{
		printf("Usage : %s <addr> \n", argv[0]);
		exit(1);
	}

	host = gethostbyname(argv[1]);		// �����θ��� �Ľ�
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



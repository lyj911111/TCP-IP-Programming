/*
	IP주소값을 수시로 바뀔수 있지만, domain name은 결코 변하지 않음.
	다음 코드는, domain name을 파씽해서, 그 domain이 갖고있는 IP외 함수구조체가 갖고 있는 모든 정보를 리턴해서 출력.
	
	함수 원형
		struct hostent * gethostbyname(const char * hostname);
	
	자료형 구조체 원형
		struct hostent
		{
			char * h_name;            // Official name 공식 도메인명.
			char ** h_aliases;        // alias list, 같은 유사 도메인명.
			int h_addrtype;           // IPv4인지, IPv6 인지 type을 결정
			int h_length;             // 주소값 길이 정보
			char ** h_addr_list;      // 실제 IP주소값들을 가지고 옴, alias가 여러개 이면, IP주소도 여러개임.
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

	host = gethostbyname(argv[1]);
	if (!host)
		error_handling("gethost.... error");

	printf("Official name : %s \n", host->h_name);
	
	for (i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s \n", i+1, host->h_aliases[i]);
	
	printf("Aliases type : %s \n", (host->h_addrtype == AF_INET)? "AF_INET" : "AF_INET6");
	
	for (i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d: %s \n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}



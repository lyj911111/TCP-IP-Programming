/*
	IP주소를 이용해서 도메인 정보 얻어오기.
	
	함수 원형
		struct hostent * gethostbyaddr(const char * addr,  socklen_t  len,  int  family);
		
		1 param : IP주소를 지니는 in_addr 구조체 변수의 포인터 전달, IPv4외 다양한 정보를 받을 수 있도록 char로 일반화
		2 param : 첫번째 인자의 주소정보 길이, IPv4는 4가전달, IPv6은 16이 전달
		3 param : 주소체계, AF_INET or AF_INET6
	
	
	자료형 구조체 원형
		struct hostent
		{
			char * h_name;            // Official name 공식 명칭.
			char ** h_aliases;        // alias list, 연결된 도메인명.
			int h_addrtype;           // IPv4인지, IPv6 인지 type을 결정
			int h_length;             // 주소값 길이 정보
			char ** h_addr_list;      // 실제 IP주소값들을 가지고 옴, alias가 여러개 이면, IP주소도 여러개임. (char의 자료형을 쓴 이유는, byte단위로 가져오기 때문, 옛날방식)
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

	printf("Official name : %s \n", host->h_name);	// 구조체 첫번째인자, 공식명을 가져옴.
	
	for (i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s \n", i+1, host->h_aliases[i]);	// 두번째 인자, 연결된 Domain name을 가져옴. (여러개 있다면 여러개 가져옴)
	
	printf("Aliases type : %s \n", (host->h_addrtype == AF_INET)? "AF_INET" : "AF_INET6");		// IPv4인지 6 인지 확인후 가져옴
	
	for (i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d: %s \n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));	// 실제로 IP주소를 가져옴.

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}



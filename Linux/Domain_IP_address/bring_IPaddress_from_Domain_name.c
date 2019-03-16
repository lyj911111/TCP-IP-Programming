/*
	IP주소값을 수시로 바뀔수 있지만, domain name은 결코 변하지 않음.
	다음 코드는, domain name을 파씽해서, 그 domain이 갖고있는 IP외 함수구조체가 갖고 있는 모든 정보를 리턴해서 출력.
	실제 domain은 official name과 alias domain들이 있음.
	
	함수 원형
		struct hostent * gethostbyname(const char * hostname);
	
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

	host = gethostbyname(argv[1]);		// 도메인명을 파싱
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



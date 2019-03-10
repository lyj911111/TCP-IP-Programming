/*
	함수 형태
	char * inet_ntoa  (struct in_addr addr );
	-> 성공시 변환된 문자열 주소 값, 실패시 -1 반환.
	   
	16진수 정수로 되어있는 Network ordered number를 우리가 볼 수 있는 형태의 xx.xx.xx.xx IP주소로 변환시켜주는 예제
	
	부연 설명:
		함수의 반환형은 char * 이다. 주소값 문자열이 메모리 어딘가 있기때문에 그곳을 참조할 수 있다는 얘기,
		함수 내부적으로 메모리를 할당하여 변환된 문자열을 지니고 있음.
		따라서 함수호출 후 가급적 반환된 문자열을 저장하는 것이 좋다.
		다시 inet_ntoa 가 호출되면 이전 문자열 정보는 지워짐으로.
		즉, inet_ntoa 함수가 재호출되기전 문자열 정보를 기억해야 한다면 별도 char * ptr 으로 저장해야 함.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	
	struct sockaddr_in addr1, addr2;
	char * str_ptr;						// 문자열을 저장할 포인터
	char str_arr[20];					// 출력할 문자의 배열선언
	
	// 임의로 IP주소 변환 후 할당.
	addr1.sin_addr.s_addr = htonl(0x01020304);
	addr2.sin_addr.s_addr = htonl(0x01010101);
	
	str_ptr = inet_ntoa(addr1, sin_addr);					// 선언한 포인터변수에 문자열 형태 IP주소를 저장
	strcpy(str_arr, str_ptr);								// 출력된 IP주소 string을 str_arr에 복사
	printf("Dotted-Decimal notation 1: %s \n", str_ptr);
	
	inet_ntoa(addr2, sin_addr);
	printf("Dotted-Decimal notation 2: %s \n", str_ptr);
	printf("Dotted-Decimal notation 3: %s \n", str_arr);
	
	return 0;
}

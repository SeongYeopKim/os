#include<stdio.h>
#include<process.h>
#include<Windows.h>
unsigned WINAPI Thread_Func( void *arg);
int main(){
	int param = 5;
	unsigned threadID;
	printf("<main thread> started .\n");
	HANDLE hThread;
	DWORD	wr;

	//1.스레드 생성하기

	hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		Thread_Func,          //이 시점에서 이 함수가 실행된다. 스레드함수
		(void *)&param,			//스레드 함수용 파라메터
		0,
		&threadID
		);

	if(hThread == 0 ) {
		//스레드 생성실패
		printf("Failed craete thread\n");
		}

	//child 쓰레드 종료 대기

	wr = WaitForSingleObject(hThread, INFINITE); //무한히 기다린다.

	if(wr==WAIT_FAILED)
	{
		printf("대기실패\n");
		exit(1);
	}
	if(wr==WAIT_OBJECT_0){
		printf("쓰레드 정상 종료\n");
	}else {
		printf("time out\n");
	}

	printf("exit main thread");
	return 0;
}
 
unsigned WINAPI Thread_Func( void *arg){
	
	int index;
	int count;
	count = *(int *)arg;

	printf("<Child Thread> started\n ");
	for(index =0; index<count; index++){
		Sleep(1000);
		printf("%d \n",index);
	}

	return 0;

}
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

	//1.������ �����ϱ�

	hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		Thread_Func,          //�� �������� �� �Լ��� ����ȴ�. �������Լ�
		(void *)&param,			//������ �Լ��� �Ķ����
		0,
		&threadID
		);

	if(hThread == 0 ) {
		//������ ��������
		printf("Failed craete thread\n");
		}

	//child ������ ���� ���

	wr = WaitForSingleObject(hThread, INFINITE); //������ ��ٸ���.

	if(wr==WAIT_FAILED)
	{
		printf("������\n");
		exit(1);
	}
	if(wr==WAIT_OBJECT_0){
		printf("������ ���� ����\n");
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
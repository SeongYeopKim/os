//2010154008김성엽
//spn을 구현했습니다
//스케줄링하기 위해 포함되는 값들은 구조체에 입력했습니다.
//전체 시간흐름을 나타내기 위해 time_stream이라는 전역변수를 사용하였습니다

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define proc_time 20 //type 1일때 처리할 시간.
typedef struct priority_queue *queue_pointer;
struct priority_queue {
    int priority;
    int computing_time;
    int process_id;
    int start_time;
    int end_time;
    int temp_time;
    
    queue_pointer left_link, right_link;
};

typedef struct queue_head *head_pointer;
struct queue_head {  //큐의 헤드
    queue_pointer next;
};
int insert_queue(int process_id,int priority, int computing_time);  // 파일 입력함수
void print_process(struct priority_queue *end);
void pro_queue(int type);  //작업을 처리함.
struct queue_head pro; //큐의 헤드를 전역변수로 지정
int time_stream=0; //프로세스의 시점을 전역변수로 지정.
int nt_count=0;// Normalized Turnaround Time 계산.
double nt_time=0;
int main() {
    
    
    int type, prio,id, comt,res;
    
    FILE *pFile = NULL;
    
    
    pFile = fopen("input.txt","r");
    
    
    if(pFile !=NULL){
        printf("Queue_ID\tpriority\tcomputing_time\tturn_arround_time\n");
        while(1){
            res = fscanf(pFile,"%d%d%d%d",&type,&id,&prio,&comt);
            if(comt<0){
                printf("computing_time은 음수일 수 없습니다.\n");
                return 0;
            }
            
            //  printf("\n%d\n",id);
            if(res==-1) break;
            switch(type){
                case 0:
                    
                    insert_queue(id,prio,comt);
                    break;
                case 1:
                    if(id==0&& comt ==0 &&prio==0)
                        pro_queue(1);
                    else
                        printf("type 1에서 다른 값들이 입력되었습니다\n");
                    
                    break;
                case -1:
                    pro_queue(-1);
                    break;
                default:
                    printf("잘못된 type입력이 존재합니다.\n");
                    return 0;
            }
        }
        fclose(pFile);
    }
    else
        printf("입력 파일이 존재하지않습니다\n");
    printf("\n");
    
    
    return 0;
}




int insert_queue(int id,int priority, int computing_time){
    
    struct priority_queue *insert;  //삽일될 노드의 정보를 입력
    struct priority_queue *find;    //삽일될 노드의 위치를 찾기위한 노드
    
    
    
    insert = (struct priority_queue*)malloc(sizeof(struct priority_queue));
    insert->priority=priority;
    insert->computing_time = computing_time;
    insert->process_id  = id;
    insert->start_time = time_stream;
    insert->temp_time=computing_time;
    
    if(pro.next==NULL){      //안에비어있을때
        insert->left_link = NULL;         //오류방지위해 해드바로 밑에 노드의 left_link는 널을 가르키도록
        insert->right_link = NULL;
        pro.next = insert;
    }
    else if(pro.next!=NULL) {      //안이 비어있지 않을때
        if(time_stream%proc_time ==0 && time_stream/proc_time >1)
            find= pro.next->right_link;
        else find= pro.next;
        
        while(find->right_link!=NULL){         //입력값이 들어갈 자리 찾기
            if(find->computing_time>computing_time)
                break;
            find=find->right_link;}
        
        if(find->computing_time<computing_time){
            
            insert->left_link=find;
            insert->right_link=find->right_link;
            find->right_link=insert;
        }
        
        else if(find->computing_time<computing_time){
            pro.next->right_link=insert;
            insert->left_link=pro.next;
            insert->right_link=NULL;
        }
        
        else if(find->left_link!=NULL){            //찾은게 헤드 바로다음이 아닌지 확인
            insert->right_link= find;
            insert->left_link= find->left_link;
            find->left_link->right_link= insert;
            find->left_link=insert;
        }
        else if(find->left_link==NULL){                           //찾은게 헤드 바로 다음거면
            pro.next=insert;
            insert->left_link=NULL;
            insert->right_link=find;
            find->left_link=insert;
        }
    }
    
    return 0;
    
}
void pro_queue(int type){    //프로세스 처리힙니다.
    struct priority_queue *ready;
    
    ready = pro.next;
    
    
    
    int res_time=0; // 프로세스 처리중 남은 시간.
    switch (type) {
        case 1:
            if(ready!=NULL){
                if(ready->computing_time<=proc_time)
                {
                    
                    time_stream=time_stream+ready->computing_time;
                    ready->end_time = time_stream;
                    res_time = proc_time-ready->computing_time;
                    ready->computing_time=0;
                    
                    print_process(ready);
                    pro.next=ready->right_link;
                    ready->right_link->left_link=NULL;
                    ready = pro.next;
                   
                    while(res_time>0){          //남은 시간이 없어질때까지 다른 프로세스들을 수행한다.
                        if(ready->computing_time<=res_time){    //남은처리시간 보다 적은 처리시간을 가진 사람이 있다면,
                            time_stream=time_stream+ready->computing_time;
                            ready->end_time=time_stream;
                            res_time = res_time-ready->computing_time;
                            ready->computing_time=0;
                            
                            print_process(ready);
                            pro.next=ready->right_link;
                            ready->right_link->left_link =NULL;
                            
                        }else{ //
                            time_stream= time_stream+res_time;
                            res_time=0;
                            ready->computing_time=ready->computing_time-res_time;
                            
                            
                        }
                        
                    }
                    
                    
                }else{
                    time_stream=time_stream+proc_time;
                    ready->computing_time=ready->computing_time-proc_time;
                    if(ready->computing_time==0){
                        ready->end_time = time_stream;
                        print_process(ready);
                    }
                    
                    
                    
                }
                
            }
            
            break;
            
        case -1:  //모든 입력이 끝나고 프로그램이 끝나기 전의 시점에 남은 프로세스의 작업을 완료한다.
            
            while(ready!=NULL){  //남는게 없을 때까지 작업을 수행한다.
                
                time_stream +=ready->computing_time;
                ready->computing_time=0;
                ready->end_time = time_stream;
                print_process(ready);
                pro.next=ready->right_link;
                if(ready->right_link!=NULL){ //헤드의 2번째 노드가 남아있다면 바뀐 노드의 순서를 해준다.
                    ready->right_link->left_link=NULL;
                    ready = pro.next;
                }
                else break;
                
                
            }
            printf("Nobalized Turnaround Time : %lf \n ",nt_time/nt_count);
            //종료 직전 Nobalized Turnaround Time  을 출력한다
            
            
            break;
            
    }
    
    
    
}




void print_process(struct priority_queue *end){ //프로세서의 정보들을 출력합니다.
    printf("%d\t\t\t",end->process_id);
    printf("%d\t\t\t" ,end->priority);
    printf("%d\t\t\t\t %d\t \n ",end->temp_time,end->end_time-end->start_time);

    nt_count++;
    nt_time += ((end->end_time-end->start_time)/(double)end->temp_time);
    }


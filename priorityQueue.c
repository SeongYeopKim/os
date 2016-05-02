/*
-
-
-
-
-
입력으로 priority 필드와 computing_time 필드를 받아 priority queue를 관리하는 프 로그램을 작성한다.
priority queue는 3개의 queue로 분리되며, priority 값이 1 - 10사이가 가장 높은 단 계의 queue(Queue ID = 1)에서 관리된다. Priority 값이 11 - 20사이는 중간 단계의 queue(Queue ID = 2)에서 관리되며, priority 값이 21 – 30인 경우에는 가장 낮은 단계의 queue(Queue ID =3)에서 관리된다.
각 queue에서는 priority 가 높을수록(숫자가 적을수록 priority 가 높다) head 가까이 에 위치된다.
priority queue 초기화 function(initialize_queue())은 queue의 초기화 기능을 수행한 다.
priority queue 삽입 function(insert_queue())은 priority에 따라서 원소를 적절한
queue에 삽입한다.
- priority queue 삭제 function(delete_queue())은 해당 queue에서 priority 보다 높거나
같은 priority 값을 가지는 원소 중 최상위 priority 값을 가지는 원소를 삭제하며, Queue ID, priority 값과 computing_time을 출력한다. (삭제를 원하는 priority와 같은 원소를 삭제하는 것이 아님을 유의하기 바람)
- 입력은 input_file에서 읽어오며, {type, priority, computing_time} 필드를 입력으로 가 진다. 이때 type 값이 0이면 insert, 1이면 delete를 의미한다. 그리고 type -1은 입력 이 완료되었음을 의미한다.
- 입력 완료 이후에는 현재 priority queue의 3개 queue의 모든 원소들을 priority가 높 은 것부터 출력하는 function(print_queue())을 이용하여 각 원소가 속한 Queue ID, priority 값과 computing_time을 출력한다.
*/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct priority_queue *queue_pointer;
typedef struct priority_queue {
   int priority;
   int computing_time;
   queue_pointer left_link, right_link;
};

typedef struct queue_head *head_pointer;
struct queue_head {
   queue_pointer left_link;
   queue_pointer right_link;
};
void initialize_queue(void);
int insert_queue(int priority, int computing_time);
int delete_queue(int priority, int computing_time);
void print_queue(void);
int search_level(int priority);
struct queue_head lv[3];

int main() {
   struct priority_queue *main_queue;
   char buf_in[255];
   char *pToken = NULL;
   int type, prio, comt,res,count=0;
   char ch;
   FILE *pFile = NULL;

   initialize_queue();

   pFile = fopen("input.txt","r");

   printf("\tQueue ID  priority  computing_time\n");
   if(pFile !=NULL){

      while(1){
         res = fscanf(pFile,"%d%d%d",&type,&prio,&comt);
         if(res==-1) break;
         switch(type){
         case 0: 
            insert_queue(prio,comt);
            break;
         case 1:
            delete_queue(prio,comt);
            break;

         }
         count++;
      }
      fclose(pFile);
      print_queue();
   }
   else
      printf("涝仿 颇老捞 粮犁窍瘤臼嚼聪促");
   printf("\n");


   return 0;
}

void initialize_queue() {
   int i;
   for(i =0; i<3;i++){


      lv[i].left_link=NULL;
      lv[i].right_link=NULL;
   }
}

int search_level(int priority)
{
   int find_lv;
   if(priority>0&&priority<11) {
      find_lv = 0;
   }
   else if(priority>10&&priority<21) {
      find_lv= 1;
   }
   else if(priority>20&&priority<31) {
      find_lv=2;
   }
   else {

      exit(-1);
      printf("涝仿坷幅");
   }
   return find_lv; 
}

int insert_queue(int priority, int computing_time){
   int find_lv;
   struct priority_queue *insert;
   struct priority_queue *find;

   find_lv= search_level(priority);

   insert = (struct priority_queue*)malloc(sizeof(struct priority_queue));
   insert->priority=priority;
   insert->computing_time = computing_time;

   if(lv[find_lv].right_link==NULL){      //救俊厚绢乐阑锭
      insert->left_link = NULL;         //坷幅规瘤困秦 秦靛官肺 关俊 畴靛狼 left_link绰 澄阑 啊福虐档废
      insert->right_link = NULL;
      lv[find_lv].right_link= insert;
   }
   else if(lv[find_lv].right_link!=NULL) {      //救捞 厚绢乐瘤 臼阑锭 

      find= lv[find_lv].right_link;
      while(find->right_link!=NULL){         //涝仿蔼捞 甸绢哎 磊府 茫扁
         if(find->priority>priority)
            break;
         find=find->right_link;}

      if(find->priority<priority){

         insert->left_link=find;
         insert->right_link=find->right_link;
         find->right_link=insert;
      }

      else if(find->priority<priority){
         lv[find_lv].right_link->right_link=insert;
         insert->left_link=lv[find_lv].right_link;
         insert->right_link=NULL;
      }

      else if(find->left_link!=NULL){            //茫篮霸 庆靛 官肺促澜捞 酒囱瘤 犬牢
         insert->right_link= find;
         insert->left_link= find->left_link;
         find->left_link->right_link= insert;
         find->left_link=insert;
      }
      else if(find->left_link==NULL){                           //茫篮霸 庆靛 官肺 促澜芭搁
         lv[find_lv].right_link=insert;
         insert->left_link=NULL;
         insert->right_link=find;
         find->left_link=insert;
      }
   }



   return 0;

}
int delete_queue(int priority,int computing_time){
   int find_lv,mode=0;
   struct priority_queue *find;

   find_lv=search_level(priority);
   find = lv[find_lv].right_link;

   while(find->right_link!=NULL){
      find=find->right_link;
      if(find->priority==priority &&find->computing_time ==computing_time){
         mode=1;
         break;
      }
      else mode=2;
   }
   find= lv[find_lv].right_link;
   if(find->priority==priority)
      mode=1;

   switch(mode){
   case 1:
      find = lv[find_lv].right_link;
      lv[find_lv].right_link=find->right_link;
      find->right_link->left_link= NULL;
      printf("\t\t%d\t%d\t%d\n",find_lv+1, find->priority,find->computing_time);
      free(find);

      break;
   case 2:
      printf("delete窃荐 俊矾, 涝仿茄 priority啊 粮犁窍瘤 臼嚼聪促.");
      break;
   }



   return 0;
}

void print_queue(){
   struct priority_queue *final;
   int i;
   for(i =0; i<3; i++) {


      if(lv[i].right_link!=NULL){
         final=lv[i].right_link;
         while(final->right_link!=NULL){
            printf("\t\t%d\t", i+1);
            printf("%d\t",final->priority);
            printf("%d\t\n",final->computing_time);
            final=final->right_link;
         }
         printf("\t\t%d\t", i+1);
         printf("%d\t",final->priority);
         printf("%d\n",final->computing_time);
      }


  

   }

}
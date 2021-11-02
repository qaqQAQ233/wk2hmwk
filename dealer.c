#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include "common.h"
#include <pthread.h>
#include <time.h>
#include <unistd.h>
struct cards{
	char name[40];
	int number;
};
struct cards pc[52];

int random_array[52];

//此变量跟踪从牌组中拉出的牌
int global_track = 0;


void timere(){
	int i = 15;
	int a;
	printf("game will start soon\n");
	fflush(stdout);
    while (i >= 0 )
    {	if(i>=10){
	printf("%d",i);	
	fflush(stdout);
	
        sleep(1);
	
	printf("\b\b");
	}	
	else{
	printf("0%d",i);
	fflush(stdout);
	
        sleep(1);
	printf("\b\b");
	}
        i--;
    }
	printf("\ngame start,good luck!\n");



}

void Initialize_random_array(){
	int i;
	for(i=0;i<52;i++){
		random_array[i] = i;
	}
	
	srand(time(NULL));

	//洗牌
	for(i=0;i<52;i++){
		int j = rand() % (i+1);

		int temp;
		temp = random_array[i];
		random_array[i] = random_array[j];
		random_array[j] = temp;	
	}
}
void Initialize_card(){
	int i;
	int j;
	j = 1;
	for(i=0;i<52;i++){
		if(i>=0 && i<=12){//同花色13张
			strcpy(pc[i].name,"黑桃");
			pc[i].number = j;
			j++;
			if(i==12){
				j=1;
			}
		}

		else if(i>=13 && i<=25){
			strcpy(pc[i].name,"红桃");
			pc[i].number = j;
			j++;
			if(i==25){
				j=1;
			}
		}

		else if(i>=26 && i<=38){
			strcpy(pc[i].name,"方块");
			pc[i].number = j;
			j++;
			if(i==38){
				j=1;
			}
		}

		else if(i>=39 && i<=51){
			strcpy(pc[i].name,"梅花");
			pc[i].number = j;
			j++;
			if(i==51){
				j=1;
			}
		}

	}
}

int clientFd[MAX_PLAYERS];

void start_game(int players){
	int j;

	//初始化牌并让庄家选择两张牌
	Initialize_card();
	int i;

	for(i=0;i<MAX_PLAYERS;i++){
		if(clientFd[i]>0){
			char buffer[BUFFER_SIZE];
			int nwritten;
			strcpy(buffer,"欢迎来到游戏，庄家正在选择他的牌 ....... \n");
			if (BUFFER_SIZE != (nwritten = write(clientFd[i], buffer, BUFFER_SIZE)))
			{
				printf("Error! Couldn't write to player \n");
				//close(clientFd[i]);
				return;
			}
		}
	}
	/* 庄家在这里选牌 */
	Initialize_random_array();

	//选择两张牌
	int total = 0 ;
	int n1 = random_array[global_track];
	global_track++;
	int n2 = random_array[global_track];
	global_track++;
	total = total + pc[n1].number + pc[n2].number;

	//传递牌的大小
	char name1[100];
	char name2[100];

	//传递牌的花色
	strcpy(name1,pc[n1].name);
	strcpy(name2,pc[n2].name);

	printf("%d %s\n",pc[n1].number,name1);
	printf("%d %s\n",pc[n2].number,name2);
	char buffer[BUFFER_SIZE];
	
	//strcat(buffer,name1);
	int nwritten;

	int temp;
	temp = global_track;
	
    /////////////////////// 第一位玩家 /////////////////////////

    int n_first = 0;
    strcpy(buffer,"您是第一位玩家, press HIT/STAND \n");
	if (BUFFER_SIZE != (nwritten = write(clientFd[0], buffer, BUFFER_SIZE)))
			{
				printf("Error! Couldn't write to player \n");
				//close(clientFd[i]);
				return;
	}

	 if (0 > (nwritten = read(clientFd[0], buffer, BUFFER_SIZE))){
            /* error("Error reading from client"); */
            printf("来自socket的响应超时\n");
            
     }
     else{
     	while(1){
	     	if(strcmp(buffer,"HIT")==0){
	     		n_first++;
	     		printf("\n玩家1选择 HIT, you have another chance!\n");


	     		 if (0 > (nwritten = read(clientFd[0], buffer, BUFFER_SIZE))){
                  /* error("Error reading from client"); */
                  printf("来自socket的响应超时\n");
            
            }

	     }
	     else{
	     		char buffer1[300];
	     		strcpy(buffer,"你的回合结束，请等待结果\n");
	     		strcat(buffer,"你拿的牌\n");
	     		int l=0;
	     		for(l=temp;l<temp+n_first;l++){
	     			strcat(buffer,pc[random_array[l]].name);
	     			strcat(buffer," ");
	     			sprintf(buffer1,"%d",pc[random_array[l]].number);
	     			strcat(buffer,buffer1);
	     			strcat(buffer,"\n");
	     		}
	     		int nwritten;

	     		if (BUFFER_SIZE != (nwritten = write(clientFd[0], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					//close(clientFd[i]);
					return;
	            }

	     		break;
	     	}
        }
     }

     printf("玩家1选择 %d  cards\n",n_first);

	//////////////////////// FOR THE second client ///////////////////////

    strcpy(buffer,"玩家1拿牌完毕, 现在是你的回合 press HIT/STAND!\n");
	int n_second = 0;
    strcat(buffer,"您是第二位玩家, press HIT/STAND \n");
	if (BUFFER_SIZE != (nwritten = write(clientFd[1], buffer, BUFFER_SIZE)))
			{
				printf("Error! Couldn't write to player \n");
				//close(clientFd[i]);
				return;
	}

	 if (0 > (nwritten = read(clientFd[1], buffer, BUFFER_SIZE))){
            /* error("Error reading from client"); */
            printf("来自socket的响应超时\n");
            
     }
     else{
     	while(1){

	     	if(strcmp(buffer,"HIT")==0){
	     		n_second++;
	     		printf("\n玩家2选择 HIT, you have another chance!\n");
	     		 if (0 > (nwritten = read(clientFd[1], buffer, BUFFER_SIZE))){
                  /* error("Error reading from client"); */
                  printf("来自socket的响应超时\n");
            
            }

	     	}
	     	else{

	     		char buffer1[300];
	     		strcpy(buffer,"你的回合结束，请等待结果\n");
	     		strcat(buffer,"你拿的牌\n");
	     		int l=0;
	     		for(l=temp+n_first;l<temp+n_first+n_second;l++){
	     			strcat(buffer,pc[random_array[l]].name);
	     			strcat(buffer," ");
	     			sprintf(buffer1,"%d",pc[random_array[l]].number);
	     			strcat(buffer,buffer1);
	     			strcat(buffer,"\n");
	     		}
	     		int nwritten;
	     		if (BUFFER_SIZE != (nwritten = write(clientFd[1], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					//close(clientFd[i]);
					return;
	            }

	     		break;
	     	}


        }
     }

     printf("玩家2选择 %d  cards\n", (n_second));

     //////////////////////// For the third client /////////////////////////////

    strcpy(buffer,"玩家2发牌完毕!现在是你的回合 press HIT/STAND!\n");
	int n_third = 0;
    strcat(buffer,"您是第三位玩家, press HIT/STAND \n");
	if (BUFFER_SIZE != (nwritten = write(clientFd[2], buffer, BUFFER_SIZE)))
			{
				printf("Error! Couldn't write to player \n");
				//close(clientFd[i]);
				return;
	}

	 if (0 > (nwritten = read(clientFd[2], buffer, BUFFER_SIZE))){
            /* error("Error reading from client"); */
            printf("来自socket的响应超时\n");
            
     }
     else{
     	while(1){
	     	if(strcmp(buffer,"HIT")==0){
	     		n_third++;
	     		printf("\n玩家3选择 HIT, you have another chance!\n");
	     		 if (0 > (nwritten = read(clientFd[2] , buffer, BUFFER_SIZE))){
                  /* error("Error reading from client"); */
                  printf("来自socket的响应超时\n");
            
            }

	     	}
	     	else{
	     		char buffer1[300];
	     		
	     		strcpy(buffer,"你的回合结束，请等待结果\n");
	     		strcat(buffer,"你拿的牌\n");
	     		int l=0;
	     		for(l=temp+n_first+n_second;l<temp+n_first+n_second+n_third;l++){
	     			strcat(buffer,pc[random_array[l]].name);
	     			strcat(buffer," ");
	     			sprintf(buffer1,"%d",pc[random_array[l]].number);
	     			strcat(buffer,buffer1);
	     			strcat(buffer,"\n");
	     		}
	     		int nwritten;
	     		if (BUFFER_SIZE != (nwritten = write(clientFd[2], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					//close(clientFd[i]);
					return;
	            }

	     		break;
	     	}

        }
     }

     printf("玩家3选择 %d  cards\n",(n_third));

     int total1 = 0;
     int total2 = 0;
     int total3 = 0;

     int m;
     int t;
     for(m=global_track;m<global_track+n_first;m++){

     	total1 = total1 + pc[random_array[m]].number;
     	//printf("%d",pc[m].number);
     }

     t = m;
     for(m=t;m<global_track + n_first+n_second;m++){

     	total2 = total2 + pc[random_array[m]].number;
        //printf("%d",pc[m].number);
     }

     t = m;
     for(m=t;m< global_track + n_first+n_second+n_third;m++){

     	total3 = total3 + pc[random_array[m]].number;
        //printf("%d",pc[m].number);
     }

     ///////////////////  Conditions for Scoring  ////////////////////

     int arr[3];
     arr[0] = 0;
     arr[1] = 0;
     arr[2] = 0;

     if(total1 > 21){
     	//printf("User 1 loses \n");
     	arr[0] = 1;
     }

     if(total2 > 21){
     	//printf("User 2 loses \n");
     	arr[1] = 1;
     }

     if(total3 >21){
     	//printf("User 3 loses \n");
     	arr[2] = 1;
     }

     int b;
     for(b=0;b<3;b++){
     	if(arr[b]==1){
     		printf("玩家 %d 由于超过21点失败 ! ",b+1);
     	}
     }

     int min;

     int win[3];
     win[0] = 0;
     win[1] = 0;
     win[2] = 0;


     if(total1>=21 && total2>=21 && total3>=21){
     	win[0] = 0;
     	win[1] = 0;
     	win[2] = 0;
     }

     int flag=0;

     if(total1<21 || total2<21 || total3<21){
     	//The minimum difference has to be calculated

     	//Find the minimum amongst the three

     	if(total1>21){
     		printf("玩家1 score : 0\n");
     	}
     	else{
     		printf("玩家1 score : %d\n",total1);
     	}
     	if(total2>21){
     		printf("玩家2 score : 0\n");
     	}
     	else{
     		printf("玩家2 score : %d\n",total2);
     	}
     	if(total3>21){
     		printf("玩家3 score : 0\n");
     	}
     	else{
     		printf("玩家3 score : %d\n",total3);
		}

		if (total1<=21){
     		if(total1>=total || total>21){
     			printf("玩家1 Wins!");
     			win[0] = 1;    		
     			flag=1;
     		}
     	}
     	else{
     		if(total>21 && total1<total){
     			printf("玩家1 Wins!");
     			win[0] = 1;
     			flag=1;
     		}
     	}

     	if (total2<=21){
     		if(total2>=total || total>21){
     			printf("玩家2 Wins!");
     			win[1] = 1; 
     			flag=1;   		
     		}
     	}
     	else{
     		if(total>21 && total2<total){
     			printf("玩家2 Wins!");
     			win[1] = 1;
     			flag=1;
     		}
     	}

     	if (total3<=21){
     		if(total3>=total || total>21){
     			printf("玩家3 Wins!");
     			win[2] = 1;  
     			flag=1;  		
     		}
     	}
     	else{
     		if(total>21 && total3<total){
     			printf("玩家3 Wins!");
     			win[2] = 1;
     			flag=1;
     		}
     	}

     	if(flag==0)
     	{
     		printf("庄家获胜");
     	}
     	for(b=0;b<3;b++){
     		if(win[b]==1){
     			//This player wins
     			strcpy(buffer,"你赢了!\n");
     			int nwritten;
     			if (BUFFER_SIZE != (nwritten = write(clientFd[b], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					return;
	            }

     		}
     		else{
     			//This player loses
     			strcpy(buffer,"你输了!\n");
     			int nwritten;
     			if (BUFFER_SIZE != (nwritten = write(clientFd[b], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					return;
	            }
     		}
     	}
     }
	 printf("\ndealer will close fd!\n");
     close(clientFd[0]);
     close(clientFd[1]);
     close(clientFd[2]);
printf("\ndealer  close fd!\n");
	 
     //关闭所有连接 -- Both Client and Server Connection -- 
}
int main(int argc, char *argv[]){

	int i;
	for(i=0;i<argc-1;i++){
		//Initializing the socket numbers
		clientFd[i] = atoi(argv[i+1]);		
	}

	start_game(MAX_PLAYERS);
    printf("\nreturn to server\n");
	//timere();


    return 0;
}

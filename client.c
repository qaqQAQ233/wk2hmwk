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
#include <sys/select.h>



void play(int sock){
    
    printf("等待游戏开始....\n");
    while(1){
        int n;
        char buffer[BUFFER_SIZE];
        if (0 > (n = read(sock, buffer, BUFFER_SIZE)))
        {
            /* error("Error reading from client"); */
            printf("来自socket的响应超时\n");
            
        }
        else{
            printf("%s\n",buffer);
        }

         if (0 > (n = read(sock, buffer, BUFFER_SIZE)))
        {
            /* error("Error reading from client"); */
            printf("来自socket的响应超时\n");
            
        }
        else{
            printf("%s\n",buffer);
        }
        
        //Now take the input from the user        
        int d;
        printf("输入 1 拿牌 或 其他任意键 停牌\n");
        
        while(1){
            scanf("%d",&d);
            if(d==1){

                int nwritten;
                strcpy(buffer,"HIT");
                printf("发送中...%s\n",buffer);
                if (BUFFER_SIZE != (nwritten = write(sock, buffer, BUFFER_SIZE)))
                        error("hit无法写入服务器");
            }
            else{
                strcpy(buffer,"STAND");
                int nwritten;
                printf("发送中....%s\n",buffer);
                if (BUFFER_SIZE != (nwritten = write(sock, buffer, BUFFER_SIZE)))
                        error("stand无法写入服务器");

                break;
            }

        }
         if (0 > (n = read(sock, buffer, BUFFER_SIZE)))
        {
            /* error("Error reading from client"); */
            printf("来自socket的响应超时\n");
            
        }
        else{
            printf("\n %s \n",buffer);
        }
            if (0 > (n = read(sock, buffer, BUFFER_SIZE)))
        {
            /* error("Error reading from client"); */
            printf("来自socket的响应超时\n");
            
        }
        else{
            printf("\n %s \n",buffer);
        }

        break;
    }
}

int main(int argc, char *argv[])
{

    int sockfd = 0, n = 0;
	int regame=0;    
	char recvBuff[1024]={};
    struct sockaddr_in serv_addr; 
	while(1){
	
	sockfd = 0;
	 n = 0;
	memset(recvBuff,0,sizeof(recvBuff));
    if(argc != 2)
    {
        printf("\n Usage: %s <服务器IP> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : 无法创建 socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton 发生错误\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

   else{
     char buffer[BUFFER_SIZE];
     int nwritten;
     printf("已连接至服务器 -- \n");
     play(sockfd);
   }
	printf("try again?\n1=cnt && 0=log_out\n");
	scanf("%d",&regame);
	if(regame==0)
	return 0;
	

	}
    return 0;
}

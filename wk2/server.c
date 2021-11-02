#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include "common.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


int main(int argc, char *argv[])
{
    int recode;
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //初始化套接字
    if(listenfd<0){
        error("Error creating socket\n");
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(9999); 

    //绑定设置监听
    if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0)
    {
      
      error("Error binding to socket\n");
    } 

    listen(listenfd, 5); //最多5

    int maxfd = listenfd;
    fd_set rdset;
    fd_set rdtemp;
    // 清零
    FD_ZERO(&rdset);
    // 将监听的lfd设置到检测的读集合中
    FD_SET(listenfd, &rdset);




    while(1)
    {



/*      rdtemp = rdset;
        int num = select(maxfd+1, &rdtemp, NULL, NULL, NULL);

        if(FD_ISSET(listenfd, &rdtemp))
        {

            int clientSockFd[MAX_PLAYERS];
            struct sockaddr_in clientAddress;
            socklen_t clientLen = sizeof(clientAddress);
            printf("\n服务器等待开始新游戏.....\n\n");
            clientSockFd[0] = accept(listenfd, (struct sockaddr*)&clientAddress, &clientLen);


*/
        int rec[MAX_PLAYERS];
        rdtemp = rdset;
        int num = select(maxfd+1, &rdtemp, NULL, NULL, NULL);
        
        int pid;
        if(FD_ISSET(listenfd, &rdtemp))
        {

            int clientSockFd[MAX_PLAYERS];
            struct sockaddr_in clientAddress;
            socklen_t clientLen = sizeof(clientAddress);

            printf("\n服务器等待开始新游戏.....\n\n");
            clientSockFd[0] = accept(listenfd, (struct sockaddr*)&clientAddress, &clientLen);
            if(clientSockFd[0]<0)
            {
                error("Error accepting first client");
            } 
            else
            {
                printf("第一位玩家已连接 %d\n",clientSockFd[0]);
                int number_of_clients = 1;//记录玩家数
                while(number_of_clients<MAX_PLAYERS)//——
                {
                    printf("等待下一位玩家连接 \n \n");
                    if((clientSockFd[number_of_clients]=accept(listenfd, (struct sockaddr*)&clientAddress, &clientLen))<0)
                    {
                        error("Error accepting next client\n");
                    }
                    else{
                        printf("下一位玩家已连接 clientSockFd:%d \n",clientSockFd[number_of_clients]);
                    }
    
                    number_of_clients++;
                }
                recode=number_of_clients;
            }
            int number_of_clients=recode;

            FD_SET(clientSockFd[number_of_clients], &rdset);
            // 重置最大的文件描述符
            maxfd = clientSockFd[number_of_clients] > maxfd ? clientSockFd[number_of_clients] : maxfd;
            memcpy(rec,clientSockFd,sizeof(clientSockFd));
        }

        // 没有新连接, 通信

            int number_of_clients=recode;

            // 判断从监听的文件描述符之后到maxfd这个范围内的文件描述符是否读缓冲区有数据

                        //为每个client创建一个socket
                int clientSockFd[MAX_PLAYERS];
                memcpy(clientSockFd,rec,sizeof(rec));
                printf("游戏开始 \n");
                //At this stage all the players in the game are connected
                //At this stage -- Create a new process
                switch(pid=fork())
                {
                    case -1: error("新进程无法建立");
                    case 0:
                    {
                        //New process created:
                        char arguments[MAX_PLAYERS][300];
                        char *args[MAX_PLAYERS+2];
                        strcpy(arguments[0],"./dealer");
                        args[0] = arguments[0];
                        int i;
                        for(i=0;i<number_of_clients;i++)
                        {
                            sprintf(arguments[i+1],"%d",clientSockFd[i]);
                            args[i+1] = arguments[i+1];
                        }
                        for(i=number_of_clients+1;i<=MAX_PLAYERS+1;i++)
                        {
                            args[i] = 0;
                        }
                        execv("./dealer",args);
                        perror("Error ");
                        error("exec didn't succeed");
			//sleep(30);
                    }
                    default:
                    {
                        break;
                    }

                }
		printf("server will close fd!\n");
            close(clientSockFd[0]);
            close(clientSockFd[1]);
            close(clientSockFd[2]);
	    printf("server  close fd!\n");
            //printf("\n服务器等待开始新游戏22.....\n\n");

        





        
    



    //break;
	
    sleep(30);
	printf("\n服务器等待开始新游戏.....\n\n");
    }

    //close(listenfd);
    //exit(0);
    return 0;
}

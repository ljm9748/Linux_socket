/* client.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCKSIZE 100000
void error_handling(char *message);

int main(int argc, char **argv)
{
        int sock;
        struct sockaddr_in serv_addr;
		    char message[30];
        char buffer[SOCKSIZE]={0};
				int str_len_tocli;
        char new_message[30]="Hello Server!!";
        char file_name[1020]="";
        


        if(argc != 3) {
               printf("Usage : &s <IP>\n", argv[0]);
               exit(1);
        }

        sock = socket(PF_INET, SOCK_STREAM, 0); /* 서버 접속을 위한 소켓 생성 */
        if(sock == -1)
            error_handling("socket() error");

        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
        serv_addr.sin_port = htons(atoi(argv[2]));

		if( connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
            error_handling("connect() error");

      //hello server 전송
        write(sock, new_message, sizeof(new_message)); 

       //hello client 수신  
        str_len_tocli = read(sock, message, sizeof(message)-1); 
         
        if(str_len_tocli == -1)
               error_handling("read() error");
				message[str_len_tocli]=0;
				printf("Message from server: %s \n", message);

      //영상수신
        FILE *recvfile= fopen("/mnt/c/SKKU/2020 1학기/Linux/termpj/client/copyfile.mp4","wb");
        
        int tmp=read(sock, buffer, SOCKSIZE);
        fwrite(buffer, sizeof(char), tmp, recvfile);

        while(tmp!=0){
        tmp=read(sock, buffer, SOCKSIZE);
        //printf("tmp:%d\n", tmp);
        fwrite(buffer, sizeof(char), tmp, recvfile);
        }



        close(sock);      // 연결 종료

        return 0;
}

void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}

/* 컴퓨터정보과 A반 202044020-이가온 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h> //쓰레드 사용을 위해 선언

#define BUF_SIZE 100
#define NAME_SIZE 20

void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);

char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[])
{
        int sock;
        struct sockaddr_in serv_addr;
        pthread_t snd_thread, rcv_thread;
        void * thread_return;
        if(argc!=4) { //실행파일 경로/IP/port번호/채팅닉네임 입력으로 받아야 함
                 printf("Usage : %s <IP> <port> <name> \n", argv[0]);
                 exit(1);
        }

        sprintf(name, "[$s]", argv[3]);
        sock=socket(PF_INET, SOCK_STREAM, 0); //TCP 소켓 생성
 
  /* 서버 주소정보 초기화 */
        memset(&serv_addr, 0,sizeof(serv_addr));
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
        serv_addr.sin_port=htons(atoi(argv[2]));

        if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
                error_handling("connect() error");
  
   /* 쓰레드 생성 및 실행 */
        pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
        pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);

        /* 쓰레드 종료까지 대기 */
        pthread_join(snd_thread, &thread_return);
        pthread_join(rcv_thread, &thread_return);
        close(sock);//클라이언트 소켓 연결 종료
        return 0;
}
void * send_msg(void * arg) // send thread main
{
        int sock=*((int*)arg);//클라이언트의 파일 디스크립터
        char name_msg[NAME_SIZE+BUF_SIZE];
        while(1)
        {
                fgets(msg, BUF_SIZE, stdin);
                if(!strcmp(msg, "q\n")||!strcmp(msg, "Q\n"))
                {
                        close(sock);//틀라이언트 소켓 연결종료 후
                        exit(0);//프로그램 종료
                }
 sprintf(name_msg, "%s %s", name, msg); //client 이름과 msg를 합침
               write(sock, name_msg, strlen(name_msg));//널문자 제외하고 서버>    로 문자열을 보냄
        }
        return NULL;
}
void * recv_msg(void * arg) //read thread main
{
        int sock=*((int*)arg);//클라이언트의 파일 디스크립터
        char name_msg[NAME_SIZE+BUF_SIZE];
        int str_len;
        while(1)
        {
                str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
                if(str_len==-1)//read 실패시
                        return (void*)-1;
                name_msg[str_len]=0;
                fputs(name_msg, stdout);
        }
        return NULL;
}

void error_handling(char *msg)
{
        fputs(msg, stderr);
        fputc('\n', stderr);
        exit(1);
}

  
  

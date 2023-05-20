#ifndef MYHRF_H
#define MYHRF_H
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/select.h>
#include<poll.h>
#include<signal.h>
#include<pthread.h>
#define MAX_CMDLEN 5000
#define MAX_TKLEN 256
#define NUM_BULINCMD 10
#define NUM_SHELLCMD 10
typedef struct {
    int ID;
    char name[12];
    char ip[20];
    int port;
    int connfd;
    pid_t childpid;
} who_info;
extern who_info who_list[10];
extern HISTORY_STATE *hist_state;
extern HIST_ENTRY **hist_list;
extern char *buf_point[MAX_CMDLEN];
extern char **tokenlist[MAX_CMDLEN];

extern char *bulincmd_list[NUM_BULINCMD];
extern char *shellcmd_list[NUM_SHELLCMD];
extern char ***argvptr;
extern char ***argvnumptr;
extern int num_of_token;
extern int bulin_flag;
extern int shell_flag;
extern int num_of_pipe;//紀錄有幾個一般pipe
extern int is_pipe;// 確認讀入的cmd有沒有pipe
extern int numpipe_num;
extern int numpipe_enable;
extern int if_unknown;
extern int unknown_count;
extern char unknown_list[MAX_TKLEN];
extern int cmd;
extern int numpipe_index;
extern char *buf_copy[MAX_CMDLEN];
extern int unk_bet_pipe;
extern char *fifo_path;
extern int fd_Wp;
extern int fd_Wb;
extern int fd_We;
extern int num_who;
extern char mes_fifo_path[100];
#endif
// 無窮迴圈 等input
#include"../include/myhrf.h"
#include"../include/ntools.h"
#include"../include/mysig.h"
char *buf_point[MAX_CMDLEN];
char **tokenlist[MAX_CMDLEN];
char *bulincmd_list[NUM_BULINCMD];
char *shellcmd_list[NUM_SHELLCMD];
char ***argvptr;
int num_of_token;
int bulin_flag;
int shell_flag;
int num_of_pipe;//紀錄有幾個一般pipe
int is_pipe;// 確認讀入的cmd有沒有pipe
int numpipe_num;
int numpipe_enable;
int numpipe_index;
int unk_bet_pipe;
char ***argvnumptr;
int if_unknown;
int unknown_count;
char unknown_list[MAX_TKLEN];
int cmd;
char *buf_copy[MAX_CMDLEN];
char *fifo_path;
int fd_Wp;
int fd_Wb;
int fd_We;
int num_who = 0;
int name_sig;
char mes_fifo_path[100];
int ifmsg;
HISTORY_STATE *hist_state;
HIST_ENTRY **hist_list;
ssize_t readn(int filedes, void *buff, size_t nbytes);
ssize_t writen(int filedes, const void *buff, size_t nbytes);
ssize_t readlinen(int filedes, void *buff, size_t maxlen);
who_info who_list[10];
void sigchld_handler(int sig) {
    int saved_errno = errno;
    pid_t end_pid;
    while((end_pid = waitpid((pid_t)(-1),0,WNOHANG))>0) {
        printf("sigchld_handler %d\n", end_pid);
        print_who_info_for_parent(&who_list);
        int index=-1;
        for(int i=0;i<num_who;i++){
            if(who_list[i].childpid == end_pid){
                index = who_list[i].ID;
                break;
            }
        }
        printf("deleteindex = %d\n", index);
        printf("now num_who = %d\n", num_who);
        num_who = remove_who_info(&who_list, index);
        print_who_info_for_parent(&who_list);
        printf("exit num_who = %d\n", num_who);
        char del_mesfifo[100];
        sprintf(del_mesfifo, "%dmes_fifo", end_pid);
        unlink(del_mesfifo);
    }
    errno = saved_errno;
}
void set_signal_action(void)
{
	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_handler = &sigchld_handler;
	sigaction(SIGCHLD, &act, NULL);
}
void whosignalHandler(int signal) {
    // 在信号处理程序中进行相应的操作
    // 这里示例将数据写入FIFO
    int saved_STDOUT = dup(STDOUT_FILENO);
    int fifo_fdW = open("who_fifo", O_WRONLY);
    dup2(fifo_fdW, STDOUT_FILENO);
    print_who_info(who_list);
    close(fifo_fdW);
    dup2(saved_STDOUT, STDOUT_FILENO);
    
}
void name_handler(int sig){
    // printf("name_handler\n");
    int fifo_fd = open("name_fifo", O_RDONLY);
    char buf[1024];
    int n;
    while((n = read(fifo_fd, buf, sizeof(buf)))>0);
    close(fifo_fd);
    int childpid;
    char newname[100];
    char *name = strtok(buf, "|");
    int ind=0;
    while(name != NULL){ //childpid|newname
        if(ind ==0)
            childpid = atoi(name);
        else if(ind == 1)
            strcpy(newname, name);
        ind++;
        name = strtok(NULL, "|");
    }

    for(int i=0;i<num_who;i++){
        if(who_list[i].childpid == childpid){
            strcpy(who_list[i].name, newname);
            break;
        }
    }
}
int main(int argc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;
    /*----------------------signal1 setting----------------------*/
    struct sigaction name_signal;
    bzero(&name_signal, sizeof(name_signal));
    name_signal.sa_handler = name_handler;
    sigaction(NAME_SIG, &name_signal, NULL);
    set_signal_action();
    signal(SIGUSR1, whosignalHandler);
    // signal(SIGUSR2, SIG_IGN);
    signal(NAME_SIG, name_handler);
    /*----------------------socket setting----------------------*/
    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9876);    
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
        exit(0);
    }

    if (listen(listenfd, 5) != 0 ) {
        perror("listen error");
    }   
    /*----------------------shell setting----------------------*/
    while(1)
    {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if(connfd == -1)
        {
            // perror("accept error");
            continue;
        }
        /*making who_info*/
        struct sockaddr_in* client_addr = (struct sockaddr_in*)&cliaddr;
        char client_ip[20] = {0};
        inet_ntop(AF_INET, &client_addr->sin_addr, client_ip, 20);
        int client_port = ntohs(client_addr->sin_port);           
        mkfifo("who_fifo",0777);   
        mkfifo("name_fifo",0777);    
        childpid = fork();
        add_who_info(&who_list, client_ip, client_port, connfd, childpid);
        print_who_info_for_parent(&who_list);
        if ( childpid == 0 ) { //child process
            close(listenfd);     // close listening socket in child process
            /*enter shell*/      
            printf("childpid = %d\n", getpid());      
            printf("enter num_who = %d\n", num_who);
            sprintf(mes_fifo_path, "%d%s", getpid(), "mes_fifo");
            mkfifo(mes_fifo_path,0777);
            send(connfd, "% ", 2, 0);
            str_echo(connfd);    // process the request
            unlink(mes_fifo_path);
            exit(0);
        }
        else if(childpid < 0)
        {
            perror("fork error");
            exit(0);
        }
        else
        {
            close(connfd); // close connected socket in parent process
            continue;
        }                       
    }
    // unlink("who_fifo");
    // unlink("who_fifo");
}
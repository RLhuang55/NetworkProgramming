// 無窮迴圈 等input
#include"../include/myhrf.h"
#include"../include/ntools.h"
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
HISTORY_STATE *hist_state;
HIST_ENTRY **hist_list;
ssize_t readn(int filedes, void *buff, size_t nbytes);
ssize_t writen(int filedes, const void *buff, size_t nbytes);
ssize_t readlinen(int filedes, void *buff, size_t maxlen);
void sigchld_handler(int sig) {
    int saved_errno = errno;
    wait(NULL);
    errno = saved_errno;
}

void set_signal_action(void)
{
	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_handler = &sigchld_handler;
	sigaction(SIGCHLD, &act, NULL);
}
int main(int argc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;

    set_signal_action();

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9877);
    
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
        exit(0);
    }

    if (listen(listenfd, 5) != 0 ) {
        perror("listen error");
    }   

    /*----------------------shell setting----------------------*/
    setenv("PATH","/home/ruoling/Desktop/410821246HW2/bin",1);
    numpipe_num=-1;
    numpipe_enable=0;
    using_history();
    hist_state = history_get_history_state();
    hist_list = history_list();
    cmd=0;
    while(1)
    {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

        if ( (childpid = fork()) ==0 ) { //child process
            close(listenfd);     // close listening socket in child process
            char *fifo_path = "myfifo";
            mkfifo(fifo_path, 0666); //建立FIFO
            int fd = open(fifo_path, O_RDONLY); //打該FIFO的讀端
            dup2(connfd, STDIN_FILENO);
            // close(fd);
            connfd = open(fifo_path, O_WRONLY);
            dup2(connfd, STDOUT_FILENO);
            // close(fd);
            execl("../bin/bash", "bash", NULL);
            str_echo(connfd);    // process the request
            exit(0);
        };
        close(connfd); //close connected socket in parent process
        unlink(FIFO_PATH);
    }
}
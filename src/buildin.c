// 內建指令
#include"../include/myhrf.h"
#include"../include/mysig.h"
int buildin(int switch_bulcmd, int connfd)
{
    // sprintf("buildin.c\n");
    int testcd;
    char *p_build;
    switch (switch_bulcmd){
        case 0:
            printf("Exit Shell\n");
            // kill(getpid(), SIGTERM);
        case 1: //cd + _____
            chdir(tokenlist[cmd][1]);   
            char *cwd = NULL;
            cwd = (char *)malloc(1000 * sizeof(char)); 
            setenv("PWD", getcwd(cwd, 1000), 1);    
            free(cwd);    
            return 1;
        case 2:
            mkdir(tokenlist[cmd][1],0700);
            return 1;
        case 3: //printenv
            if(tokenlist[cmd][1])
            {
                if(strcmp(tokenlist[cmd][1], "LANG") == 0){
                    p_build = getenv("LANG") ;
                    printf("%s\n", p_build);
                }
                if(strcmp(tokenlist[cmd][1], "PATH") == 0){
                    p_build = getenv("PATH") ;
                    printf("%s\n", p_build);
                }
            }                            
            return 1;
        case 4: //getcwd
            testcd=1;
            cwd = NULL;
            cwd = (char *)malloc(1000 * sizeof(char));
            if(getcwd(cwd, 1000) != NULL){
                printf("Current working dir: %s\n", cwd);
            }
            else
                printf("getcwd() error\n");
            free(cwd);            
            return 1;
        case 5: //setenv
            if(tokenlist[cmd][1] == NULL){
                printf("please enter variable name\n");
            }
            if(tokenlist[cmd][2] == NULL){
                printf("please enter variable to assign\n");
            }
            else
                setenv(tokenlist[cmd][1], tokenlist[cmd][2], 1);           
            return 1;
        case 6: //who
            testcd=1;            
            kill(getppid(), SIGUSR1);            
            int fifo_fdRout = open("who_fifo", O_RDONLY);
            char buf[1024];
            int n;
            while((n = read(fifo_fdRout, buf, sizeof(buf)))>0);
            who_info who_list[10];
            int whonum = split_who(buf, &who_list);
            pid_t pid = getpid();
            printf("<ID>\t<name>\t<IP:port>\t\t<indicate me>\n");
            for(int i=0; i<whonum; i++){
                printf("%d\t%s\t%s:%d\t\t%s\n", who_list[i].ID, who_list[i].name, who_list[i].ip, who_list[i].port,  (pid == who_list[i].childpid) ? "<-(me)" : "" );
            }
            close(fifo_fdRout);
            return 1;
        case 7: //tell id message
            if(tokenlist[cmd][1] == NULL){
                printf("please enter id\n");
            }
            if(tokenlist[cmd][2] == NULL){
                printf("please enter message\n");
            }
            //get who list
            kill(getppid(), SIGUSR1);            
            fifo_fdRout = open("who_fifo", O_RDONLY);
            memset(buf, 0, sizeof(buf));
            n = 0;
            while((n = read(fifo_fdRout, buf, sizeof(buf)))>0);
            whonum = split_who(buf, &who_list);
            close(fifo_fdRout);
            //tell
            int ind = atoi(tokenlist[cmd][1]);
            char tell_path[100];
            pid = who_list[ind].childpid;
            sprintf(tell_path, "%d%s", pid, "mes_fifo");
            char tell_msg[1024];
            memset(tell_msg, 0, sizeof(tell_msg));;
            int user=0;
            for(int i=0;i<whonum;i++){
                if(getpid() == who_list[i].childpid){
                    user = who_list[i].ID;
                    break;
                }
            }
            sprintf(tell_msg, "%s%d%s%s", "<user(", user,") told you>: " ,tokenlist[cmd][2]);
            int fd_mesW = open(tell_path, O_WRONLY);
            write(fd_mesW, tell_msg, sizeof(tell_msg));
            close(fd_mesW);
            printf("send accpet!\n");
            return 1;
        case 8: //yell message
            if(tokenlist[cmd][1] == NULL){
                printf("please enter message\n");
            }
            //get who list
            kill(getppid(), SIGUSR1);            
            fifo_fdRout = open("who_fifo", O_RDONLY);
            memset(buf, 0, sizeof(buf));
            n = 0;
            while((n = read(fifo_fdRout, buf, sizeof(buf)))>0);
            whonum = split_who(buf, &who_list);
            close(fifo_fdRout);
            //yell
            char yell_path[100];
            user=0;
            for(int i=0;i<whonum;i++){
                if(getpid() == who_list[i].childpid){
                    user = who_list[i].ID;
                    break;
                }
            }
            for(int i=0; i<whonum;i++){
                // if(who_list[i].name == NULL)
                //     break;
                pid = who_list[i].childpid;
                sprintf(yell_path, "%d%s", pid, "mes_fifo");
                char yell_msg[1024];
                memset(yell_msg, 0, sizeof(yell_msg));
                sprintf(yell_msg, "%s%d%s%s", "<user(", user,") yelled>: " ,tokenlist[cmd][1]);
                if(i==user){
                    printf("%s\n", yell_msg);
                }
                else{
                    int fd_mesW = open(yell_path, O_WRONLY);
                    write(fd_mesW, yell_msg, sizeof(yell_msg));
                    close(fd_mesW);
                }
            }
            return 1;
        case 9: //name name
            if(tokenlist[cmd][1] == NULL){
                printf("please enter name\n");
            }
            else{
                char pid_str[10];
                char result[1024];
                pid = getpid();
                sprintf(pid_str, "%d", pid);
                sprintf(result, "%s%s%s", pid_str, "|", tokenlist[cmd][1]);
                //get who list
                kill(getppid(), SIGUSR1);            
                int fifo_fdRout = open("who_fifo", O_RDONLY);
                char buf[1024];
                int n;
                while((n = read(fifo_fdRout, buf, sizeof(buf)))>0);
                who_info who_list[10];
                int whonum = split_who(buf, &who_list);
                close(fifo_fdRout);
                //change name
                int flag=0;
                kill(getppid(), NAME_SIG);  
                for(int i=0; i<10; i++){
                    if(who_list[i].name == NULL)
                        break;
                    if(strcmp(who_list[i].name, tokenlist[cmd][1]) == 0){
                        printf("User %s already exists\n", tokenlist[cmd][1]);
                        flag=1;
                        break;
                    }
                }
                if(flag==0){
                    int fdW = open("name_fifo", O_WRONLY);                      
                    write(fdW, result, sizeof(result));
                    close(fdW);   
                    printf("name change accept!\n");
                } 
            }       
            return 1;
        default:
            break;      

    }
    return 0;
}
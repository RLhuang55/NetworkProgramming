#include"../include/myhrf.h"
int exe(int switch_shellcmd, char ***argv)
{
    switch (switch_shellcmd){
        case 1:{
            pid_t pid = fork(); 
            if (pid == -1) {
                return 1;
            } 
            else if (pid == 0) {
                // printf("this is child process and child's pid = %d,parent's pid = %d\n",getpid(),getppid());
                // printf("execvp: %d\n",execvp(tokenlist[0], tokenlist));
                if (execvp(tokenlist[cmd][0], tokenlist[cmd]) < 0) {
                    printf("\nCould not execute command.."); 
                }
                exit(0);
            } 
            else {
                // waiting for child to terminate
                // printf("this is parent process and pid =%d ,child's pid = %d\n",getpid(),pid);
                wait(NULL); 
                return 1;
            }
            return 1;
        }
        case 2:{
            // printf("multiple pipes cmd\n");
            int unknown_index[num_of_pipe];
            int cmdnum=0;
            for(int i=0;i<=num_of_pipe;i++)
            {
                int ifun = iftk_unknown(argv, i);
                if(ifun==0)//known cmd
                {
                    cmdnum++;
                    unknown_index[i]=1;
                }                    
                else //unknown cmd
                    unknown_index[i]=0;
            }
            // printf("cmdnum %d\n",cmdnum);
            int pipes[2*(cmdnum-1)];
            pid_t pid;
            for( int i = 0; i < cmdnum-1; i++ ){
                if( pipe(pipes + i*2) < 0 ){
                    perror("pipe init");
                }
            }
            int index=0;
            for(int i=0; i<=num_of_pipe;i++)
            {
                // printf("i: %d\n",i);
                // printf("index: %d\n",index);
                // printf("argv[%d][0]: %s\n", i, argv[i][0]);
                if(unknown_index[i]==1)
                {
                    pid = fork();
                    if(pid==0)
                    {
                        // close(STDIN_FILENO);
                        // close(STDOUT_FILENO);
                        // if(index==0)
                        //     close(STDIN_FILENO);
                        // else if(index == cmdnum-1)
                            // close(STDOUT_FILENO);
                        if(index!=0)// not first command: STDIN ro read-end
                        {                        
                            dup2(pipes[(index-1) * 2], 0);
                                // perror("dup2 cond1");
                        }
                        if(index!=cmdnum-1) //not last command: STDOUT to write-end
                        {
                            dup2(pipes[index * 2 + 1], 1);
                                // perror("dup2 cond2");
                        }
                        for(int j = 0; j < 2*(cmdnum-1); j++){
                            close(pipes[j]);
                        }
                        if( execvp(argv[i][0], argv[i]) < 0 ){                       
                            // perror("excvp");
                        }
                    }   
                    else if(pid<0)
                    {
                        // perror("fork error");
                        exit(EXIT_FAILURE);
                    }
                    index++;
                }
                else
                    continue;
                
            }
            for(int j=0; j< 2*(cmdnum-1);j++) 
            {
                close(pipes[j]);
            }      
            waitpid(pid, NULL, 0);
            // perror("while waitpid");
            return 1;
        }
        default:
            break;      
    }
    
}
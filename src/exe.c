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
                
                if (execvp(tokenlist[cmd][0], tokenlist[cmd]) < 0) {
                    printf("\nCould not execute command.."); 
                }
                exit(0);
            } 
            else {                
                wait(NULL);
                return 1;
            }
            return 1;
        }
        case 2:{
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
                if(unknown_index[i]==1)
                {
                    // int save_stdout = dup(STDOUT_FILENO);
                    pid = fork();
                    if(pid==0)
                    {                        
                        if(index!=0)// not first command: STDIN ro read-end
                        {                        
                            dup2(pipes[(index-1) * 2], 0); //0=STDIN
                                // perror("dup2 cond1");
                        }
                        if(index!=cmdnum-1) //not last command: STDOUT to write-end
                        {
                            dup2(pipes[index * 2 + 1], 1); //1=STDOUT
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
                    /*else {
                        wait(NULL); //parent
                        return 1;
                    }*/
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
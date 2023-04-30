#include"../include/myhrf.h"
int parseinit()
{
    /*--------------------------------------buildin.c--------------------------------------*/
    bulin_flag=-1; //if flag=1 then turn to buildin.c
    int count=0, j=0;
    for(j=0;j<NUM_BULINCMD;j++)
    {
        if (strcmp(tokenlist[cmd][0], bulincmd_list[j]) == 0) {
            bulin_flag = j;
            break;
        }
    }
    if(bulin_flag!=-1)
        buildin(bulin_flag);
        
    /*-----------------------------------decide shell flag-----------------------------------*/
    shell_flag=0;
    num_of_pipe=0;
    if(is_pipe)
    {
        for(j=0;j<num_of_token;j++) //算有幾個pipe 
        {
            if(strcmp(tokenlist[cmd][j], "|")==0) 
                num_of_pipe++;
        }
        if(strstr(tokenlist[cmd][num_of_token-1],"|")!=NULL) //如果是numberpipe 取出numberpipe
        {
            char* numtoken = strtok(tokenlist[cmd][num_of_token-1], "|");
            numpipe_num = (*numtoken)-'0';
            if(numpipe_num>128 || numpipe_num<1)
                printf("Numbered-Pipes N out of range\n");
            else
            {   /*reset all numpipe index*/
                shell_flag = 3;
                numpipe_enable = 1;
                numpipe_index = numpipe_num; //number的數字
                num_of_pipe++;
            }                   
        }
        if(num_of_pipe>=1 && shell_flag!=3) //如果是一般pipe
            shell_flag=2;        
    }
    else if(bulin_flag==-1) //如果都沒有pipe 且不是buildin
        shell_flag=1;   
    else
        shell_flag=-1;
    /*-----------------------------------check unknown cmd-----------------------------------*/
    int match_time=0;
    if_unknown=0;
    unknown_count=0;
    if(shell_flag==1)
    {
        for(int a=0; a<NUM_SHELLCMD;a++)
        {
            if(strcmp(tokenlist[cmd][0], shellcmd_list[a])==0)
            {
                match_time=1;//只要match到一個token就不是未知指令
                break; 
            }                                     
        }
        if(match_time==0 && bulin_flag==-1)
        {
            if_unknown=1;
            numpipe_index++;
            printf("Unknown command: [%s]\n",tokenlist[cmd][0]);
        }  
    }
    else if(shell_flag==2 || shell_flag==3)
    {
        for(int j =0;j<num_of_token;j++)
        {
            if(shell_flag==3 && j==num_of_token-1) //number pipe的最後一項不算unknown cmd
                break;
            else if(j==0) //第一項指令不用跳過pipe
            {
                for(int a=0; a<NUM_SHELLCMD;a++)
                {
                    if(strcmp(tokenlist[cmd][0], shellcmd_list[a])==0)
                    {
                        match_time=1;//只要match到一個token就不是未知指令
                        break; 
                    }   
                }
                if(match_time!=1)
                {
                    printf("Unknown command: [%s]\n",tokenlist[cmd][0]);
                    unknown_count++;
                    if_unknown=1;
                } 
                match_time=0;
            }
            else 
            {
                if(strcmp(tokenlist[cmd][j],"|")==0) //pipe的後一像是shellcmd
                {
                    for(int a=0; a<NUM_SHELLCMD;a++)
                    {
                        if(strcmp(tokenlist[cmd][j+1], shellcmd_list[a])==0)
                        {
                            match_time=1;//只要match到一個token就不是未知指令
                            break; 
                        }   
                    }
                    if(match_time!=1)
                    {
                        if_unknown=1;
                        unknown_count++;
                        printf("Unknown command: [%s]\n",tokenlist[cmd][j+1]);
                    }
                    match_time=0;   
                }
            }
        }
    }
    /*-----------------------------------make argv table &exe-----------------------------------*/
    if(numpipe_enable==1 && if_unknown==1)
    {
        numpipe_num++;
        // unk_bet_pipe++;
    }       
    int argvnum=0;
    char **argv[MAX_CMDLEN]={};
    if(numpipe_enable==1 && numpipe_index == 0) //number pipe製作argvlist
    {
        int k=0; 
        while(1) //計算之前的cmd有幾個token
        {
            if(tokenlist[cmd-numpipe_num][k]!=NULL)
                k++;
            else
                break;
        }
        int head = 0;
        for(int count=0;count<k;count++)
        {
            if(count==k-1)
            {
                tokenlist[cmd-numpipe_num][count] = NULL;
                argv[argvnum]= &tokenlist[cmd-numpipe_num][head];
                argvnum++;
            }
            else if(strcmp(tokenlist[cmd-numpipe_num][count], "|")==0)
            {
                tokenlist[cmd-numpipe_num][count] = NULL;
                argv[argvnum]= &tokenlist[cmd-numpipe_num][head];
                head = count+1;
                argvnum++;
            }    
        }
        head=0;
        for(int count=0;count<num_of_token;count++)
        {
            if(strcmp(tokenlist[cmd][count], "|")==0)
            {
                tokenlist[cmd][count] = NULL;
                argv[argvnum]= &tokenlist[cmd][head];
                head = count+1;
                argvnum++;
            }
            if(count==num_of_token-1)
            {
                argv[argvnum]= &tokenlist[cmd][head];
                argvnum++;
            }  
            
        }
        argvptr = &argv[0];
        num_of_pipe = argvnum-1;
        int status = exe(2, argvptr);
        wait(&status);
        numpipe_enable = 0;
        numpipe_index=0;
        numpipe_num = -1;

    }
    else if(if_unknown==0 && shell_flag==1 && bulin_flag==-1) //simple command
    {
        int status=exe(shell_flag,NULL);
        wait(&status);
    }  
    else if(shell_flag==2)//是multiple pipes則要分成指令的token
    {
        int head = 0;
        for(int count=0;count<num_of_token;count++)
        {
            if(strcmp(tokenlist[cmd][count], "|")==0)
            {
                tokenlist[cmd][count] = NULL;
                argv[argvnum]= &tokenlist[cmd][head];
                head = count+1;
                argvnum++;
            }
            if(count==num_of_token-1)
            {
                argv[argvnum]= &tokenlist[cmd][head];
                argvnum++;
            }            
        }
        argvptr = &argv[0];
        int status = exe(shell_flag, argvptr);
        wait(&status);
    }
    if(numpipe_index!=-1 && numpipe_enable==1)
        numpipe_index--;
    return 1;
}
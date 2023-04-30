// 內建指令
#include"../include/myhrf.h"
int buildin(int switch_bulcmd)
{
    // perror("buildin");
    switch (switch_bulcmd){
        case 0:
            printf("Exit Shell\n");
            exit(0);
        case 1:      
            int testcd = chdir(tokenlist[cmd][1]);
            perror("chdir");
            return 1;
        case 2:
            mkdir(tokenlist[cmd][1],0700);
            return 1;
        case 3:
            if(tokenlist[cmd][1])
            {
                if(strcmp(tokenlist[cmd][1], "LANG") == 0)
                    printf("%s\n", getenv("LANG"));
                if(strcmp(tokenlist[cmd][1], "PATH") == 0)
                    printf("%s\n", getenv("PATH"));
            }                            
            return 1;
        case 4:
            char cwd[500];
            if(getcwd(cwd, sizeof(cwd)) != NULL)
                printf("Current working dir: %s\n", cwd);
            else
                perror("getcwd() error");            
            return 1;
        case 5:
            printf("setenv\n");
            if(tokenlist[cmd][1] == NULL)
                printf("please enter variable name\n");
            if(tokenlist[cmd][2] == NULL)
                printf("please enter variable to assign\n");
            else
                setenv(tokenlist[cmd][1], tokenlist[cmd][2], 1);           
            return 1;
        default:
            break;      

    }
    return 0;
}
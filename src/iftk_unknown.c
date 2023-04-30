#include"../include/myhrf.h"
int iftk_unknown(char ***argv, int i)
{
    int match = 0;
    // printf("ifun argn[][]:%s\n",argv[i][0]);
    for(int a =0; a<NUM_SHELLCMD;a++)
    {
        if(strcmp(argv[i][0], shellcmd_list[a])==0)
        {
            match = 1;//只要match到一個token就不是未知指令
            break; 
        }   
    }
    if(match==1)
        return 0;
    else
        return 1;
}
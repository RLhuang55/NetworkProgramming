#include"../include/myhrf.h"
#include <dirent.h>
void doinit(char* buf)
{
    //Make input from command line to tokens
    buf = strtok(buf, "\n"); //delete \n
    /*make tokenlist*/    
    if(strstr(buf,"|")==NULL)
        is_pipe=0;
    else
        is_pipe=1;
    char* token = strtok(buf, " ");
    num_of_token = 0;
    tokenlist[cmd] = (char**)malloc(sizeof(char*) * MAX_CMDLEN);
    while(token!=NULL)
    {
        tokenlist[cmd][num_of_token] = token;
        token = strtok(NULL," ");
        num_of_token++;
    }
    tokenlist[cmd][num_of_token] = NULL;
    /*buildin cmd list*/
    bulincmd_list[0]="quit";
    bulincmd_list[1]="cd";
    bulincmd_list[2]="mkdir";
    bulincmd_list[3]="printenv";
    bulincmd_list[4]="getcwd";
    bulincmd_list[5]="setenv";
    /*shell cmd list*/
    DIR *dir;
    struct dirent *entry;
    char *dir_name = "/home/ruoling/Desktop/410821246HW2/bin"; // 指定目錄名稱
    dir = opendir(dir_name); // 開啟目錄
    if (dir == NULL)
        perror("opendir");
    int shell_cmd_num=0;
    while ((entry = readdir(dir)) != NULL) { // 讀取目錄中的檔案
        if (entry->d_name[0] == '.') // 忽略隱藏檔
            continue;    
        shellcmd_list[shell_cmd_num++] = entry->d_name;
    }
    closedir(dir); // 關閉目錄
    // for(int i=0; i<shell_cmd_num; i++)
    //     printf("%s\n", shellcmd_list[i]);
}
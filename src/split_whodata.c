#include "../include/myhrf.h"
int split_who(char buf[], who_info * who_list) {
    int list_ind = 0; //紀錄有幾份資料
    char *data_token = strtok(buf, "|"); // 以|切割資料中的每一個欄位
    int data_ind = 0;
    while(data_token != NULL) {  
        if(data_ind==0)
            who_list[list_ind].ID = atoi(data_token); 
        else if(data_ind==1)
            strcpy(who_list[list_ind].name, data_token);
        else if(data_ind==2)
            strcpy(who_list[list_ind].ip, data_token);
        else if (data_ind==3)
            who_list[list_ind].port = atoi(data_token);
        else if (data_ind==4)
            who_list[list_ind].connfd = atoi(data_token);
        else {
            who_list[list_ind].childpid = atoi(data_token);
            list_ind++;
        }
        if(data_ind==5)
            data_ind = 0;
        else    
            data_ind++; 
        data_token = strtok(NULL, "|");  
    }
    return list_ind;
}
#include"../include/myhrf.h"
int add_who_info(who_info * who_list, char ip[20], int port,int connfd,pid_t childpid) {    
    who_list[num_who].ID = num_who;
    strcpy(who_list[num_who].name, "no_name");
    strcpy(who_list[num_who].ip, ip);
    who_list[num_who].port = port;
    who_list[num_who].connfd = connfd;
    who_list[num_who].childpid = childpid;
    num_who++;
}

int remove_who_info(who_info * who_list,int index) {
    for (int i = index; i < num_who-1; i++) {
        who_list[i].ID = who_list[i+1].ID;
        strcpy(who_list[i].name, who_list[i+1].name);
        strcpy(who_list[i].ip, who_list[i+1].ip);
        who_list[i].port = who_list[i+1].port;
        who_list[i].connfd = who_list[i+1].connfd;
        who_list[i].childpid = who_list[i+1].childpid;
    }
    who_list[num_who-1].ID = 0;
    who_list[num_who-1].port = 0;
    who_list[num_who-1].connfd = 0;
    memset(who_list[num_who-1].name, 0, 12);
    memset(who_list[num_who-1].ip, 0, 20);
    num_who--;  
    for (int i = 0; i < num_who; i++) {
        who_list[i].ID = i;
    }
    return num_who;
}

void print_who_info(who_info * who_list) {
    for (int i = 0; i < num_who; i++) {
        printf("%d|",who_list[i].ID); 
        printf("%s|",who_list[i].name); 
        printf("%s|",who_list[i].ip); 
        printf("%d|",who_list[i].port); 
        printf("%d|",who_list[i].connfd); 
        printf("%d|",who_list[i].childpid);
    }
    printf("\n");
}

void print_who_info_for_parent(who_info * who_list) {
    printf("ID\tname\tip\t\tport\tconnfd\tchildpid\n");
    for (int i = 0; i < num_who; i++) {
        printf("%d\t",who_list[i].ID); 
        printf("%s\t",who_list[i].name); 
        printf("%s\t",who_list[i].ip); 
        printf("%d\t",who_list[i].port); 
        printf("%d\t",who_list[i].connfd); 
        printf("%d\n",who_list[i].childpid);
    }
    printf("\n");
}
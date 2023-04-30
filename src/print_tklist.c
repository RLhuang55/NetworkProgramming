#include"../include/myhrf.h"
void print_tokenlist()
{
    for(int i=0;i<num_of_token;i++)
    {
        printf("%s \n",tokenlist[cmd][i]);
    }
    printf("\n");
}
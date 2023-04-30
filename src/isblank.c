#include"../include/myhrf.h"
int isblank_p(char* buf)
{
    if(buf ==NULL || *buf == '\0')
        return 1;
    else
        return 0;
}
#include"../include/myhrf.h"
int isblank_p(char* buf)
{
    if(buf ==NULL || *buf == '\r')
        return 1;
    else
        return 0;
}
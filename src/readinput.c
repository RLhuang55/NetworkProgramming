#include"../include/myhrf.h"
void rInput(char * buf)
{
    buf_point[cmd]=readline("% "); //ex: ls | number
    if(isblank_p(buf_point[cmd])!=1)
    {
        add_history(buf_point[cmd]);
        doinit(buf_point[cmd]);
        parseinit();
        cmd++;
    }
}
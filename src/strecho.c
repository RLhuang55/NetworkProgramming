#include"../include/myhrf.h"
#include<errno.h>
void str_echo(int sockfd)
{
	ssize_t n;
	char buff[MAX_CMDLEN];

	again:
	while( (n = read(sockfd, buff, MAX_CMDLEN)) > 0) 
		printf("buff:%s\n",buff);
		buf_point[cmd]=buff;
		if(isblank_p(buf_point[cmd])!=1)
    	{
        	add_history(buf_point[cmd]);
        	doinit(buf_point[cmd]);
        	parseinit();
        	cmd++;
    	}
		// writen(sockfd, buff, n);
	
	if (n<0 && errno == EINTR)
		goto again;
	else if (n<0)
		perror("read error");
	exit(0);
}
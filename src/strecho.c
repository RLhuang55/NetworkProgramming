#include"../include/myhrf.h"
#include<pthread.h>
void str_echo(int sockfd)
{
	setenv("PATH","./bin",1);
    numpipe_num=-1;
    numpipe_enable=0;
    using_history();
    hist_state = history_get_history_state();
    hist_list = history_list();
    cmd=0;
	ssize_t n;
	char buff[MAX_CMDLEN];
	int saved_STDOUT = dup(STDOUT_FILENO); //儲存client的stdout
	struct pollfd fds[2];
	// int fd_mesR = open(mes_fifo_path, O_RDONLY | O_NONBLOCK); //開啟mes的fifo
	// fds[0].fd = fd_mesR;
	// fds[0].events = POLLIN;
	// fds[1].fd = sockfd;
	// fds[1].events = POLLIN;
	again:	
	while(1){
		int fd_mesR = open(mes_fifo_path, O_RDONLY | O_NONBLOCK); //開啟mes的fifo
		fds[0].fd = fd_mesR;
		fds[0].events = POLLIN;
		fds[1].fd = sockfd;
		fds[1].events = POLLIN;
		int result = poll(fds, 2, -1); //等待1.client自己輸入 2.其他client傳訊息
		if (result == -1) {
			perror("poll");
			exit(1);
		}
		if (fds[0].revents & POLLIN) { //其他client傳訊息
			n = read(fd_mesR, buff, sizeof(buff) - 1);
			buff[n] = '\n';
			send(sockfd, buff, n, 0);
			close(fd_mesR);
			send(sockfd, "\n% ", 3, 0);
			
		}
		else if (fds[1].revents & POLLIN) { //client自己輸入
			// close(fd_mesR);
			n = read(sockfd, buff, sizeof(buff) - 1);
			buf_point[cmd]=buff;
			if(isblank_p(buf_point[cmd])!=1)
    		{
				saved_STDOUT = dup(STDOUT_FILENO);
				buff[n] = '\0';
				buff[MAX_CMDLEN-1] = '\0';
				// printf("buff:%s",buff);
        		add_history(buff);
        		doinit(buf_point[cmd]);
				// printf("init pass\n");
				dup2(sockfd, STDOUT_FILENO);
        		parseinit(sockfd);
				dup2(saved_STDOUT, STDOUT_FILENO);
				// printf("parse pass\n");
        		cmd++;
    		}
			// printf("Hello message sent\n");
			if(strstr(buff,"quit")!=NULL){
				break;
			}
			send(sockfd, "% ", 2, 0);
			memset(buff, 0, sizeof(buff));
		}
		close(fd_mesR);
		
	}
	// if (n<0 && errno == EINTR){
	// 	printf("EINTR\n");
	// 	goto again;
	// }
	// else if (n<0){
	// 	perror("read error");
	// }
	// exit(0);
}
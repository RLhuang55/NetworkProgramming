#ifndef NTOOLS_H
#define NTOOLS_H
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 5000
// ssize_t readn(int filedes, void *buff, size_t nbytes);
// ssize_t writen(int filedes, const void *buff, size_t nbytes);
// ssize_t readlinen(int filedes, void *buff, size_t maxlen);

extern ssize_t readn(int fd, void *buff, size_t n) {
    ssize_t nread;
    size_t nleft = n;
    char *ptr = (char *)buff;

    while (nleft > 0) {
        if ((nread=read(fd, buff, nleft))<0) {
            if (errno == EINTR)
                nread = 0;
            else  return(-1);
        } else if (nread==0)
            break;  //EOF
        nleft -= nread;
        ptr += nread;
    }
    return (n-nleft);  //return >=0
}
extern ssize_t writen(int fd, const void *buff, size_t n) {
    ssize_t nwritten;
    size_t nleft = n;
    const char *ptr = buff;

    while (nleft > 0) {
        if ((nwritten=write(fd, buff, nleft))<=0) {
            if (nwritten <0 && errno == EINTR)
                nwritten = 0; //call write again
            else  return(-1);
        } 
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n);
}
extern ssize_t readlinen(int fd, void *buff, size_t maxlen) {
    ssize_t n, rc;
    char c, *ptr=buff;

    for(n==1;n<maxlen;n++){
        again:
        if ((rc=read(fd,&c,1))==1) {
            *ptr++ = c;
            if (c=='\n') break;
        }else if (rc==0){
            *ptr = 0;
            return (n-1); // EOF is read, n-1 bytes were read
        }else {
            if (errno == EINTR) 
                goto again;
            return(-1);
        }
    }
    *ptr = 0;
    return(n);
}
#endif
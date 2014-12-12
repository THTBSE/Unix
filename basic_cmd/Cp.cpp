#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXBYTES 1024

void unix_error(char *msg)
{
    fprintf(stderr,"Errors : %s\n",msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
	printf("Not enough parameter for this command\n");
	exit(0);
    }
    int srcfd, tarfd;
    if ((srcfd = open(argv[1],O_RDONLY)) == -1)
    {
	unix_error("open error!");
    }
    if ((tarfd = creat(argv[2],S_IRWXO)) == -1)
    {
	unix_error("creat error!");
    }

    int reclen;
    char buf[MAXBYTES];
    while ((reclen = read(srcfd,buf,MAXBYTES)) > 0)
    {
	if (write(tarfd,buf,reclen) != reclen)
        {
	    unix_error("write error!");
	}
    }
    close(srcfd);
    close(tarfd);
    exit(0);   
}

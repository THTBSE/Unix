#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXBYTES 1024
extern int errno;

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
    if (strcmp(argv[1],argv[2]) == 0)
    {
	printf("copy:\'%s\' and \'%s\' are the same file.\n",argv[1],argv[2]);
	exit(1); 
    }
    
    if (open(argv[2],O_RDONLY) != -1)
    {
	printf("overwrite %s (y/n)",argv[2]);
        int y = (int)'y';
        int n = (int)'n';
	int r = getc(stdin);
        if (n == r || y != r)
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

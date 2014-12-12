#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

 
void showInfo(utmp &info);
void showTime(time_t time);

#define SHOWHOST

int main()
{
    utmp current_info;
    int fd;
    int reclen = sizeof(current_info);
    if ((fd = open("/var/run/utmp",O_RDONLY)) == -1)
    {
	printf("open error\n");
	exit(1);
    }
    while (read(fd,&current_info,reclen) == reclen)
    {
	showInfo(current_info);
    }

    if (close(fd) == -1)
    {
	printf("close error\n");
	exit(1);
    }
    exit(0);
}

void showInfo(utmp &info)
{
    if (info.ut_type != USER_PROCESS)
	return;
    printf("% -8.8s",info.ut_name);
    printf(" ");
    printf("% -8.8s",info.ut_line);
    printf(" ");
    showTime(info.ut_time);
    #ifdef SHOWHOST
    printf("(%s)",info.ut_host);
    #endif
    printf("\n");
}

void showTime(time_t time)
{
    tm *tms = localtime(&time);
    printf("    %d-%d-%d %d:%d",tms->tm_year+1900,tms->tm_mon+1,tms->tm_mday,
tms->tm_hour,tms->tm_min);
    printf(" ");
}

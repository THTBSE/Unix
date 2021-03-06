testing 123... .h>
#include <unistd.h>
#include <utmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define UTMPBUF 16
#define SHOWHOST
 
int  utmp_open();
int  utmp_read(utmp buffer[], int fd, int reclen); 
void utmp_close(int fd);
 
void showInfo(utmp &info);
void showTime(time_t time);

void unix_error(char *msg)
{
	fprintf(stderr,"Error : %s\n", msg);
	exit(1);
}

int main()
{
    utmp utmpBuffer[UTMPBUF];
    int reclen = sizeof(utmpBuffer);
    int fd;
	fd = utmp_open();
	int count;
	do
	{
		count = utmp_read(utmpBuffer,fd, reclen);
		for (int i=0; i<count; ++i)
		{
			showInfo(utmpBuffer[i]);
		}
	}while(count == UTMPBUF);
    utmp_close(fd);
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

int utmp_open()
{
	int fd;
    if ((fd = open("/var/run/utmp",O_RDONLY)) == -1)
    {
		unix_error("open error");
    }
	return fd;
}

void utmp_close(int fd)
{
	if (close(fd) == -1)
	{
		unix_error("close error");
	}
}

//return the number of utmp struct
int utmp_read(utmp buffer[], int fd, int reclen)
{
    int realsize;
    if ((realsize = read(fd,buffer,reclen)) == -1)
    {
		unix_error("read error");
    }
	int count = realsize / sizeof(utmp);
	return count;
}

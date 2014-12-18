#include <ftw.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string>
#include <string.h>
using std::string;

typedef int func(const char *, const struct stat *, int);

static func fn;
static int nftw01(const char *, func *);

long numReg = 0,numDir = 0,numChr = 0,
numBlk = 0,numFifo = 0,numLnk = 0,numSock = 0;

void PrintResult()
{
	printf("Reg : %8ld\n",numReg);
	printf("Dir : %8ld\n",numDir);
	printf("Chr : %8ld\n",numChr);
	printf("Blk : %8ld\n",numBlk);
	printf("Fifo: %8ld\n",numFifo);
	printf("Lnk : %8ld\n",numLnk);
	printf("Sock: %8ld\n",numSock);
}



int fn(const char *fpath, const struct stat *sb,int typeflag)
{
	switch(typeflag)
	{
	case FTW_F:
		switch(sb->st_mode & S_IFMT)
		{
		case S_IFREG: numReg++; break;
		case S_IFBLK: numBlk++;	break;
		case S_IFCHR: numChr++; break;
		case S_IFIFO: numFifo++; break;
		case S_IFLNK: numLnk++; break;
		case S_IFSOCK: numSock++; break;
		}
		break;
	case FTW_D:
		numDir++;
		break;
	case FTW_DNR:
		printf("can't read directory %s\n", fpath);
		break;
	case FTW_NS:
		printf("stat error for %s\n",fpath);
		break;
	default:
		printf("unknow type %d for pathname %s\n",typeflag, fpath);
	}
	return 0;
}


int nftw01(const char *dirpath, func *f)
{
	DIR *dp;
	if ((dp = opendir(dirpath)) == NULL)
	{
		perror("opendir\n");
		exit(1);
	}

	struct stat sb;
	if (lstat(dirpath,&sb) < 0)
	{
		perror("lstat error\n");
		exit(1);
	}

	f(dirpath,&sb,FTW_D);

	dirent *fp;
	while ((fp = readdir(dp)) != NULL)
	{
		if (strcmp(fp->d_name,".") == 0
			|| strcmp(fp->d_name,"..") == 0)
			continue;

		string wpath(dirpath);
		if (wpath.back() != '/')
			wpath.push_back('/');
		wpath += fp->d_name;

		if (lstat(wpath.c_str(),&sb) < 0)
		{
			perror("lstat error\n");
			exit(1);
		}

		printf("%s\n",wpath.c_str());
		if (S_ISDIR(sb.st_mode))
		{
			if (!(sb.st_mode & (S_IRUSR | S_IRGRP | S_IROTH)))
				f(wpath.c_str(),&sb,FTW_DNR);
			else
			{
				nftw01(wpath.c_str(),f);
			}
		}
		else
		{
			f(wpath.c_str(),&sb,FTW_F);
		}
		
	}

	if (closedir(dp) < 0)
	{
		perror("closedir");
		exit(1);
	}
	return 0;
}
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("at least 2 parameters\n");
		exit(0);
	}

	nftw01(argv[1],fn);
//	ftw(argv[1],fn,1000);
	PrintResult();

	return 0;
}

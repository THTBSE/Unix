#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <string>
using std::string;
#define MAXBYTES 1024
extern int errno;

#define RWXRWRW (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IRGRP)

void unix_error(const char *msg)
{
    fprintf(stderr,"Errors : %s\n",msg);
    exit(1);
}

//dir 0
//reg 1
inline int dir_reg(const char *file);

string dirformat(const char *dir);

void file2dir(const char *src, const char *dest);
void file2file(const char *src, const char *dest);
void dir2dir(const char *src, const char *dest);


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
	   printf("copy: missing file operand\n");
	   exit(0);
    }
    else if (argc < 3)
    {
        printf("copy: missing destination file operand after %s\n",argv[1]);
        exit(0);
    }

    if (strcmp(argv[1],argv[2]) == 0)
    {
	   printf("copy:\'%s\' and \'%s\' are the same file.\n",argv[1],argv[2]);
	   exit(0); 
    }
    
    struct stat FileInfo1,FileInfo2;
    if (stat(argv[1],&FileInfo1) < 0)
    {
        perror("stat error");
        exit(1);
    }

    bool SrcIsReg = S_ISREG(FileInfo1.st_mode);
    bool DestIsReg;

    if (stat(argv[2],&FileInfo2) < 0)
    {
        if (errno != ENOENT)
        {
            perror("stat error");
            exit(1);
        }
        DestIsReg = true;
    }
    else
    {
        DestIsReg = S_ISREG(FileInfo2.st_mode);
    }

    if (SrcIsReg && DestIsReg)
    {
        file2file(argv[1],argv[2]);
    }
    else if (!SrcIsReg && !DestIsReg)
    {
        if (S_ISDIR(FileInfo1.st_mode) && S_ISDIR(FileInfo2.st_mode))
        {
            dir2dir(argv[1],argv[2]);
        }
        else
        {
            printf("Wrong file format");
            exit(0);
        }
    }
    else if (SrcIsReg && !DestIsReg)
    {
        if (S_ISDIR(FileInfo2.st_mode))
        {
            file2dir(argv[1],argv[2]);
        }
        else
        {
            printf("Wrong file format");
            exit(0);
        }
    }
    else
    {
        printf("Wrong file format");
        exit(0);
    }

    exit(0);   
}

void file2file(const char *src, const char *dest)
{
    if (open(dest,O_RDONLY) != -1)
    {
        printf("overwrite %s (y/n)",dest);
        int y = (int)'y';
        int n = (int)'n';
        int r = getc(stdin);
        if (n == r || y != r)
           exit(0);
    }

    int srcfd, tarfd;
    if ((srcfd = open(src,O_RDONLY)) == -1)
    {
       unix_error("open error!");
    }
    if ((tarfd = creat(dest,RWXRWRW)) == -1)
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
}

void file2dir(const char *src, const char *dest)
{
    string path = dirformat(dest);
    string Src(src);
    size_t pos = Src.find_last_of("/");
    if (pos != string::npos)
    {
        Src = Src.substr(pos+1);
    }
    path += Src;
    file2file(src,path.c_str());
}

void dir2dir(const char *src, const char *dest)
{
    DIR *dp;
    if ((dp = opendir(src)) == NULL)
    {
       perror("open error");
       exit(1);
    }

    printf("open directory : %s\n",src);
    string SrcDir = dirformat(src);

    dirent *fp;
    while ((fp = readdir(dp)) != NULL)
    {
        string SubFile = fp->d_name;
        if (SubFile == "." || SubFile == "..")
            continue;
        SubFile = SrcDir + SubFile;
        int type = dir_reg(SubFile.c_str());
        if (type == 0)
        {
            dir2dir(SubFile.c_str(), dest);
        }
        else if (type == 1)
        {
            file2dir(SubFile.c_str(), dest);
        }
    }
}

string dirformat(const char *dir)
{
    string ret(dir);
    int len = strlen(dir);
    if (dir[len-1] != '/')
        ret.push_back('/');
    return ret;
}

int dir_reg(const char *file)
{
    struct stat buf;
    if (stat(file,&buf) < 0)
    {
        perror("stat error");
        exit(1);
    }

    if (S_ISDIR(buf.st_mode))
        return 0;

    if (S_ISREG(buf.st_mode))
        return 1;
}
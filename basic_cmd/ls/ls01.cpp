#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include "lsfile.h"
extern int errno;

static void ParseCmd(int argc, char *argv[], std::set<char>& parameter, std::string& dir);
static set<char> CmdSet{'a','l'};

int main(int argc, char *argv[])
{
    std::set<char> cmd;
    std::string dir = "./";
    if (argc > 1)
        ParseCmd(argc, argv, cmd, dir);

    DIR *dp;
    if ((dp = opendir(dir.c_str())) == NULL)
    {
       std::string msg("ls01:cannot access ");
       msg += dir;
	   perror(msg.c_str());
	   exit(0);
    }

    dirent *fp;
    std::vector<file> dnames;
    while ((fp = readdir(dp)) != NULL)
    {
	   dnames.push_back(file(fp->d_name));
    }
    std::sort(dnames.begin(),dnames.end());

    for (auto &f : dnames)
    {
        f.PrintFileInfo(cmd,dir);
    }

    if (closedir(dp) == -1)
    {
	   perror("close error:");
	   exit(1);
    }

    exit(0);
}

void ParseCmd(int argc, char *argv[], std::set<char>& parameter, std::string& dir)
{
    for (int i = 1; argv[i] != NULL; ++i)
    {
        if (argv[i][0] != '-')
        {
            dir = argv[i];
            continue;
        }
        for (int j = 1; argv[i][j] != '\0'; ++j)
        {
            if (CmdSet.count(argv[i][j]))
            {
                parameter.insert(argv[i][j]);
            }
            else
            {
                printf("ls01:cannot access %s:No such file or directory\n",argv[i]);
                exit(1);
            }
        }
    }
}
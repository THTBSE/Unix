#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <vector>
#include <algorithm>
extern int errno;

int main(int argc, char *argv[])
{
    DIR *dp;
    if ((dp = opendir("./")) == NULL)
    {
	perror("Error");
	exit(0);
    }
    
    dirent *fp;
    int count = 0;
    std::vector<std::string> dnames;
    while ((fp = readdir(dp)) != NULL)
    {
	dnames.push_back(fp->d_name);
    }
    std::sort(dnames.begin(),dnames.end());
    auto iter = std::max_element(dnames.begin(),dnames.end(),
	[](const std::string& lhs, const std::string& rhs) ->bool
	{
		return lhs.size() < rhs.size();
	});
    const auto width = iter->size();
    for (const auto &s : dnames)
    {
	if (count++ == 6)
	{
	    printf("\n");
            count = 0;
	}
	printf("%s ",s.c_str());
    } 
    if (closedir(dp) == -1)
    {
	perror("Error");
	exit(1);
    }
    exit(0);
}

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
	    std::string str;
	    while(std::getline(std::cin,str))
	    {
		std::cout << str << std::endl;
	    }
	    exit(0);
	}
	
	char *string;
	size_t read;
	size_t len;
	for (int i = 1; argv[i] != NULL; ++i)
	{
		FILE *fp;
		if ((fp = fopen(argv[i],"r")) == NULL)
		{
			perror("Error");
			exit(1);
		}
		while((read = getline(&string,&len,fp)) != -1)
		{
			printf("%s",string);
		}
		if (fclose(fp) != 0)
		{
			perror("Error");
			exit(1);
		}
	}
	free(string);
	exit(0);
}

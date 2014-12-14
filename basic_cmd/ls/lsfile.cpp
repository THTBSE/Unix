#include "lsfile.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <time.h>

static inline void ParseMode(mode_t mod, string& mode);
static inline void ParseUID(uid_t uid, string& owner);
static inline void ParseGID(gid_t gid, string& group);
static inline void ParseTime(time_t *mtime, string& stime);

file::file(const char *dname):name(dname),links(-1),bytes(-1)
{
	
}

bool operator< (const file &lhs, const file &rhs)
{
	return lhs.name < rhs.name;
}

void file::GetFileInfo(const std::string& dir)
{
	struct stat buff;
	string dir_name(dir);
	dir_name.push_back('/');
	dir_name += name;

	if (stat(dir_name.c_str(), &buff) == -1)
	{
		perror("get stat error");
		exit(1);
	}

	ParseMode(buff.st_mode,mode);
	links = buff.st_nlink;
	ParseUID(buff.st_uid,owner);
	ParseGID(buff.st_gid,group);
	bytes = (long)buff.st_size;
	ParseTime(&buff.st_mtime,last_modified);
}

void file::PrintFileInfo(const set<char>& cmd, const std::string& dir)
{
	if (!cmd.count('a') && name[0] == '.')
	{
		return;
	}

	if (cmd.count('l'))
	{
		GetFileInfo(dir);
		printf("%s",mode.c_str());
		printf("%4d ",links);
		printf("%-8s",owner.c_str());
		printf("%-8s",group.c_str());
		printf("%8ld ",bytes);
		printf("%.12s ",last_modified.c_str());
	}
	
	printf("%s\n",name.c_str());
}

void ParseMode(mode_t mod, string& mode)
{
	mode.clear();
	mode.resize(10,'-');

	if (S_ISREG(mod))
		mode[0] = '-';
	else if (S_ISDIR(mod))
		mode[0] = 'd';
	else if (S_ISCHR(mod))
		mode[0] = 'c';
	else if (S_ISFIFO(mod))
		mode[0] = 'f';
	else if (S_ISLNK(mod))
		mode[0] = 'l';
	else if (S_ISSOCK(mod))
		mode[0] = 's';
	else
		mode[0] = '*';

	//owner 
	if (mod & S_IRUSR) mode[1] = 'r';
	if (mod & S_IWUSR) mode[2] = 'w';
	if (mod & S_IXUSR) mode[3] = 'x';

	//group
	if (mod & S_IRGRP) mode[4] = 'r';
	if (mod & S_IWGRP) mode[5] = 'w';
	if (mod & S_IXGRP) mode[6] = 'x';

	//others
	if (mod & S_IROTH) mode[7] = 'r';
	if (mod & S_IWOTH) mode[8] = 'w';
	if (mod & S_IXOTH) mode[9] = 'x';

}

void ParseUID(uid_t uid, string& owner)
{
	struct passwd *pwd;
	if ((pwd = getpwuid(uid)) == NULL)
	{
		perror("getpwuid");
		exit(1);
	}

	owner = pwd->pw_name;
}

void ParseGID(gid_t gid, string& group)
{
	struct group *grp;
	if ((grp = getgrgid(gid)) == NULL)
	{
		perror("getgrgid");
		exit(1);
	}
	group = grp->gr_name;
}

void ParseTime(time_t *mtime, string& stime)
{
	char *st;
	if ((st = ctime(mtime)) == NULL)
	{
		perror("ctime");
		exit(1);
	}
	stime = st;
	stime = stime.substr(4);
}
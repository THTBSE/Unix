#ifndef LSFILE_H_
#define LSFILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <set>
using std::string;
using std::set;

class file
{
public:
	file(const char *dname);

	void PrintFileInfo(const set<char>& cmd, const std::string& dir);
	friend bool operator< (const file &lhs, const file &rhs);
private:
	void GetFileInfo(const std::string& dir);

	string name,mode,owner,group,last_modified;
	int links;
	long bytes;

};

#endif
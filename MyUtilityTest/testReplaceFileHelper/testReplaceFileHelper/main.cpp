#include <windows.h>
#include <tchar.h>

#include <iostream>
#include <fstream>
#include <string>

#include "../../../../MyUtility/ReplaceFileHelper.h"

int main()
{
	std::string file("file.txt");

	std::ifstream ifs(file.c_str());
	std::string str;
	ifs >> str;
	std::cout << str << std::endl;
	ifs.close();


	str += "X";


	std::string tmp = file + (".tmp");
	std::string back = file + (".back");

	std::ofstream ofs;
	ofs.open(tmp.c_str());
	ofs << str << std::endl;
	ofs.close();


	DWORD dwLE=0;
	if(!ReplaceFileHelper(file.c_str(), tmp.c_str(), back.c_str(), 0, &dwLE))
	{
		std::cerr << "error:" << dwLE << std::endl;
		return 1;
	}

	return 0;
}
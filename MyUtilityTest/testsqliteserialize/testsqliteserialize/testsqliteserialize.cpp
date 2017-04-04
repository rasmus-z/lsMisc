// testsqliteserialize.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include "../../../sqliteserialize.h"
#include "stlsoft/smartptr/scoped_handle.hpp"

int main()
{
	for (int i=0 ; ; ++i)
	{
		int d = (int)GetTickCount();
		if (!sqlWritePrivateProfileInt(L"APP", L"KEY", d, L"file.ini"))
		{
			std::cerr << "WRITE ERROR" << std::endl;
			
		}

		
		int f = sqlGetPrivateProfileInt(L"APP", L"KEY", 0, L"file.ini");
		if(d != f)
		{
			std::cerr << "RACING ERROR: " << d << "!=" << f << std::endl;
		}
		if(f==0)
		{ 
			std::cerr << "CRITICAL ERROR: " << d << "!=" << f << std::endl;
		}
		std::cout << i << std::endl;
	}

	return 0;
}


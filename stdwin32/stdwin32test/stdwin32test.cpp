// stdwin32test.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "../stdwin32.h"

int main(int argc, char* argv[])
{
	std::string s=	stdwin32::string_format("%d-%s", 100, "aaa");
	return 0;
}

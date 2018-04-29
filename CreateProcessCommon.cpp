//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include "CreateProcessCommon.h"

static LPTSTR copystring(LPCTSTR p)
{
	if (p == NULL)
		return NULL;

	int len = (lstrlen(p) + 1) * sizeof(TCHAR);
	LPTSTR q = (LPTSTR)LocalAlloc(LMEM_FIXED, len);
	if (q == NULL)
		return NULL;
	lstrcpy(q, p);
	return q;
}
static void freestring(LPCTSTR q)
{
	LocalFree((HLOCAL)q);
}

static LPTSTR createcommandline(LPCTSTR a, LPCTSTR b)
{
	if (a == NULL && b == NULL)
		return NULL;

	if (a == NULL)
		a = L"";
	if (b == NULL)
		b = L"";

	int lenA = (lstrlen(a)) * sizeof(TCHAR);
	int lenB = (lstrlen(b)) * sizeof(TCHAR);


	int len = 0;
	if (lenA)
		len += lenA + sizeof(TCHAR);
	if(lenB)
		len += lenB + sizeof(TCHAR);

	LPTSTR q = (LPTSTR)LocalAlloc(LMEM_FIXED, len);
	if (q == NULL)
		return NULL;
	*q = 0;
	if (*a)
	{
		lstrcat(q, a);
		if(*b)
			lstrcat(q, L" ");
	}
	lstrcat(q, b);

	return q;
}

BOOL CreateProcessCommon(LPCTSTR pApp, 
						 LPCTSTR pArg, // =NULL,
						 BOOL bHide, //=FALSE
						 DWORD* pdwLastError,
						 WaitProcessType wpt,
						 DWORD dwMaxWait,
						HANDLE* phProcess
						 )
{
	STARTUPINFO si = {0};
	
	// The size of the structure, in bytes.
	si.cb = sizeof(si);
	
	// コンソールのタイトル
	// si.lpTitle = NULL;

	// ウインドウがデフォルトで作られた時の表示位置、STARTF_USEPOSITIONを指定した場合指定
	// si.dwX=0;
	// si.dwY=0;

	// ウインドウがデフォルトでつくられた時のサイズ、STARTF_USESIZEを指定した場合指定
	// si.dwXSize = 0;
	// si.dwYSize = 0;


	// コンソールのキャラクターバッファ、STARTF_USECOUNTCHARSを指定した場合指定
	// si.dwXCountChars = 0;
	// si.dwYCountChars = 0;


	// コンソールの文字の色と背景の色、STARTF_USEFILLATTRIBUTEを指定した場合指定
	// si.dwFillAttribute = FOREGROUND_RED| BACKGROUND_RED| BACKGROUND_GREEN| BACKGROUND_BLUE;

	// フラグ（一部）
	si.dwFlags =
		//STARTF_FORCEONFEEDBACK			| // 普通の待機マウスカーソル
		//STARTF_FORCEOFFFEEDBACK			| // 待機マウスカーソルなし
		//STARTF_PREVENTPINNING				| // タスクバーにピンさせない、STARTF_TITLEISAPPIDと共に使う
		//STARTF_RUNFULLSCREEN				| // フルスクリーン、x86コンソールでのみ有効
		//STARTF_TITLEISAPPID				| // lpTitleはAppUserModeIDを設定、タスクバーの同一性情報
		STARTF_USESHOWWINDOW				| // SW_HIDEとかを指定
		//STARTF_USESTDHANDLES				| // ハンドル継承関係
		0;
	if(bHide)
	{
		si.dwFlags |= STARTF_USESHOWWINDOW;
	}


	// ウインドウの表示形態、STARTF_USESHOWWINDOWと共に使う
	// si.wShowWindow = SW_SHOWDEFAULT;
	// si.wShowWindow = SW_HIDE;
	if(bHide)
	{
		si.wShowWindow = SW_HIDE;
	}
	else
	{
		si.wShowWindow = SW_SHOWNORMAL;
	}




	// 出力用、設定不要
	PROCESS_INFORMATION pi = {0};


	LPTSTR cl = createcommandline(pApp, pArg);

	BOOL bRet = CreateProcess(
		 //LPCTSTR lpApplicationName, 
		 // 実行可能モジュールの名前、NULLでもいい
		NULL,

		// LPTSTR lpCommandLine,
		// コマンドラインの文字列、NULLでもいい
		// コンストじゃだめ
		cl,

		// LPSECURITY_ATTRIBUTES lpProcessAttributes,
		// セキュリティ記述子
		NULL,


		// LPSECURITY_ATTRIBUTES lpThreadAttributes,
		// セキュリティ記述子
		NULL,

		// BOOL bInheritHandles,
		// ハンドルの継承オプション
		FALSE,

		// DWORD dwCreationFlags, 
		// 作成のフラグ（一部）
		//CREATE_DEFAULT_ERROR_MODE		| // エラーモードを継承しない
		//CREATE_NO_WINDOW				| // コンソールウインドウを表示しない
		//CREATE_SUSPENDED				| // 起動させないでサスペンドする
		//IDLE_PRIORITY_CLASS			| // プロセスの優先順位
		0,

		// LPVOID lpEnvironment,
		// 新しい環境ブロック
		NULL,


		// LPCTSTR lpCurrentDirectory, 
		// カレントディレクトリの名前
		NULL,

	
		// LPSTARTUPINFO lpStartupInfo,
		// スタートアップ情報
		&si,


		// LPPROCESS_INFORMATION lpProcessInformation
		// プロセス情報
		&pi
	);

	if(pdwLastError)
		*pdwLastError = GetLastError();

	freestring(cl);

	if(!bRet)
	{
		return FALSE;
	}

	// Wait if necessary
	if(wpt==WaitProcess_InputIdle)
		WaitForInputIdle(pi.hProcess, dwMaxWait);
	else if(wpt==WaitProcess_Complete)
		WaitForSingleObject(pi.hProcess, dwMaxWait);

	// ハンドルを閉じる
	CloseHandle(pi.hThread);

	if (phProcess)
		*phProcess = pi.hProcess;
	else
		CloseHandle(pi.hProcess);


	return TRUE;
}

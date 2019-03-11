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

#pragma warning(disable :4786)
#include "stdafx.h"
#include <cassert>
#include <string>
#include <memory>
#include <stlsoft/smartptr/scoped_handle.hpp>
#include "sqlite3.h"
#include "IsFileExists.h"
#include "UTF16toUTF8.h"
#include "sqliteserialize.h"

#include "DebugNew.h"

using namespace std;
// #define IFDISABLED_RETURN do { if(smDisabled_){ return; } } while(false)
#define SAFE_SQLERROR_FREE(s) do { if(s){sqlite3_free(s);     (s)=NULL;} }while(false)
#define SAFE_SQLFINALIZE(s)   do { if(s){sqlite3_finalize(s); (s)=NULL;} }while(false)


namespace Ambiesoft {

	using std::wstring;
	using std::vector;




	// static sqlite3* db_;

	static void cfree(char*p)
	{
		free((void*)p);
	}

	static int callbackt(void *pString, int argc, char **argv, char **azColName)
	{
		std::string* pResult = (std::string*)pString;
		for (int i = 0; i < argc; i++)
		{
			std::string strt;
			strt = argv[i] ? argv[i] : "NULL";
			(*pResult) += strt;
			if ((i + 1) < argc)
				(*pResult) += "\t";
		}
		(*pResult) += "\r\n";
		return 0;
	}

	static BOOL createDB(LPCWSTR pDBPath, LPCWSTR pTableNameW)
	{
		//	if(!InitSqliteLibrary())
		//		return FALSE;

		unique_ptr<char> pTableNamePtr(UTF16toUTF8_new(pTableNameW));
		const char* pTableName = pTableNamePtr.get();
		// stlsoft::scoped_handle<char*> tab(pTableName);// , cfree);

		char* pErr = NULL;
		sqlite3* pDB = NULL;
		std::string strResult;
		BOOL ret = FALSE;

		do
		{
			LPCSTR p;

			if (SQLITE_OK != sqlite3_open16(pDBPath, &pDB))
			{
				break;
			}


			p =
				"CREATE TABLE [%s] ( "
				"[c1] TEXT, "
				"[c2] TEXT,"
				"PRIMARY KEY([c1]));";

			size_t malsize = strlen(p) + strlen(pTableName) + sizeof(char);
			char* pT = (char*)malloc(malsize);
			stlsoft::scoped_handle<char*> ma(pT, cfree);

#if _MSC_VER < 1500
			sprintf(pT, p, pTableName);
#else
			sprintf_s(pT, malsize, p, pTableName);
#endif

			if (SQLITE_OK != sqlite3_exec(pDB,
				pT,
				callbackt,
				&strResult,
				&pErr) || pErr)
			{
				break;
			}
			SAFE_SQLERROR_FREE(pErr);








			/**
					if ( SQLITE_OK != fn_sqlite3_exec(pDB,
					"insert into [group] values('aaa','bbb')",
					callbackt,
					&strResult,
					&pErr) || pErr)
					{
					break;
					}
					**/









			ret = TRUE;
		} while (0);

		SAFE_SQLERROR_FREE(pErr);

		if (pDB)
		{
			sqlite3_close(pDB);
			pDB = NULL;
		}

		return ret;
	}


	int infinite_sqlite3_prepare16_v2(
		sqlite3 *db,              /* Database handle. */
		LPCWSTR zSql,         /* UTF-16 encoded SQL statement. */
		int nBytes,               /* Length of zSql in bytes. */
		sqlite3_stmt **ppStmt,    /* OUT: A pointer to the prepared statement */
		const void **pzTail       /* OUT: End of parsed string */
		)
	{
		int ret;
		for (;;)
		{
			ret = sqlite3_prepare16_v2(
				db,
				zSql,
				nBytes,
				ppStmt,
				pzTail
				);
			if (ret != SQLITE_BUSY)
				break;

			Sleep(100);
		}
		return ret;
	}
	int inifinite_sqlite3_step(sqlite3_stmt *pStmt) {
		int ret;
		for (;;) {
			ret = sqlite3_step(pStmt);
			if (ret != SQLITE_BUSY)
				break;

			Sleep(100);
		}
		return ret;
	}


	struct stTransactionScope
	{

		static int cc_;

	private:
		static sqlite3_stmt* stmtBegin_;
		static sqlite3_stmt* stmtCommit_;
		sqlite3* pDB_;
	public:
		stTransactionScope(sqlite3* pDB) : pDB_(pDB)
		{

			++cc_;
			assert(cc_==1);

			assert(pDB);

			// DVERIFY_IS(fn_sqlite3_exec(db_, "BEGIN TRANSACTION", NULL, NULL, NULL), SQLITE_OK);
			if (!stmtBegin_)
			{
				if (SQLITE_OK != sqlite3_prepare16_v2(pDB,
					L"BEGIN TRANSACTION",
					-1,
					&stmtBegin_,
					0) || !stmtBegin_)
				{
					// ReportSQLError(__FILE__,__LINE__);
					return;
				}
			}
			if (!stmtCommit_)
			{
				if (SQLITE_OK != sqlite3_prepare16_v2(pDB,
					L"COMMIT;",
					-1,
					&stmtCommit_,
					0) || !stmtCommit_)
				{
					// ReportSQLError(__FILE__,__LINE__);
					return;
				}
			}


			if (SQLITE_DONE != inifinite_sqlite3_step(stmtBegin_))
			{
				// ReportSQLError(__FILE__,__LINE__);
			}

		}
		~stTransactionScope()
		{

			--cc_;
			assert(pDB_);
			// DVERIFY_IS(fn_sqlite3_exec(db_, "COMMIT;", NULL, NULL, NULL), SQLITE_OK);


			if (SQLITE_DONE != inifinite_sqlite3_step(stmtCommit_))
			{
				//ReportSQLError(__FILE__,__LINE__);
			}

			if (cc_ == 0)
			{
				Finalize();
			}
		}

		static void Finalize()
		{
			SAFE_SQLFINALIZE(stmtBegin_);
			SAFE_SQLFINALIZE(stmtCommit_);
		}
	};

	int stTransactionScope::cc_;

	sqlite3_stmt* stTransactionScope::stmtBegin_;
	sqlite3_stmt* stTransactionScope::stmtCommit_;

	//struct stTransactionScopeFinalizer
	//{
	//	~stTransactionScopeFinalizer()
	//	{
	//		stTransactionScope::Finalize();
	//	}
	//};
	//static stTransactionScopeFinalizer gsTransactionScopeFinalizer;

	/*
	static sqlite3_stmt* getInsertStatement(sqlite3* pDB,LPCWSTR pApp)
	{
	static std::map<std::wstring, sqlite3_stmt*> theInsertMap;
	sqlite3_stmt* pRet = theInsertMap[pApp];
	if(!pRet)
	{
	LPCWSTR pKata = L"INSERT OR REPLACE INTO [%s] VALUES (?, ?);";
	LPWSTR p = (LPWSTR)malloc(wcslen(pKata)*2 + wcslen(pApp)*2 + 2);
	stlsoft::scoped_handle<void*> mahh( (void*)p, free);
	wsprintf(p, pKata, pApp);
	int ret = sqlite3_prepare16_v2(pDB,
	p,
	-1,
	&pRet,
	0);

	if(ret != SQLITE_OK)
	{
	return NULL;
	}
	theInsertMap[pApp]=pRet;
	}
	return pRet;
	}

	static sqlite3_stmt* getUpdateStatement(sqlite3* pDB,LPCWSTR pApp)
	{
	static std::map<std::wstring, sqlite3_stmt*> theUpdateMap;
	sqlite3_stmt* pRet = theUpdateMap[pApp];
	if(!pRet)
	{
	LPCWSTR pKata = L"UPDATE [%s] SET c2=? WHERE c1=?;";
	LPWSTR p = (LPWSTR)malloc(wcslen(pKata)*2 + wcslen(pApp)*2 + 2);
	stlsoft::scoped_handle<void*> mahh( (void*)p, free);
	wsprintf(p, pKata, pApp);
	int ret = sqlite3_prepare16_v2(pDB,
	p,
	-1,
	&pRet,
	0);

	if(ret != SQLITE_OK)
	{
	return NULL;
	}
	theUpdateMap[pApp]=pRet;
	}
	return pRet;
	}
	*/

	BOOL sqlWritePrivateProfileString(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		LPCWSTR lpString,
		LPCWSTR lpFileName)
	{

		if (!IsFileExistsW(lpFileName))
		{
			if (!createDB(lpFileName, lpAppName))
			{
				return FALSE;
			}
		}

		sqlite3* pDB = NULL;

		if (SQLITE_OK != sqlite3_open16(lpFileName, &pDB))
			return FALSE;
		stlsoft::scoped_handle<sqlite3*> ma(pDB, sqlite3_close);

		// sqlite3_busy_timeout(pDB, 3000);

		int sqRet = 0;
		{
			stTransactionScope stts(pDB);

			sqlite3_stmt* pStmtInsert = NULL;
			if (pStmtInsert == NULL)
			{
				LPCWSTR pKata = L"INSERT OR REPLACE INTO [%s] VALUES (?, ?);";
				LPWSTR p = (LPWSTR)malloc(wcslen(pKata) * 2 + wcslen(lpAppName) * 2 + 2);
				stlsoft::scoped_handle<void*> mahh((void*)p, free);
				wsprintf(p, pKata, lpAppName);
				int ret = infinite_sqlite3_prepare16_v2(pDB,
					p,
					-1,
					&pStmtInsert,
					0);
				if (SQLITE_OK != ret)
					return FALSE;

				if (!pStmtInsert)
					return FALSE;
			}
			stlsoft::scoped_handle<sqlite3_stmt*> mastml(pStmtInsert, sqlite3_finalize);


			if (SQLITE_OK != sqlite3_reset(pStmtInsert))
				return FALSE;

			int index = 0;
			if (SQLITE_OK != sqlite3_bind_text16(pStmtInsert, ++index, lpKeyName, -1, SQLITE_STATIC))
				return FALSE;
			if (SQLITE_OK != sqlite3_bind_text16(pStmtInsert, ++index, lpString, -1, SQLITE_STATIC))
				return FALSE;

			sqRet = inifinite_sqlite3_step(pStmtInsert);

			assert(sqRet == SQLITE_DONE);
		}

		/*
			if(SQLITE_DONE != sqRet)
			{
			stTransactionScope stts(pDB);

			sqlite3_stmt* pStmtUpdate = getUpdateStatement(pDB,lpAppName);

			if(SQLITE_OK != sqlite3_reset(pStmtUpdate))
			return FALSE;

			int index=0;
			if(SQLITE_OK != sqlite3_bind_text16(pStmtUpdate, ++index, lpString, -1, SQLITE_STATIC))
			return FALSE;

			if(SQLITE_OK != sqlite3_bind_text16(pStmtUpdate, ++index, lpKeyName, -1, SQLITE_STATIC))
			return FALSE;

			if(SQLITE_DONE != sqlite3_step(pStmtUpdate))
			return FALSE;
			}
			*/
		return SQLITE_DONE == sqRet;
	}

	BOOL sqlGetPrivateProfileString(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		LPCWSTR lpDefault,
		LPWSTR lpReturnedString,
		DWORD nSize,
		LPCWSTR lpFileName)
	{
		lstrcpyn(lpReturnedString, lpDefault, nSize);

		if (!IsFileExistsW(lpFileName))
		{
			return FALSE;
		}

		sqlite3* pDB = NULL;

		if (SQLITE_OK != sqlite3_open16(lpFileName, &pDB))
			return FALSE;
		stlsoft::scoped_handle<sqlite3*> ma(pDB, sqlite3_close);

		stTransactionScope stts(pDB);

		sqlite3_stmt* pStmtSelect = NULL;
		if (!pStmtSelect)
		{
			LPCWSTR pKata = L"SELECT c2 FROM [%s] WHERE c1 = ?;";
			LPWSTR p = (LPWSTR)malloc(wcslen(pKata) * 2 + wcslen(lpAppName) * 2 + 2);
			stlsoft::scoped_handle<void*> mahh((void*)p, free);
			wsprintf(p, pKata, lpAppName);
			int ret = infinite_sqlite3_prepare16_v2(pDB,
				p,
				-1,
				&pStmtSelect,
				0);

			if (ret != SQLITE_OK)
				return FALSE;

			if (!pStmtSelect)
				return FALSE;
		}
		stlsoft::scoped_handle<sqlite3_stmt*> mastml(pStmtSelect, sqlite3_finalize);

		if (SQLITE_OK != sqlite3_reset(pStmtSelect))
			return FALSE;

		int index = 0;
		if (SQLITE_OK != sqlite3_bind_text16(pStmtSelect, ++index, lpKeyName, -1, SQLITE_STATIC))
			return FALSE;

		int sqRet = inifinite_sqlite3_step(pStmtSelect);
		if (sqRet != SQLITE_ROW && sqRet != SQLITE_DONE)
		{
			return FALSE;
		}

		LPWSTR pRet = (LPWSTR)sqlite3_column_text16(pStmtSelect, 0);
		if (!pRet)
			return FALSE;

		lstrcpyn(lpReturnedString, pRet, nSize);

		return TRUE;
	}

	BOOL sqlWritePrivateProfileInt(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		int nData,
		LPCWSTR lpFileName)
	{
		TCHAR szT[32];
		wsprintf(szT, L"%d", nData);
		return sqlWritePrivateProfileString(
			lpAppName,
			lpKeyName,
			szT,
			lpFileName);
	}

	UINT sqlGetPrivateProfileInt(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		INT nDefault,
		LPCWSTR lpFileName
		)
	{
		TCHAR szT[32];
		if (!sqlGetPrivateProfileString(
			lpAppName,
			lpKeyName,
			L"",
			szT,
			sizeof(szT) / sizeof(szT[0]),
			lpFileName))
		{
			return nDefault;
		}

		return _wtoi(szT);
	}












	static wstring tostring(size_t i)
	{
		WCHAR szT[8];
		wsprintf(szT, L"%d", i);
		return szT;
	}

#if (_MSC_VER <= 1400)
	typedef WCHAR* RPC_WSTR;
#endif

	static wstring getuuidstring()
	{
		UUID uuid;
		UuidCreate(&uuid);
		WCHAR* pS;
		UuidToStringW(&uuid, (RPC_WSTR*)&pS);
		wstring ret((WCHAR*)pS);
		RpcStringFreeW((RPC_WSTR*)&pS);
		return ret;
	}
#pragma comment(lib,"Rpcrt4.lib")

	BOOL sqlGetPrivateProfileStringArray(LPCWSTR pApp, LPCWSTR pKey, vector<wstring>& ss, LPCWSTR pIni)
	{
		if (pApp == NULL || pApp[0] == 0)
			return FALSE;

		if (pKey == NULL || pKey[0] == 0)
			return FALSE;

		const int buffcharsize = 4096;
		TCHAR* pBuff = (TCHAR*)malloc(buffcharsize * sizeof(TCHAR));
		for (size_t i = 0;; ++i)
		{
			wstring key(pKey);
			if (i != 0)
				key += tostring(i);

			wstring us = getuuidstring();
			DWORD ret = sqlGetPrivateProfileString(
				pApp,
				key.c_str(),
				us.c_str(),
				pBuff,
				buffcharsize,
				pIni);

			if (us == pBuff)
				break;

			ss.push_back(pBuff);
		}
		free(pBuff);
		return TRUE;
	}
	BOOL sqlWritePrivateProfileStringArray(LPCWSTR pApp, LPCWSTR pKey, vector<wstring>& ss, LPCWSTR pIni)
	{
		BOOL bFailed = FALSE;
		const size_t count = ss.size();
		for (size_t i = 0; i < count; ++i)
		{
			wstring key(pKey);
			if (i != 0)
			{
				key += tostring(i);
			}

			bFailed |= !sqlWritePrivateProfileString(pApp, key.c_str(), ss[i].c_str(), pIni);
		}
		return !bFailed;
	}


} // namespace
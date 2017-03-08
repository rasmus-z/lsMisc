#pragma warning(disable :4786)
#include "stdafx.h"
#include <cassert>
#include <stlsoft/smartptr/scoped_handle.hpp>
#include "sqlite3.h"
#include "sqliteserialize.h"
#include "IsFileExists.h"
#include "UTF16toUTF8.h"



























// #define IFDISABLED_RETURN do { if(smDisabled_){ return; } } while(false)
#define SAFE_SQLERROR_FREE(s) do { if(s){sqlite3_free(s);     (s)=NULL;} }while(false)
#define SAFE_SQLFINALIZE(s)   do { if(s){sqlite3_finalize(s); (s)=NULL;} }while(false)

// static sqlite3* db_;

static void cfree(char*p)
{
	free((void*)p);
}

static int callbackt(void *pString, int argc, char **argv, char **azColName)
{
	std::string* pResult = (std::string*)pString;
	for(int i=0; i<argc; i++)
	{
		std::string strt;
		strt = argv[i] ? argv[i] : "NULL";
		(*pResult) += strt;
		if( (i+1) < argc )
			(*pResult) += "\t";
	}
	(*pResult) += "\r\n";
	return 0;
}

static BOOL createDB(LPCWSTR pDBPath, LPCWSTR pTableNameW)
{
//	if(!InitSqliteLibrary())
//		return FALSE;

	char* pTableName = (char*)UTF16toUTF8(pTableNameW);
	stlsoft::scoped_handle<char*> tab(pTableName, cfree);	

	char* pErr = NULL;
	sqlite3* pDB=NULL;
	std::string strResult;
	BOOL ret = FALSE;

	do
	{
		LPCSTR p;

		if ( SQLITE_OK != sqlite3_open16(pDBPath, &pDB) )
		{
			break;
		}


		p = 
			"CREATE TABLE [%s] ( "
			"[c1] TEXT, "
			"[c2] TEXT,"
			"PRIMARY KEY([c1]));";

		char* pT = (char*)malloc(strlen(p) + strlen(pTableName) + sizeof(char));
		stlsoft::scoped_handle<char*> ma(pT, cfree);	

		sprintf(pT, p, pTableName);

		if ( SQLITE_OK != sqlite3_exec(pDB, 
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
	} while(0);

	SAFE_SQLERROR_FREE(pErr);

	if ( pDB )
	{
		sqlite3_close(pDB);
		pDB = NULL;
	}

	return ret;
}




struct stTransactionScope
{
#ifdef _DEBUG
	static int cc_;
#endif


private:
	static sqlite3_stmt* stmtBegin_;
	static sqlite3_stmt* stmtCommit_;
	sqlite3* pDB_;
public:
	stTransactionScope(sqlite3* pDB)
	{
#ifdef _DEBUG
		++cc_;
		assert(cc_==1);
#endif
		assert(pDB);
		pDB_ = pDB;
		// DVERIFY_IS(fn_sqlite3_exec(db_, "BEGIN TRANSACTION", NULL, NULL, NULL), SQLITE_OK);
		if(!stmtBegin_)
		{
			if(SQLITE_OK != sqlite3_prepare_v2(pDB,
				"BEGIN TRANSACTION",
				-1,
				&stmtBegin_,
				0) || !stmtBegin_)
			{
				// ReportSQLError(__FILE__,__LINE__);
				return;
			}
		}
		if(!stmtCommit_)
		{
			if(SQLITE_OK != sqlite3_prepare_v2(pDB,
				"COMMIT;",
				-1,
				&stmtCommit_,
				0) || !stmtCommit_)
			{
				// ReportSQLError(__FILE__,__LINE__);
				return;
			}
		}

		
		if(SQLITE_DONE != sqlite3_step(stmtBegin_))
		{
			// ReportSQLError(__FILE__,__LINE__);
		}

	}
	~stTransactionScope()
	{
#ifdef _DEBUG
		--cc_;
#endif
		assert(pDB_);
		// DVERIFY_IS(fn_sqlite3_exec(db_, "COMMIT;", NULL, NULL, NULL), SQLITE_OK);

		
		if(SQLITE_DONE != sqlite3_step(stmtCommit_))
		{
			//ReportSQLError(__FILE__,__LINE__);
		}
	}

	static void Finalize()
	{
		SAFE_SQLFINALIZE(stmtBegin_);
		SAFE_SQLFINALIZE(stmtCommit_);
	}
};
#ifdef _DEBUG
int stTransactionScope::cc_;
#endif
sqlite3_stmt* stTransactionScope::stmtBegin_;
sqlite3_stmt* stTransactionScope::stmtCommit_;
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
  LPCTSTR lpAppName,
  LPCTSTR lpKeyName,
  LPCTSTR lpString,
  LPCTSTR lpFileName )
{
	
	if(!IsFileExistsW(lpFileName))
	{
		if(!createDB(lpFileName, lpAppName))
		{
			return FALSE;
		}
	}

	sqlite3* pDB=NULL;

	if(SQLITE_OK != sqlite3_open16(lpFileName, &pDB))
		return FALSE;
	stlsoft::scoped_handle<sqlite3*> ma(pDB, sqlite3_close);

	// sqlite3_busy_timeout(pDB, 3000);

	int sqRet=0;
	{
		stTransactionScope stts(pDB);

		sqlite3_stmt* pStmtInsert = NULL;
		LPCWSTR pKata = L"INSERT OR REPLACE INTO [%s] VALUES (?, ?);";
		LPWSTR p = (LPWSTR)malloc(wcslen(pKata)*2 + wcslen(lpAppName)*2 + 2);
		stlsoft::scoped_handle<void*> mahh( (void*)p, free);
		wsprintf(p, pKata, lpAppName);
		int ret = sqlite3_prepare16_v2(pDB,
			p,
			-1,
			&pStmtInsert,
			0);
		if(SQLITE_OK != ret)
			return FALSE;
		stlsoft::scoped_handle<sqlite3_stmt*> mastml( pStmtInsert, sqlite3_finalize);


		if(SQLITE_OK != sqlite3_reset(pStmtInsert))
			return FALSE;

		int index=0;
		if(SQLITE_OK != sqlite3_bind_text16(pStmtInsert, ++index, lpKeyName, -1, SQLITE_STATIC))
			return FALSE;
		if(SQLITE_OK != sqlite3_bind_text16(pStmtInsert, ++index, lpString, -1, SQLITE_STATIC))
			return FALSE;

		sqRet = sqlite3_step(pStmtInsert);

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
    LPCTSTR lpAppName,
    LPCTSTR lpKeyName,
    LPCTSTR lpDefault,
    LPTSTR lpReturnedString,
    DWORD nSize,
    LPCWSTR lpFileName)
{
	lstrcpyn(lpReturnedString, lpDefault, nSize);

	if(!IsFileExistsW(lpFileName))
	{
		return FALSE;
	}

	sqlite3* pDB=NULL;

	if(SQLITE_OK != sqlite3_open16(lpFileName, &pDB))
		return FALSE;
	stlsoft::scoped_handle<sqlite3*> ma(pDB, sqlite3_close);

	stTransactionScope stts(pDB);

	sqlite3_stmt* pStmtSelect = NULL;
	LPCWSTR pKata = L"SELECT c2 FROM [%s] WHERE c1 = ?;";
	LPWSTR p = (LPWSTR)malloc(wcslen(pKata)*2 + wcslen(lpAppName)*2 + 2);
	stlsoft::scoped_handle<void*> mahh( (void*)p, free);
	wsprintf(p, pKata, lpAppName);
	int ret = sqlite3_prepare16_v2(pDB,
		p,
		-1,
		&pStmtSelect,
		0);

	if(ret != SQLITE_OK)
		return FALSE;
	stlsoft::scoped_handle<sqlite3_stmt*> mastml(pStmtSelect, sqlite3_finalize);
	

	if(SQLITE_OK != sqlite3_reset(pStmtSelect))
		return FALSE;

	int index=0;
	if(SQLITE_OK != sqlite3_bind_text16(pStmtSelect, ++index, lpKeyName, -1, SQLITE_STATIC))
		return FALSE;

	int sqRet = sqlite3_step(pStmtSelect);
	if(sqRet != SQLITE_ROW && sqRet != SQLITE_DONE)
	{
		return FALSE;
	}

	LPWSTR pRet = (LPWSTR)sqlite3_column_text16(pStmtSelect, 0);
	if(!pRet)
		return FALSE;

	lstrcpyn(lpReturnedString, pRet, nSize);

	return TRUE;
}

BOOL sqlWritePrivateProfileInt(
  LPCTSTR lpAppName,
  LPCTSTR lpKeyName,
  int nData,
  LPCTSTR lpFileName )
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
	if(!sqlGetPrivateProfileString(
		lpAppName,
		lpKeyName,
		L"",
		szT,
		sizeof(szT)/sizeof(szT[0]),
		lpFileName))
	{
		return nDefault;
	}

	return _wtoi(szT);
}
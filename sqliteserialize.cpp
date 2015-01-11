#include <windows.h>
#include <assert.h>
#include <string>
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
//			CString message = (LPCTSTR)I18N(L"Failed to create group file");
//			message += L" : ";
//			message += m_pszGroupDBName;
//			AfxMessageBox(message);
			return FALSE;
		}
	}

	sqlite3* pDB=NULL;

	if(SQLITE_OK != sqlite3_open16(lpFileName, &pDB))
		return FALSE;
	stlsoft::scoped_handle<sqlite3*> ma(pDB, sqlite3_close);
/*
	{
		stTransactionScope stts(pDB);
		static sqlite3_stmt* pStmtUpdate = NULL;
		if(pStmtUpdate==NULL)
		{
			LPCWSTR pKata = L"UPDATE [%s] SET c2=?2 WHERE c1=?1 ;";
			LPWSTR p = (LPWSTR)malloc(wcslen(pKata)*2 + wcslen(lpAppName)*2 + 2);
			stlsoft::scoped_handle<void*> mahh( (void*)p, free);
			wsprintf(p, pKata, lpAppName);
			int ret = sqlite3_prepare16_v2(pDB,
				p, // L"insert into group values('?','?')",
				-1,
				&pStmtUpdate,
				0);

			if(ret != SQLITE_OK)
			{
				return FALSE;
			}
		}
		if(SQLITE_OK != sqlite3_reset(pStmtUpdate))
			return FALSE;

		if(SQLITE_OK != sqlite3_bind_text16(pStmtUpdate, 1, lpKeyName, -1, SQLITE_STATIC))
			return FALSE;

		if(SQLITE_OK != sqlite3_bind_text16(pStmtUpdate, 2, lpString, -1, SQLITE_STATIC))
			return FALSE;

		if(SQLITE_DONE != sqlite3_step(pStmtUpdate))
			return FALSE;
	}
	
	if(sqlite3_changes(pDB)==1)
	{
		return TRUE;
	}
	
*/



	stTransactionScope stts(pDB);

	static sqlite3_stmt* pStmtInsert = NULL;
	if(pStmtInsert==NULL)
	{
		LPCWSTR pKata = L"INSERT OR REPLACE INTO [%s] VALUES (?, ?);";
		LPWSTR p = (LPWSTR)malloc(wcslen(pKata)*2 + wcslen(lpAppName)*2 + 2);
		stlsoft::scoped_handle<void*> mahh( (void*)p, free);
		wsprintf(p, pKata, lpAppName);
		int ret = sqlite3_prepare16_v2(pDB,
			p, // L"insert into group values('?','?')",
			-1,
			&pStmtInsert,
			0);

		if(ret != SQLITE_OK)
		{
			return FALSE;
		}
	}
	
	if(SQLITE_OK != sqlite3_reset(pStmtInsert))
		return FALSE;

	if(SQLITE_OK != sqlite3_reset(pStmtInsert))
		return FALSE;
	if(SQLITE_OK != sqlite3_bind_text16(pStmtInsert, 1, lpKeyName, -1, SQLITE_STATIC))
		return FALSE;
	if(SQLITE_OK != sqlite3_bind_text16(pStmtInsert, 2, lpString, -1, SQLITE_STATIC))
		return FALSE;

	if(SQLITE_DONE != sqlite3_step(pStmtInsert))
	{
		return FALSE;
	}

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

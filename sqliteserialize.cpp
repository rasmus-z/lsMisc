#include <windows.h>
#include <string>
#include <stlsoft/smartptr/scoped_handle.hpp>
#include "sqlite3.h"
#include "sqliteserialize.h"

using namespace std;

typedef int						(* SQLITE_API SQLITEFUNC_OPEN)			(const char *, sqlite3 **);
typedef int						(* SQLITE_API SQLITEFUNC_OPEN16)			(const void*, sqlite3 **);
typedef unsigned int			(* SQLITE_API SQLITEFUNC_EXEC)			(sqlite3*, const char *, sqlite3_callback, void *, char **);
typedef unsigned int			(* SQLITE_API SQLITEFUNC_CLOSE)			(sqlite3 *);
typedef int					    (* SQLITE_API SQLITEFUNC_CHANGES)         (sqlite3*);
typedef int						(* SQLITE_API SQLITEFUNC_RESET)           (sqlite3_stmt*);
typedef int						(* SQLITE_API SQLITEFUNC_COLUMN_COUNT)	(sqlite3_stmt*);
typedef int						(* SQLITE_API SQLITEFUNC_PREPARE)			(sqlite3*, const char*, int, sqlite3_stmt**, const char**);
typedef int						(* SQLITE_API SQLITEFUNC_FINALIZE)		(sqlite3_stmt*);
typedef const char *			(* SQLITE_API SQLITEFUNC_COLUMN_NAME)		(sqlite3_stmt*, int iCol);
typedef int						(* SQLITE_API SQLITEFUNC_COLUMN_TYPE)		(sqlite3_stmt*, int iCol);
typedef int						(* SQLITE_API SQLITEFUNC_STEP)			(sqlite3_stmt*);
typedef int						(* SQLITE_API SQLITEFUNC_COLUMN_INT)		(sqlite3_stmt*, int iCol);
typedef const unsigned char *	(* SQLITE_API SQLITEFUNC_COLUMN_TEXT)		(sqlite3_stmt*, int iCol);
typedef const void *			(* SQLITE_API SQLITEFUNC_COLUMN_BLOB)		(sqlite3_stmt*, int iCol);
typedef double					(* SQLITE_API SQLITEFUNC_COLUMN_DOUBLE)	(sqlite3_stmt*, int iCol);

typedef int					    (* SQLITE_API SQLITEFUNC_BIND_BLOB)       (sqlite3_stmt*, int, const void*, int n, void(*)(void*));
typedef int                     (* SQLITE_API SQLITEFUNC_BIND_DOUBLE)     (sqlite3_stmt*, int, double);
typedef int				        (* SQLITE_API SQLITEFUNC_BIND_INT)        (sqlite3_stmt*, int, int);
typedef int				        (* SQLITE_API SQLITEFUNC_BIND_INT64)      (sqlite3_stmt*, int, sqlite_int64);
typedef int				        (* SQLITE_API SQLITEFUNC_BIND_NULL)       (sqlite3_stmt*, int);
typedef int				        (* SQLITE_API SQLITEFUNC_BIND_TEXT)       (sqlite3_stmt*, int, const char*, int n, void(*)(void*));
typedef int				        (* SQLITE_API SQLITEFUNC_BIND_TEXT16)     (sqlite3_stmt*, int, const void*, int, void(*)(void*));
typedef int				        (* SQLITE_API SQLITEFUNC_BIND_VALUE)      (sqlite3_stmt*, int, const sqlite3_value*);
typedef int						(* SQLITE_API SQLITEFUNC_EXTENDED_ERRORCODE) (sqlite3 *db);
typedef const char*				(* SQLITE_API SQLITEFUNC_ERRSTR)			(int);
typedef const void *			(* SQLITE_API SQLITEFUNC_COLUMN_TEXT16)	(sqlite3_stmt*, int iCol);
typedef int						(* SQLITE_API SQLITEFUNC_COLUMN_INT)		(sqlite3_stmt *,int);
typedef void					(* SQLITE_API SQLITEFUNC_FREE)				(void*);
typedef int						(* SQLITE_API SQLITEFUNC_PREPARE16_V2) (
  sqlite3 *,            /* Database handle */
  const void *,       /* SQL statement, UTF-16 encoded */
  int ,              /* Maximum length of zSql in bytes. */
  sqlite3_stmt **,  /* OUT: Statement handle */
  const void **     /* OUT: Pointer to unused portion of zSql */
);
typedef int						(* SQLITE_API SQLITEFUNC_PREPARE_V2) (
  sqlite3 *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
typedef int						(* SQLITE_API SQLITEFUNC_CLEAR_BINDINGS)	(sqlite3_stmt*);
typedef const char*				(* SQLITE_API SQLITEFUNC_LIBVERSION) (void);
typedef const char*				(* SQLITE_API SQLITEFUNC_SOURCEID) (void);
typedef const void *			(* SQLITE_API SQLITEFUNC_ERRMSG16) (sqlite3*);



static SQLITEFUNC_OPEN fn_sqlite3_open;
static SQLITEFUNC_OPEN16 fn_sqlite3_open16;
static SQLITEFUNC_EXEC fn_sqlite3_exec;
static SQLITEFUNC_CLOSE fn_sqlite3_close;
static SQLITEFUNC_CHANGES fn_sqlite3_changes;
static SQLITEFUNC_RESET fn_sqlite3_reset;
static SQLITEFUNC_COLUMN_COUNT fn_sqlite3_column_count;
static SQLITEFUNC_PREPARE fn_sqlite3_prepare;
static SQLITEFUNC_FINALIZE fn_sqlite3_finalize;
static SQLITEFUNC_COLUMN_NAME fn_sqlite3_column_name;
static SQLITEFUNC_COLUMN_TYPE fn_sqlite3_column_type;
static SQLITEFUNC_STEP fn_sqlite3_step;
static SQLITEFUNC_COLUMN_INT fn_sqlite3_column_int;
static SQLITEFUNC_COLUMN_TEXT fn_sqlite3_column_text;
static SQLITEFUNC_COLUMN_BLOB fn_sqlite3_column_blob;
static SQLITEFUNC_BIND_INT fn_sqlite3_bind_int;
static SQLITEFUNC_BIND_INT64 fn_sqlite3_bind_int64;
static SQLITEFUNC_BIND_TEXT fn_sqlite3_bind_text;
static SQLITEFUNC_BIND_TEXT16 fn_sqlite3_bind_text16;
static SQLITEFUNC_EXTENDED_ERRORCODE fn_sqlite3_extended_errcode;
static SQLITEFUNC_ERRSTR fn_sqlite3_errstr;
static SQLITEFUNC_COLUMN_TEXT16 fn_sqlite3_column_text16;
static SQLITEFUNC_FREE fn_sqlite3_free;
static SQLITEFUNC_PREPARE16_V2 fn_sqlite3_prepare16_v2;
static SQLITEFUNC_PREPARE_V2 fn_sqlite3_prepare_v2;
static SQLITEFUNC_CLEAR_BINDINGS fn_sqlite3_clear_bindings;
static SQLITEFUNC_LIBVERSION fn_sqlite3_libversion;
static SQLITEFUNC_SOURCEID fn_sqlite3_sourceid;
static SQLITEFUNC_ERRMSG16 fn_sqlite3_errmsg16;

// #define IFDISABLED_RETURN do { if(smDisabled_){ return; } } while(false)
#define SAFE_SQLERROR_FREE(s) do { if(s){fn_sqlite3_free(s);     (s)=NULL;} }while(false)
#define SAFE_SQLFINALIZE(s)   do { if(s){fn_sqlite3_finalize(s); (s)=NULL;} }while(false)

static sqlite3* db_;

HMODULE hSqlite3DLL_;

static bool InitSqliteLibrary(void)
{
	hSqlite3DLL_ = LoadLibrary(L"sqlite3");
	if(!hSqlite3DLL_)
	{
		return false;
	}

	fn_sqlite3_open = (SQLITEFUNC_OPEN)GetProcAddress(hSqlite3DLL_, "sqlite3_open");
	fn_sqlite3_open16 = (SQLITEFUNC_OPEN16)GetProcAddress(hSqlite3DLL_, "sqlite3_open16");
	fn_sqlite3_exec = (SQLITEFUNC_EXEC)GetProcAddress(hSqlite3DLL_, "sqlite3_exec");
	fn_sqlite3_close = (SQLITEFUNC_CLOSE)GetProcAddress(hSqlite3DLL_, "sqlite3_close");
	fn_sqlite3_changes = (SQLITEFUNC_CHANGES)GetProcAddress(hSqlite3DLL_, "sqlite3_changes");
	fn_sqlite3_reset = (SQLITEFUNC_RESET)GetProcAddress(hSqlite3DLL_, "sqlite3_reset");
	fn_sqlite3_column_count = (SQLITEFUNC_COLUMN_COUNT)GetProcAddress(hSqlite3DLL_, "sqlite3_column_count");
	fn_sqlite3_prepare = (SQLITEFUNC_PREPARE)GetProcAddress(hSqlite3DLL_, "sqlite3_prepare");
	fn_sqlite3_finalize = (SQLITEFUNC_FINALIZE)GetProcAddress(hSqlite3DLL_, "sqlite3_finalize");
	fn_sqlite3_column_name = (SQLITEFUNC_COLUMN_NAME)GetProcAddress(hSqlite3DLL_, "sqlite3_column_name");
	fn_sqlite3_column_type = (SQLITEFUNC_COLUMN_TYPE)GetProcAddress(hSqlite3DLL_, "sqlite3_column_type");
	fn_sqlite3_step = (SQLITEFUNC_STEP)GetProcAddress(hSqlite3DLL_, "sqlite3_step");
	fn_sqlite3_column_int = (SQLITEFUNC_COLUMN_INT)GetProcAddress(hSqlite3DLL_, "sqlite3_column_int");
	fn_sqlite3_column_text = (SQLITEFUNC_COLUMN_TEXT)GetProcAddress(hSqlite3DLL_, "sqlite3_column_text");
	fn_sqlite3_column_blob = (SQLITEFUNC_COLUMN_BLOB)GetProcAddress(hSqlite3DLL_, "sqlite3_column_blob");
	fn_sqlite3_bind_int	  =	(SQLITEFUNC_BIND_INT	)GetProcAddress(hSqlite3DLL_, "sqlite3_bind_int");
	fn_sqlite3_bind_int64  =	(SQLITEFUNC_BIND_INT64	)GetProcAddress(hSqlite3DLL_, "sqlite3_bind_int64");	
	fn_sqlite3_bind_text	  =	(SQLITEFUNC_BIND_TEXT	)GetProcAddress(hSqlite3DLL_, "sqlite3_bind_text");	
	fn_sqlite3_bind_text16 =	(SQLITEFUNC_BIND_TEXT16)GetProcAddress(hSqlite3DLL_, "sqlite3_bind_text16");	
	fn_sqlite3_extended_errcode = (SQLITEFUNC_EXTENDED_ERRORCODE)GetProcAddress(hSqlite3DLL_, "sqlite3_extended_errcode");
	fn_sqlite3_errstr =			(SQLITEFUNC_ERRSTR)GetProcAddress(hSqlite3DLL_,"sqlite3_errstr");
	fn_sqlite3_column_text16 = (SQLITEFUNC_COLUMN_TEXT16)GetProcAddress(hSqlite3DLL_, "sqlite3_column_text16");
	fn_sqlite3_free = (SQLITEFUNC_FREE)GetProcAddress(hSqlite3DLL_,"sqlite3_free");
	fn_sqlite3_prepare16_v2 = (SQLITEFUNC_PREPARE16_V2)GetProcAddress(hSqlite3DLL_,"sqlite3_prepare16_v2");
	fn_sqlite3_prepare_v2 = (SQLITEFUNC_PREPARE_V2)GetProcAddress(hSqlite3DLL_,"sqlite3_prepare_v2");
	fn_sqlite3_clear_bindings = (SQLITEFUNC_CLEAR_BINDINGS)GetProcAddress(hSqlite3DLL_, "sqlite3_clear_bindings");
	fn_sqlite3_libversion = (SQLITEFUNC_LIBVERSION)GetProcAddress(hSqlite3DLL_,"sqlite3_libversion");
	fn_sqlite3_sourceid = (SQLITEFUNC_SOURCEID)GetProcAddress(hSqlite3DLL_,"sqlite3_sourceid");
	fn_sqlite3_errmsg16 = (SQLITEFUNC_ERRMSG16)GetProcAddress(hSqlite3DLL_,"sqlite3_errmsg16");
	if(
		fn_sqlite3_open == NULL ||
		fn_sqlite3_open16 == NULL ||
		fn_sqlite3_exec == NULL ||
		fn_sqlite3_close == NULL ||
		fn_sqlite3_changes == NULL ||
		fn_sqlite3_reset == NULL ||
		fn_sqlite3_column_count == NULL ||
		fn_sqlite3_prepare == NULL ||
		fn_sqlite3_finalize == NULL ||
		fn_sqlite3_column_name == NULL ||
		fn_sqlite3_column_type == NULL ||
		fn_sqlite3_step == NULL ||
		fn_sqlite3_column_int == NULL ||
		fn_sqlite3_column_text == NULL ||
		fn_sqlite3_column_blob == NULL ||
		fn_sqlite3_bind_int	  == NULL ||
		fn_sqlite3_bind_int64  == NULL ||
		fn_sqlite3_bind_text	  == NULL ||
		fn_sqlite3_bind_text16 == NULL ||
		fn_sqlite3_extended_errcode==NULL||
//		fn_sqlite3_errstr==NULL ||
		fn_sqlite3_column_text16==NULL ||
		fn_sqlite3_free==NULL ||
		fn_sqlite3_prepare16_v2==NULL ||
		fn_sqlite3_prepare_v2 == NULL ||
		fn_sqlite3_clear_bindings==NULL ||
		fn_sqlite3_libversion==NULL ||
		fn_sqlite3_sourceid==NULL ||
		fn_sqlite3_errmsg16==NULL

		)
	{
		FreeLibrary (hSqlite3DLL_);
		hSqlite3DLL_ = NULL;
		return false;
	}
	return true;
}


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

BOOL createDB(LPCWSTR pDBPath, LPCSTR pTableName)
{
	if(!InitSqliteLibrary())
		return FALSE;

	char* pErr = NULL;

	std::string strResult;
	BOOL ret = FALSE;

	do
	{
		LPCSTR p;

		if ( SQLITE_OK != fn_sqlite3_open16(pDBPath, &db_) )
		{
			break;
		}


		p = 
			"CREATE TABLE [%s] (\n"
			"[c1] TEXT,\n"
			"[c2] TEXT,\n"
			"[c3] TEXT,\n"
			"[c4] TEXT);";

		char* pT = (char*)malloc(strlen(p) + strlen(pTableName) + sizeof(char));
		stlsoft::scoped_handle<char*> ma(pT, cfree);	

		sprintf(pT, p, pTableName);

		if ( SQLITE_OK != fn_sqlite3_exec(db_, 
			pT,
			callbackt,
			&strResult,
			&pErr) || pErr)
		{
			break;
		}
		SAFE_SQLERROR_FREE(pErr);

		ret = TRUE;
	} while(0);

	SAFE_SQLERROR_FREE(pErr);

	if ( db_ )
	{
		fn_sqlite3_close(db_);
		db_ = NULL;
	}

	return ret;
}




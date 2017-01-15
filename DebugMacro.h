
#include <crtdbg.h>



#if _DEBUG
	#define malloc(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__ )
	#define calloc(s1,s2) _calloc_dbg(s1, s2, _NORMAL_BLOCK, __FILE__, __LINE__ )
	#define realloc(p,s) _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
	#define free(p)  _free_dbg(p, _NORMAL_BLOCK)
	inline void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
	{
		return _malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
	}
	inline void operator delete(void *p)
	{
		_free_dbg(p, _NORMAL_BLOCK);
	}
	#define DEBUG_NEW new(__FILE__, __LINE__)
	#define new DEBUG_NEW
#endif

#if defined(_DEBUG)
	#define DASSERT(s) System::Diagnostics::Debug::Assert(!!(s))
	inline void DTRACE(System::Object^ o) { System::Diagnostics::Debug::Print(o ? o->ToString() : L"<NULL>"); } 
	#define DTRACE_NUMBER(start) do { static int num = start; DTRACE(num.ToString()); ++num; } while(0);
	#define DTRACE_NUMBERTEXT(start,text) do { static int num = start; DTRACE(num.ToString() + ":" + text); ++num; } while(0);
	#define DVERIFY DASSERT
	#define DVERIFY_IS(a,b)  DASSERT( (a)==(b) )
	#define DVERIFY_NOT(a,b) DASSERT( (a)!=(b) )
	#define DVERIFY_GL(a) do { DASSERT( (a) ); DWORD gle = GetLastError(); if ( gle!=0 ) { DASSERT(gle || 0); } } while(0)
	#define DVERIFY_GL_NUM(a,n) do { DASSERT( (a) ); DWORD gle = GetLastError(); if ( gle!=n ) { DASSERT( gle || 0); } } while(0)
	#define DVERIFY_GL_NUM2(a,n1, n2) do { DASSERT( (a) ); DWORD gle = GetLastError(); if ( gle!=n1 && gle!= n2) { DTRACE(gle.ToString()); DASSERT( gle || 0); } } while(0)
	#define DASSERT_IS_CLASS(instance, clazz) DASSERT( ((instance)->GetType())==clazz::typeid ) 
	#define DASSERT_IS_CLASS_OR_NULL(instance, clazz) DASSERT( instance==nullptr || ((instance)->GetType())==clazz::typeid ) 
	inline bool isDerived(System::Object^ o, System::Type^ t)
		{
			if ( !o || !t )
				return false;

			System::Type^ ot = o->GetType(); 
			do
			{
				if ( ot == t )
					return true;

				ot = ot->BaseType;
			} while ( ot != System::Object::typeid );

			return false;
		}
	#define DASSERT_IS_DERIVED(instance, clazz) DASSERT(isDerived(instance, clazz::typeid))
#else
	#define DASSERT(s) ((void)0)
	#define DTRACE(s) ((void)0)
	#define DTRACE_NUMBER(start) ((void)0)
	#define DTRACE_NUMBERTEXT(start,text) ((void)0)
	#define DVERIFY(a) (a)
	#define DVERIFY_IS(a,b)  (a)
	#define DVERIFY_NOT(a,b) (a)
	#define DVERIFY_GL(a) (a)
	#define DVERIFY_GL_NUM(a,n) (a)
	#define DVERIFY_GL_NUM2(a,n1, n2) (a)
	#define DASSERT_IS_CLASS(instance, clazz) ((void)0)
	#define DASSERT_IS_CLASS_OR_NULL(instance, clazz) ((void)0)
	#define DASSERT_IS_DERIVED(instance, clazz) ((void)0)
#endif

#ifdef _RELEASE
#define DTRACEBOX(s) System::Windows::Forms::MessageBox::Show(gcnew String(s))
#else
#define DTRACEBOX(s) ((void)0)
#endif


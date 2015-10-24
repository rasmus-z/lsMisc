#pragma warning(disable :4786)
#pragma warning(disable: 4503)

#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <map>
#include <set>
#include <string>
using namespace std;

#include "tstring.h"
#include "I18N.h"
#define countof(a) (sizeof(a)/sizeof(a[0]))

namespace Ambiesoft {

static CRITICAL_SECTION cs;
static void UninitCS()
{
	DeleteCriticalSection(&cs);
}
static bool InitCS();
static bool bCinit = InitCS();
static bool InitCS()
{
	if(!bCinit)
	{
		InitializeCriticalSection(&cs);
		atexit(UninitCS);
	}
	return true;
}
struct i18nlock
{
	i18nlock()
	{
		EnterCriticalSection(&cs);
	}
	~i18nlock()
	{
		LeaveCriticalSection(&cs);
	}
};

struct CFreer {
	void* p_;
	CFreer(void* p) : p_(p){}
	~CFreer() {
		free(p_);
	}
};

typedef std::map<wstring,wstring> I18NSTRINGMAP;
static I18NSTRINGMAP i18map;


static BYTE* UTF16toUTF8(LPCWSTR pIN)
{
	int nReqSize = WideCharToMultiByte(CP_UTF8,
		0,
		pIN,
		-1,
		NULL,
		0,
		NULL,
		NULL);

	if ( nReqSize == 0 )
		return NULL;

	BYTE* pOut = (BYTE*)malloc(nReqSize);
	int nRet = WideCharToMultiByte(CP_UTF8,
		0,
		pIN,
		-1,
		(char*)pOut,
		nReqSize,
		NULL,
		NULL);

	if ( nRet==0 || nRet != nReqSize )
		return NULL;

	return pOut;;
}
static LPWSTR UTF8toUTF16(const LPBYTE pIN)
{
	
	int nReqSize = MultiByteToWideChar(
		CP_UTF8,
		0,
		(const char*)pIN,
		-1,
		NULL,
		0);

	if ( nReqSize == 0 )
		return NULL;

	LPWSTR pOut = (LPWSTR)malloc(nReqSize*sizeof(WCHAR));
	int nRet = MultiByteToWideChar(CP_UTF8,
		0,
		(const char*)pIN,
		-1,
		pOut,
		nReqSize);

	if ( nRet==0 || nRet != nReqSize )
		return NULL;

	return pOut;;
}
#ifdef _DEBUG
static std::set<wstring> nai;
static std::set<wstring> aru;
void shownai()
{
	std::set<wstring>::iterator it;
	wstring message;
	for ( it = nai.begin() ; it != nai.end() ; ++it )
	{
		if(it->empty())
			continue;
		message += *it;
		message += L"\r\n";
	}

	if (!message.empty())
	{
		OutputDebugString(_T("---------------------NOTI18N------->>>>>>>>>>>>\r\n"));
		OutputDebugStringW(message.c_str());
		OutputDebugString(_T("---------------------NOTI18N-------<<<<<<<<<<<<\r\n"));
	}



	message = L"";
	for ( it = aru.begin() ; it != aru.end() ; ++it )
	{
		if(it->empty())
			continue;
		message += *it;
		message += L"\r\n";
	}

	if(!message.empty())
	{
		OutputDebugString(_T("---------------------DUPLICATEI18N------->>>>>>>>>>>>\r\n"));
		OutputDebugStringW(message.c_str());
		OutputDebugString(_T("---------------------DUPLICATEI18N-------<<<<<<<<<<<<\r\n"));
	}
}
#endif  // _DEBUG


static bool langinit=false;
static TCHAR stLang[4];

LPCTSTR i18nGetCurrentLang()
{
	return stLang;
}

LPCTSTR i18nInitLangmap(HINSTANCE hInst, LPCTSTR pLang)
{
	TCHAR szLang[4];
	if(!pLang || pLang[0]==0)
	{
		::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT ,
						LOCALE_SABBREVLANGNAME,
						szLang, 
						4);

		pLang = szLang;
	}

	assert(pLang[0]==0 || lstrlen(pLang)==3);
	if(lstrcmpi(pLang,stLang)!=0)
	{
		langinit = false;
		lstrcpy(stLang, pLang);
		i18map.clear();
	}
	
	if(!langinit)
	{
		{
			i18nlock lock;
			if(!langinit)
			{
				do
				{
					TCHAR szLang[4];
					lstrcpy(szLang, stLang);
					{
						_tcslwr(szLang);
						TCHAR szT[MAX_PATH]={0};
						GetModuleFileName(hInst,szT,(sizeof(szT)/sizeof(szT[0]))-1);
						*(_tcsrchr(szT, L'\\'))=0;

						TCHAR szTry[MAX_PATH];
						wsprintf(szTry, _T("%s\\lang\\%s.txt"),szT, szLang);

						
						FILE* f=_tfopen(szTry, _T("rb"));
						if(!f)
							break;


						BYTE* pB=NULL;
						BYTE b;
						size_t count=0;
						size_t bomcount=0;
						while(1==fread((void*)&b, 1, 1, f))
						{
							if(count == 0 && bomcount < 3)
							{
								if(bomcount==0)
								{
									if(b==0xEF)
									{
										++bomcount;
										continue;
									}
									else
										bomcount=4;
								}
								else if(bomcount==1)
								{
									if(b==0xBB)
									{
										++bomcount;
										continue;
									}
									else
									{
										++count;
										pB = (BYTE*)realloc(pB, count);
										pB[count-1]=0xFE;

										bomcount=4;
									}

								}
								else if(bomcount==2)
								{
									if(b==0xBF)
									{
										++bomcount;
										continue;
									}
									else
									{
										++count;
										pB = (BYTE*)realloc(pB, count);
										pB[count-1]=0xFE;

										++count;
										pB = (BYTE*)realloc(pB, count);
										pB[count-1]=0xBB;

										bomcount=4;
									}
								}
							}
							++count;
							pB = (BYTE*)realloc(pB, count);
							pB[count-1]=b;
						}
						++count;
						pB = (BYTE*)realloc(pB, count);
						pB[count-1]=0;

						
						WCHAR* pA = (WCHAR*)UTF8toUTF16(pB);
						free(pB);
						

						LPCWSTR pTok = wcstok(pA, L"\n");
						while(pTok)
						{
							wstring left;
							wstring right;
							int phase=0;
							bool done = false;
							bool esc = false;
							for( ; *pTok ; ++pTok)
							{
								if(phase==0)
								{
									if(*pTok==L'#')
									{
										done=true;
										break;
									}

									if(*pTok != L'"')
										continue;

									phase = 1;
									continue;
								}
								else if(phase==1)
								{
									if(esc)
									{
										esc = false;
										if(*pTok==L'n')
										{
											left+=L"\n";
										}
										else if(*pTok==L'r')
										{
											left+=L"\r";
										}
										else
											left+=*pTok;
										continue;
									}
									if(*pTok==L'\\')
									{
										esc = true;
										continue;
									}
									if(*pTok==L'"')
									{
										phase=2;
										continue;
									}
									left+=*pTok;
									continue;
								}
								else if(phase==2)
								{
									if(*pTok != L'=')
										continue;

									phase = 3;
								}
								else if(phase==3)
								{
									if(*pTok != L'"')
										continue;

									phase = 4;
									continue;
								}
								else if(phase==4)
								{
									if(esc)
									{
										esc = false;
										if(*pTok==L'n')
										{
											right+=L"\n";
										}
										else if(*pTok==L'r')
										{
											right+=L"\r";
										}
										else
										{
											right+=*pTok;
										}
										continue;
									}
									if(*pTok==L'\\')
									{
										esc = true;
										continue;
									}
									if(*pTok==L'"')
									{
										phase=5;
										continue;
									}
									right+=*pTok;
									continue;
								}
								else if(phase==5)
								{
									break;
								}
								else
								{
									assert(false);
								}
							}
							
							if(!left.empty())
							{
#ifdef _DEBUG
								if(!i18map[left].empty())
								{
									aru.insert(left);
								}
#endif
								i18map[left] = right;
							}

							pTok = wcstok(NULL, L"\n");
						}
						free(pA);

					}


				} while(0);
				
#ifdef _DEBUG
				atexit(shownai);
#endif
			}
			langinit=true;
		}
	}

	return stLang;
}

LPCWSTR I18N(LPCWSTR pIN)
{
	if(!bCinit)
		InitCS();

	if(!langinit)
		i18nInitLangmap();

	i18nlock lock;
	wstring& ret = i18map[pIN];
	if(!ret.empty())
		return ret.c_str();

#ifdef _DEBUG
	nai.insert(pIN);
#endif
	return pIN;
}



#ifdef UNICODE

static BOOL CALLBACK enumDlgChild(
  HWND hwnd,
  LPARAM lParam)
{
	TCHAR szClass[256];
	if(!GetClassName(hwnd, szClass, countof(szClass)))
		return TRUE;

	if(lstrcmpi(szClass, _T("Static"))!=0 &&
	   lstrcmpi(szClass, _T("Button"))!=0 &&
	   lstrcmpi(szClass, _T("ScrollBar"))!=0 )
	{
		return TRUE;
	}

	i18nChangeWindowText(hwnd);

	return TRUE;
}

void i18nChangeWindowText(HWND hwnd)
{
	int size = ::GetWindowTextLength(hwnd);
	if(size)
	{
		LPTSTR p = new TCHAR[size+1];
		if(size == ::GetWindowText(hwnd, p, size+1))
		{
			::SetWindowText(hwnd, I18N(p));
		}
		delete[] p;
	}
}
void i18nChangeChildWindowText(HWND hDlg)
{
	EnumChildWindows(hDlg,
		enumDlgChild,
		0);
}

void i18nChangeMenuText(HMENU menu)
{
	int count = GetMenuItemCount(menu);
	for(int i=0 ; i<count ; ++i)
	{
		int len = GetMenuString(menu, i, NULL, 0, MF_BYPOSITION|MFT_STRING);
		int buffsize = (len+1)*sizeof(TCHAR);
		TCHAR* pBuff = (TCHAR*)malloc(buffsize);
		CFreer maBuff(pBuff);
		GetMenuString(menu,i,pBuff,len+1,MF_BYPOSITION|MFT_STRING);

		LPWSTR text = _wcsdup(I18N(pBuff));
		CFreer maText(text);
		

		MENUITEMINFO miiset;
		miiset.cbSize=sizeof(miiset);
		miiset.fMask = 0x00000040; //MIIM_STRING;
		miiset.dwTypeData = text;
		SetMenuItemInfo(menu,i,TRUE, &miiset);
		


		MENUITEMINFO  mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_SUBMENU ;
		
		GetMenuItemInfo(menu, i, TRUE, &mii);

		if(mii.hSubMenu)
		{
			i18nChangeMenuText(mii.hSubMenu);
		}

		
	}
}

#endif // UNICODE

} // namespace Ambiesoft
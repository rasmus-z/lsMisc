#include <vector>
#include "tstring.h"

#define OFNBUFFERSIZE 1024
struct OFNBUFFER
{
	
	LPTSTR pFile_;
	LPTSTR pFileTitle_;
	LPTSTR pFilter_;
	OFNBUFFER():
		pFile_(new TCHAR[OFNBUFFERSIZE]),
		pFileTitle_(new TCHAR[OFNBUFFERSIZE]),
		pFilter_(new TCHAR[OFNBUFFERSIZE]) {
			pFile_[0]=0;
			pFileTitle_[0]=0;
			pFilter_[0]=0;
		}

	~OFNBUFFER() {
		delete [] pFile_;
		delete [] pFileTitle_;
		delete [] pFilter_;
	}
};

class CGetOpenFileFilter{
	std::vector<tstring> descs_;
	std::vector<tstring> exts_;
	std::vector<bool> showextsindesc_;
	tstring theFilter_;

public:
	CGetOpenFileFilter() {
	}
	void AddFilter(const tstring& desc, const tstring& ext, bool bShowExtInDesc=true)
	{
		descs_.push_back(desc);
		exts_.push_back(ext);
		showextsindesc_.push_back(bShowExtInDesc);
	}


	operator LPCTSTR() {
		size_t size = exts_.size();
		if(size==0)
			return NULL;

		theFilter_ = _T("");
		for(size_t i=0 ; i < size ; ++i)
		{
			theFilter_ += descs_[i];
			if(showextsindesc_[i])
			{
				theFilter_ += _T(" (");
				theFilter_ += exts_[i];
				theFilter_ += _T(")");
			}
			theFilter_ += _T('|');
			
			theFilter_ += exts_[i];
			theFilter_ += _T('|');
		}
		theFilter_ += _T('|');
		return theFilter_.c_str();
	}
};

BOOL GetOpenFile(
				 HWND hWnd, 
				 LPCTSTR pFilter, 
				 LPCTSTR pInitialDir,
				 LPCTSTR pTitle,
				 wstring* fullpath, wstring* path=NULL);

BOOL GetSaveFile(
				 HWND hWnd, 
				 LPCTSTR pFilter, 
				 LPCTSTR pInitialDir,
				 LPCTSTR pInitialFile,
				 LPCTSTR pDefExt,
				 LPCTSTR pTitle,
				 wstring* fullpath, wstring* path=NULL);